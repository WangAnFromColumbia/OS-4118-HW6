#ifndef _GPS_H
#define _GPS_H

struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
};

#endif
