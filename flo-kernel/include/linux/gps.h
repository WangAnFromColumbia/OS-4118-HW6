/*
* Header file of GPS location
*/
#ifndef __GPS_H__
#define __GPS_H__


struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
};

struct gps_kdata {
    unsigned char m_lat[8]; /* 64 but double */
    unsigned char m_lon[8]; /* 64 but double */
    unsigned char m_acc[4]; /* 32 but double */
    unsigned char m_age[4]; /* 32 but double */
};

extern void get_gps_data(struct gps_kdata *data);

#endif