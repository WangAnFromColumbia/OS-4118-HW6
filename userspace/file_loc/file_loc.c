#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "file_loc.h"


/* The user space utility, file_loc, should take exactly one argument which is
 * the path to a file or directory. It should then print out the GPS
 * coordinates / accuracy and data age of the specified file or directory, and
 * also format a Google Maps URL which can be pasted into a web browser to view
 * the location.
 * Example: https://www.google.com/maps/@40.8167651,-73.96536,15z
 */

int main(int argc, char **argv)
{
    int retval = 0;
    char *pathname;
    char *g_url = "https://www.google.com/maps/@";
    struct gps_location loc;

    if (argc == 2) {
        pathname = argv[1];
    } else{
        printf("Enter a filepath. Try again!\n");
        return -1;
    }

    printf("GPS read from file: %s\n", pathname);

    retval = get_gps_location(pathname, &loc);

    if (retval < 0) {
        printf("__NR_set_gps_location failed\n");
        printf("retval: %f\n", (float)retval);
        return -1;
    }

    printf("__NR_set_gps_location success\n");

    printf("latitude: %f longitude: %f accuracy: %f age: %d\n",
            loc.latitude, loc.longitude, loc.accuracy, retval);

    printf("Google Maps URL: %s%f,%f,15z\n",
            g_url, loc.latitude, loc.longitude);

    return 0;
}