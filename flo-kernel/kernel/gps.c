#include <linux/syscalls.h>
#include <linux/gps.h>
#include <linux/rwlock.h>
#include <linux/uaccess.h>
#include <linux/time.h>


static struct gps_kdata s_kdata = {
    .m_lat = {0, 0, 0, 0, 0, 0, 0},
    .m_lon = {0, 0, 0, 0, 0, 0, 0},
    .m_acc = {0, 0, 0, 0},
    .m_age = {0, 0, 0, 0},
};
static DEFINE_RWLOCK(s_lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user*, loc)
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

    memcpy(s_kdata.m_lat, &loc->latitude, sizeof(double));
    memcpy(s_kdata.m_lon, &loc->longitude, sizeof(double));
    memcpy(s_kdata.m_acc, &loc->accuracy, sizeof(float));
    memcpy(s_kdata.m_age, &CURRENT_TIME_SEC, sizeof(unsigned long));


    write_unlock(&s_lock);

    return retval;
}

void get_gps_data(struct gps_kdata *data)
{
    read_lock(&s_lock);

    memcpy(data->m_lat, s_kdata.m_lat, sizeof(double));
    memcpy(data->m_lon, s_kdata.m_lon, sizeof(double));
    memcpy(data->m_acc, s_kdata.m_acc, sizeof(float));
    memcpy(data->m_age, s_kdata.m_age, sizeof(unsigned long));

    read_unlock(&s_lock);
}