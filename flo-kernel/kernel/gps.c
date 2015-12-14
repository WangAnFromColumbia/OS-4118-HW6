#include <linux/fs.h>
#include <linux/gps.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/rwlock.h>
#include <linux/slab.h>

static struct gps_location location = {
	.latitude = 0,
	.longitude = 0,
	.accuracy = 0,
};
static DEFINE_RWLOCK(s_lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{

    	long retval = 0;
	struct gps_location k_loc;
 	if (loc == NULL)
 		return -EINVAL;
	if(!copy_from_user(&k_loc, loc, sizeof(k_loc)))
 		return -EFAULT;

	/*use lock to copy gps_location from userspace to the struct stored in kernel*/
    write_lock(&s_lock);
    memcpy(&location, &loc, sizeof(struct gps_location));
    write_unlock(&s_lock);

    return retval;
}

SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname, struct gps_location __user *, loc)
{
	return 0;
}

double get_longitude()
{
	return location.longitude;
}

double get_latitude()
{
	return location.latitude;
}

float get_accuracy()
{
	return location.accuracy;
}

float location_time()
{
	return location.time;
}
