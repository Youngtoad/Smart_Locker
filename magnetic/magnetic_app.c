#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char ** argv)
{
   char buffer3[10];
   
   int mag_dev=open("/dev/magnetic_dev", O_RDONLY);
   
   if(mag_dev<0)
   {
      perror("Failed to open the device");
      return -1;
   }
           // int sonic;
				//char door;//open=1, close=0

				while (1)		
				{
               while(1){
                  read(mag_dev, &buffer3, sizeof(int));
                  if(buffer3[0] == '0'){
                     printf("door is not opened yet\n");
                     sleep(1);
                     }
                     else{
                        printf("door is opened\n");
                        break;
                        }
                  }
                  sleep(1);
                  while(1){
                     read(mag_dev, &buffer3, sizeof(int));
					printf("check\n");
					//door = buffer3[0];
					printf("%c\n", buffer3[0]);		//문이 열렷는지 계속 확인

					if (buffer3[0] == '3') {					//문이 열려있다면
						//read(hcsr04_dev, &buffer2, sizeof(char));
						//sonic = atoi(buffer2);
						//if(sonic > 1300)
						//{
							printf("close the door!\n");
						//	printf("result = %s", buffer2);
							//lcdClear(lcd);
							//lcdPuts(lcd, "close the door");
						//}
                  sleep(1);
					}
					else {								//문이 닫혀있다면
						//write(client_fd, cword , 2);
						printf("bye!\n");
						//lcdClear(lcd);
						//lcdPuts(lcd, "bye!");
						sleep(1);
						break;
					}
                     }
					
               break;
					
				}
   
   close(mag_dev);
}
