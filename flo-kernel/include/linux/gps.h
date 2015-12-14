/*
* Header file of GPS location
*/

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

