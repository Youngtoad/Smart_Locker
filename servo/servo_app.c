#include <unistd.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/sysmacros.h>  

#define SERVO_MAJOR_NUMBER   504
#define SERVO_MINOR_NUMBER 0
#define SERVO_DEV_NAME      "servo_dev"

#define SERVO_DEV_PATH 		"/dev/servo_dev"

#define INTERVAL 		50000
int main (int argc, char ** argv ){ 

	int servo_dev; 
	char buffer[1024];
	int cmd;
	
	printf("SERVO Control program\n"); 
	
	servo_dev = open(SERVO_DEV_PATH, O_WRONLY);
	printf("%d\n", servo_dev);
	if (servo_dev < 0)
	{ 
		printf("failed to open servo device\n"); 
	}
	while(1)
	{
		printf("\nInput(0~3) : ");
		scanf("%d", &cmd);
		fflush(stdin);
		ioctl(servo_dev, cmd);
		sleep(1);
	}
	//write(servo_dev, &buffer, sizeof(char));
	
	
	
	 
	close(servo_dev); 
}
