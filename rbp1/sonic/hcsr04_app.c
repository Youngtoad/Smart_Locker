#include <unistd.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/sysmacros.h>  


#define HCSR04_MAJOR_NUMBER 505
#define HCSR04_MINOR_NUMBER 0
#define HCSR04_DEV_NAME      "hcsr04_dev"

#define HCSR04_DEV_PATH 		"/dev/hcsr04_dev"

int main (int argc, char ** argv ){ 

	int hcsr04_dev;
	int sonic; 
	char buffer[1024];
	
	printf("HCSR04 Control program\n"); 
	
	hcsr04_dev = open(HCSR04_DEV_PATH, O_RDONLY);
	printf("%d\n", hcsr04_dev);
	if (hcsr04_dev < 0)
	{ 
		printf("failed to open hcsr04 device\n"); 
		return -1;
	}
	while(1)
	{
		read(hcsr04_dev, &buffer, sizeof(char));
		sonic = atoi(buffer);
		//printf("result = %s", buffer);
		printf("result = %d\n", sonic/58);
		sleep(1);
		
	}
	
	 
	close(hcsr04_dev); 
}
