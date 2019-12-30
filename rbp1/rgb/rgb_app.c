#include <unistd.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/sysmacros.h>  


#define RGB_MAJOR_NUMBER   502
#define RGB_MINOR_NUMBER 0
#define RGB_DEV_NAME      "rgb_dev"

#define RGB_DEV_PATH 		"/dev/rgb_dev"

#define RED_ON 1
#define GREEN_ON 4
#define BLUE_ON 3

int main (int argc, char ** argv ){ 

	int rgb_dev; 
	char buffer[1024];
	
	printf("RGB Control program\n"); 
	
	rgb_dev = open(RGB_DEV_PATH, O_RDONLY);
	printf("%d\n", rgb_dev);
	if (rgb_dev < 0)
	{ 
		printf("failed to open rgb device\n"); 
		return -1;
	}
	while(1)
	{
		ioctl(rgb_dev, RED_ON);
		sleep(1);
		ioctl(rgb_dev, GREEN_ON);
		sleep(1);
		ioctl(rgb_dev, BLUE_ON);
		sleep(1);
	}
	
	 
	close(rgb_dev); 
}
