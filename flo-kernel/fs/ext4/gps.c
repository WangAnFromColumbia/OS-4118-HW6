#include <linux/time.h>
#include <linux/gps.h>
#include "ext4.h"

int ext4_set_gps_location(struct inode *inode)
{
	long long longitude;
	long long latitude;
	long accuracy;
	long age;
	struct ext4_inode_info *ei = EXT4_I(inode);
	if (!ei)
		return -ENODEV;
	longitude = get_longitude();
	latitude = get_latitude();	
	accuracy = get_accuracy();
	age = CURRENT_TIME_SEC.tv_sec - location_time();

	write_lock(&ei->s_lock);
	memcpy(&ei->i_latitude, &latitude, sizeof(long long));
	memcpy(&ei->i_longitude, &longitude, sizeof(long long));
	memcpy(&ei->i_accuracy, &accuracy, sizeof(long));
	memcoy(&ei->i_coord_age, &age, sizeof(long));
	write_unlock(&ei->s_lock);
	return 0;
}

int ext4_get_gps_location(struct inode *inode, struct gps_location *location)
{
	struct gps_location loc;
	struct ext4_inode_info *ei = EXT4_I(inode);
	if (!ei)
		return -ENODEV;
	
	read_lock(&ei->s_lock);
	memcpy(&loc.latitude, &ei->i_latitude, sizeof(unsigned long long));
	memcpy(&loc.longitude, &ei->i_longitude, sizeof(unsigned long long));
	memcpy(&loc.accuracy, &ei->i_accuracy, sizeof(unsigned int));
	memcpy(location, &loc, sizeof(loc));
	read_unlock(&ei->s_lock);

	return 0;
}
