#ifndef _GPS_H
#define _GPS_H

struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
	float time;
};
long long get_longitude();
long long get_latitude();
long get_accuracy();
long location_time();

#endif
