#ifndef _GPSD_H_
#define _GPSD_H_
/*
 * gpsd.h
 *
 * Columbia University
 * COMS W4118 Fall 2014
 * Homework 6
 *
 */

#include <sys/syscall.h>

struct gps_location {
	double  latitude;
	double  longitude;
	float   accuracy;   /* accuracy in meters */
};

#define GPS_LOCATION_FILE "/data/media/0/gps_location.txt"

static inline int set_gps_location(struct gps_location *loc)
{
	return syscall(__NR_set_gps_location, loc);
}

#endif
