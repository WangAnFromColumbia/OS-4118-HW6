#include <linux/fs.h>
#include <linux/gps.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/rwlock.h>

static struct gps_location location;
static DEFINE_RWLOCK(s_lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
    long retval = 0;
    unsigned long gps_loc_size = sizeof(struct gps_location);
    struct gps_location kloc;

    if (loc == NULL)
        return -EINVAL;

    if (copy_from_user(&kloc, loc, gps_loc_size) != gps_loc_size) {
        printk("copy from user failure!\n");
        return -EINVAL;
    }

    write_lock(&s_lock);
    memcpy(&location, &loc, sizeof(double));
    location.time = CURRENT_TIME_SEC.tv_sec;
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
