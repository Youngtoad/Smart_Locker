#include <unistd.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <wiringPi.h>

#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/sysmacros.h>  

#define KEYPAD_DEV_NAME      "keypad_dev"

#define KEYPAD_DEV_PATH       "/dev/keypad_dev"

int main (int argc, char ** argv ){ 

   int keypad_dev; 
   char buffer[1024];
   
   printf("Keypad Control program\n"); 
   
   keypad_dev = open(KEYPAD_DEV_PATH, O_RDONLY);
   printf("%d\n", keypad_dev);
   if (keypad_dev < 0)
   { 
      printf("failed to open servo device\n"); 
      return -1;
   }
   while(1)
   {
      read(keypad_dev, &buffer, sizeof(char));
      printf("%s\n", buffer);
      //sleep();
      sleep(1);
   }
   
    
   close(keypad_dev); 
}
