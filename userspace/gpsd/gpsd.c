#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "gpsd.h"

void daemonize(void)
{
	pid_t pid = fork();

	if (pid < 0) {
		perror("Error: fork failed");
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		/* close parent */
		exit(EXIT_SUCCESS);
	} else if (setsid() < 0) {
		perror("Error: setsid failed");
		exit(EXIT_FAILURE);
	}
	close(0);
	close(1);
	close(2);
	chdir("/");
	umask(0);

	/* fork again? */
}

void update_gps_location(void)
{
	FILE *file;
	struct gps_location location;

	file = fopen(GPS_LOCATION_FILE, "r");
	if (file == NULL) {
		perror("Error: cannot open gps file\n");
		return;
	}
	if (fscanf(file, "%lf %lf %f", &location.latitude,
				       &location.longitude,
				       &location.accuracy) != 3)
		perror("Error: cannot parse gps_location\n");
	fclose(file);

	set_gps_location(&location);
}

int main(int argc, char *argv[])
{
	daemonize();

	while (1) {
		update_gps_location();
		usleep(1000 * 1000);
	}

	return 0;
}

