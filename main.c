#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <sys/ioctl.h> 
#include <sys/sysmacros.h>  

#include "rotate.h"

#define SERVO_MAJOR_NUMBER   504
#define SERVO_MINOR_NUMBER 0
#define SERVO_DEV_NAME      "servo_dev"

#define SERVO_DEV_PATH 		"/dev/servo_dev"

#define LOCK 31
#define UNLOCK 32
#define C_LOCK 33


#define PORT 4000
#define BUF_LEN 1024

int main()
{
	char buffer_rcv [BUF_LEN];
	char buffer_snd[BUF_LEN];
 
	static struct sockaddr_in server_addr,client_addr;
	char temp[20];
	int server_fd,client_fd;
	int len,msg_size;
	char test[20];
	int servo_dev;
	int c_room[2];
	int input;
	int lock;
	int floor;
	int room;
	
	c_room[0] = 0;
	c_room[1] = 0;
	
	servo_dev = open(SERVO_DEV_PATH, O_WRONLY);
	printf("%d\n", servo_dev);
	if (servo_dev < 0)
	{ 
		printf("failed to open servo device\n"); 
	}
	
	printf("raspi 2 power on!\n\n");
	
	if((server_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("Server: can not Open Socket\n");
        exit(0);
    }

    memset(&server_addr,0x00,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    if(bind(server_fd, (struct sockaddr *)
            &server_addr, sizeof(server_addr))< 0)
    {
        printf("Server: cat not bind local addrss\n");
        exit(0);
    }
    
    if(listen(server_fd,5) < 0)
    {
        printf("Server: cat not listen connnect.\n");
        exit(0);
    }
    
	printf("=====[PORT] : %d =====\n", PORT);
	printf("Server : wating connection request.\n\n");
    
    
    len = sizeof(client_addr);
    client_fd = accept(server_fd,(struct sockaddr *)&client_addr,(socklen_t *)&len);
	
	while(1)
	{
		if(client_fd ==-1)
		{
			printf("Server: accept failed\n");
			exit(0);
		}
		read(client_fd,buffer_rcv,BUF_LEN);
		printf("recieve: %s\n",buffer_rcv);
		input = atoi(buffer_rcv);
		if(input > 8) break;
		input--;
		floor = input/4;
		room = input%4;
		
		printf("User's cabinet is : Floor => %d, Room => %d\n", floor + 1, room + 1);
		printf("Current front cabinet : Floor 1 => %d, Floor 2 => %d\n", c_room[0] + 1, c_room[1] + 1);
		rotate(floor, room, c_room[floor], servo_dev);
		sleep(1);
		c_room[floor] = room;
		printf("After cabinet : Floor 1 => %d, Floor 2 => %d\n\n\n", c_room[0] +1 , c_room[1] + 1);
		printf("Server : wating connection request.\n\n");
		
		//client_fd = accept(server_fd,(struct sockaddr *)&client_addr,(socklen_t *)&len);
	}
	
	
	printf("Program exited!!!\n");
	return 0;
	
	
	
}
