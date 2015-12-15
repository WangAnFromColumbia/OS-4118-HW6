#ifndef _GPS_H
#define _GPS_H

struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
	float time;
};
double get_longitude(void);
double get_latitude(void);
float get_accuracy(void);
float location_time(void);

#endif
