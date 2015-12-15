#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/gps.h>
#include <linux/rwlock.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/namei.h>
#include <linux/limits.h>
#include <linux/slab.h>

/* debug define */
#define log printk
#define R_OK    4

/* my own repository */
static struct gps_kdata s_kdata = {
    .m_lat = {0, 0, 0, 0, 0, 0, 0, 0},
    .m_lon = {0, 0, 0, 0, 0, 0, 0, 0},
    .m_acc = {0, 0, 0, 0},
    .m_age = {0, 0, 0, 0},
};

static DEFINE_RWLOCK(s_lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
    long retval = 0;
    unsigned long sz = sizeof(struct gps_location);
    struct gps_location kloc;
    unsigned long cur_time = get_seconds();

    /* safety check */
    if (loc == NULL) {
        log("loc null failure\n");
        return -EINVAL;
    }

    if (copy_from_user(&kloc, loc, sz) != 0) {
        log("copy_from_user failure\n");
        return -EINVAL;
    }

    write_lock(&s_lock);

    memcpy(s_kdata.m_lat, &loc->latitude, sizeof(double));
    memcpy(s_kdata.m_lon, &loc->longitude, sizeof(double));
    memcpy(s_kdata.m_acc, &loc->accuracy, sizeof(float));
    memcpy(s_kdata.m_age, &cur_time, sizeof(unsigned long));

    write_unlock(&s_lock);

    return retval;
}

SYSCALL_DEFINE2(get_gps_location,
                const char __user *, pathname,
                struct gps_location __user *, loc)
{
    int retval = 0;
    char *s_kpathname;
    struct gps_location s_kloc;
    struct inode *file_ind;
    struct path s_kpath;

    if (sys_access(pathname, R_OK) != 0) {
        log("sys_access failure\n");
        return -EACCES;
    }

    s_kpathname = kmalloc(sizeof(char)*PATH_MAX, GFP_KERNEL);

    if (s_kpathname == NULL) {
        log("s_kpathname failure\n");
        return -ENOMEM;
    }

    if (strncpy_from_user(s_kpathname, pathname, PATH_MAX) < 0) {
        log("strncpy_from_user failure\n");
        kfree(s_kpathname);
        return -EFAULT;
    }

    log("s_kpathname: %s\n", s_kpathname);

    if (kern_path(s_kpathname, LOOKUP_FOLLOW, &s_kpath) != 0) {
        kfree(s_kpathname);
        log("kern_path failure\n");
        return -EAGAIN;
    }

    file_ind = s_kpath.dentry->d_inode;
    log("s_kpath dentry name: %s\n", s_kpath.dentry->d_name.name);

    if (file_ind == NULL) {
        log("file_ind failure\n");
        path_put(&s_kpath);
        kfree(s_kpathname);
        return -EINVAL;
    }

    if (generic_permission(file_ind, MAY_READ) != 0) {
        path_put(&s_kpath);
        kfree(s_kpathname);
        return -EPERM;
    }

    /* check if the file is in ext3 */
    if (file_ind->i_op == NULL ||
        file_ind->i_op->get_gps_location == NULL) {

        log("if the file is in ext3 failure\n");
        path_put(&s_kpath);
        kfree(s_kpathname);
        return -ENODEV;
    }

    /* get the gps loc of a file */
    retval = file_ind->i_op->get_gps_location(file_ind, &s_kloc);

    if (copy_to_user(loc, &s_kloc, sizeof(struct gps_location)) != 0) {
        log("copy_to_user failure\n");
        path_put(&s_kpath);
        kfree(s_kpathname);
        return -EFAULT;
    }

    /* @lfred: test - decrement the refcount */
    path_put(&s_kpath);
    kfree(s_kpathname);
    log("get_gps_location done!\n");
    return retval;
}

void get_gps_data(struct gps_kdata *data)
{
    unsigned long curr_age;

    read_lock(&s_lock);

    memcpy(&data->m_lat, s_kdata.m_lat, sizeof(double));
    memcpy(&data->m_lon, s_kdata.m_lon, sizeof(double));
    memcpy(&data->m_acc, s_kdata.m_acc, sizeof(float));

    /* this is for debugging - check if it works */
#if 1
    memcpy(&curr_age, s_kdata.m_age, sizeof(float));
    curr_age = get_seconds() - curr_age;
    memcpy(&data->m_age, &curr_age, sizeof(unsigned long));
#else
    curr_age = get_seconds();
#endif
    read_unlock(&s_lock);

    /* @lfred: use log to check if we are reallt running */
    log("[HW6] get_gps_data: current time: %ld\n", curr_age);
}