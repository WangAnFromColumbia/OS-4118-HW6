#include <linux/gps.h>

static struct gps_location location;

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location k_loc;
	if (loc == NULL)
		return -EINVAL;
	if(!copy_from_user(&k_loc, loc, sizeof(k_loc)))
		return -EFAULT;
	/*use lock to copy gps_location from userspace to the struct stored in kernel*/
	return 0;
}

SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname, struct gps_location __user *, loc)
{

}

long long get_longitude()
{
	return location.longitude;
}

long long get_latitude()
{
	return location.latitude;
}

long get_accuracy()
{
	return location.accuracy;
}

long location_time()
{
	return location.time;
}
