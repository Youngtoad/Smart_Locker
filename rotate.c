#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 

#include <sys/ioctl.h> 
#include <sys/sysmacros.h>  

#include "rotate.h"


#define LOCK 31
#define UNLOCK 32
#define C_LOCK 33

#define MOTOR1_RIGHT 0
#define MOTOR1_LEFT 1
#define MOTOR2_RIGHT 4
#define MOTOR2_LEFT 3

#define SERVO1 1 	//floor 1
#define SERVO2 26	//floor 2
#define SERVO3 24	//door lock
#define DEGREE90	60000

int rotateRight(int floor, int step, int servo_dev)
{
	int i;
	if(floor == 0)
	{
		for(i = 0; i < step ; i ++)
		{
			ioctl(servo_dev, MOTOR1_RIGHT);
			sleep(1);
		}
	}
	else
	{
		for(i = 0; i < step ; i ++)
		{
			ioctl(servo_dev, MOTOR2_RIGHT);
			sleep(1);
		}
	}
}

int rotateLeft(int floor, int step, int servo_dev)
{
	int i;
	if(floor == 0)
	{
		for(i = 0; i < step ; i ++)
		{
			ioctl(servo_dev, MOTOR1_LEFT);
			sleep(1);
		}
		sleep(1);
	}
	else
	{
		for(i = 0; i < step ; i ++)
		{
			ioctl(servo_dev, MOTOR2_LEFT);
			sleep(1);
		}
		
	}
}

int rotate(int floor, int room, int c_room, int servo_dev)
{
	int temp, move;
	temp = c_room - room;
	
	if(temp == 0)
	{
		printf("Don't rotate, temp = %d\n", temp);
	}
	else if(temp%2 == 0)
	{
		printf("Rotate Floor %d right two steps, temp = %d\n", floor + 1, temp);
		rotateRight(floor, 2, servo_dev);
		
	}
	else if(temp > 0)
	{
		move = temp - 2;
		if(move > 0)
		{
			printf("Rotate Floor %d right one steps, temp = %d\n", floor + 1, temp);
			rotateRight(floor, 1, servo_dev);
		}
		else
		{
			printf("Rotate Floor %d left one steps, temp = %d\n", floor + 1, temp);
			rotateLeft(floor, 1, servo_dev);
		}
	}
	else if(temp < 0)
	{
		move = temp + 2;
		if(move > 0)
		{
			printf("Rotate Floor %d right one steps, temp = %d\n", floor + 1, temp);
			rotateRight(floor, 1, servo_dev);
		}
		else
		{
			printf("Rotate Floor %d left one steps, temp = %d\n", floor + 1, temp);
			rotateLeft(floor, 1, servo_dev);
		}
	}
}
