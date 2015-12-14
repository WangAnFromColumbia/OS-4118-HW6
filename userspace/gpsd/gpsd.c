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

static int read_gpsloc(FILE *fd, struct gps_location *loc)
{
	if (fd == NULL || loc == NULL) {
		log("Null FILE or gps_location in read_gpsloc");
		return -1;
	}
	fscanf(fd, "%lf", &loc->latitude);
	fscanf(fd, "%lf", &loc->longitude);
	fscanf(fd, "%f",  &loc->accuracy);

	log("Read GPS_LOCATION_FILE latitude: %f longitude: %f accuracy: %f\n",
			loc->latitude, loc->longitude, loc->accuracy);

	return 1;
}


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

	/* Fork to get rid of TTY */
	pid = fork();
	if (pid < 0) {
		printf("Failed to fork\n");
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		printf("nst parent to terminate\n");
		exit(EXIT_SUCCESS);
	}
	/* Set up directory */
	if (chdir("/") < 0) {
		printf("Failed to change dir\n");
		exit(EXIT_FAILURE);
	}

	umask(0);

	fd = open("/dev/null", O_RDWR);
	if (fd < 0) {
		log("Failed to open null dev\n");
		exit(EXIT_FAILURE);
	}
	dup2(fd, 0);
	dup2(fd, 1)
	dup2(fd, 2);
	close(fd);
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
	FILE *fd = NULL;
	struct gps_location loc;

	daemonize();

	while (1) {
		fd = fopen(GPS_LOCATION_FILE, "r");
		if (fd == NULL) {
			log("Failed to open GPS_LOCATION_FILE\n");
			exit(EXIT_FAILURE);
		}

		if (read_gpsloc(fd, &loc) < 0) {
			log("Failed to read_gpsloc\n");
			fclose(fd);
			exit(EXIT_FAILURE);
		}

		if (set_gps_location(&loc) < 0)
			log("Failed to set_gps_location\n");

		/*read the values once every second*/
		sleep(1);
		fclose(fd);
	}

	return 0;
}
