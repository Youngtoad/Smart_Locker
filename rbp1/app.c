#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>			//통신용 헤더선언
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <wiringPi.h>           //WiringPi headers
#include <lcd.h>                //LCD headers from WiringPi

#include <unistd.h>			//rgb용 헤더선언
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/types.h> 
#include <sys/sysmacros.h>  


#define BUF_SIZE 1024		//소켓통신 포트 및 아이피 설정
#define PORT 4000
#define IPADDR "192.168.28.135"

#define LCD_RS  3               //Register select pin		LCD PIN번호 설정
#define LCD_E   0               //Enable Pin
#define LCD_D4  6              //Data pin 4
#define LCD_D5  1              //Data pin 5
#define LCD_D6  5              //Data pin 6
#define LCD_D7  4               //Data pin 7

#define RGB_DEV_NAME      	"rgb_dev"
#define HCSR04_DEV_NAME     "hcsr04_dev"
#define SERVO_DEV_NAME      "servo_dev"
#define MAG_DEV_NAME		"mag_dev"
#define KEYPAD_DEV_NAME      "keypad_dev"

#define HCSR04_DEV_PATH 	"/dev/hcsr04_dev"
#define MAG_DEV_PATH		"/dev/magnetic_dev"
#define SERVO_DEV_PATH      "/dev/servo_dev"
#define RGB_DEV_PATH 		"/dev/rgb_dev"
#define KEYPAD_DEV_PATH       "/dev/keypad_dev"

#define RED_ON 1
#define GREEN_ON 4
#define OFF 3

void *t_voice(void *data)		//멀티스레드 사용한 스피커 안내문 출력 함수
{
    
    while(1)
    {
        int num = *((int *)data);
        if (num == 4)
            pthread_exit(0);
        if(num == 1)
        system("mpg321 Hello.mp3");
        else if(num == 2)
        system("mpg321 Door.mp3");
        else if(num == 3)
        system("mpg321 Bye.mp3");
    
    }
}

int main(int argc, char* argv[])
{
	
	int client_fd, len;			//통신용 변수선언
	struct sockaddr_in client_addr;
	char recv_data[BUF_SIZE];

	int lcd;                //Handle for LCD	LCD용 변수선언
	wiringPiSetup();        //Initialise WiringPi
	
	char mode;		//통합 앱용 변수선언
	int stuid;
	int pwd;
	int wlocker;
	int a;
	char wfloor;
	int wfloor1;
	int sonic;


	int locker[8][3] = {		//락커 내부 비밀번호 저장 배열
		{0, 1000, 0000},
		{1, 1111, 1111},
		{2, 2222, 2222},
		{3, 0, 0},
		{4, 4444, 4444},
		{5, 0, 0},
		{6, 6666, 6666},
		{7, 0, 0}
	};
	int floor[2] = { 1, 1 };

	int rgb_dev;
	char buffer1[1024];		//rgb 변수 선언
	int hcsr04_dev;
	char buffer2[1024];		//초음파 센서 변수 선언
	int mag_dev;
	char buffer3[1024];
	int servo_dev; 
    char buffer4[1024];
    int keypad_dev; 
    char buffer5[1024];
	
	mag_dev = open(MAG_DEV_PATH, O_RDONLY);
	servo_dev = open(SERVO_DEV_PATH, O_WRONLY);
	rgb_dev = open(RGB_DEV_PATH, O_RDONLY);		//디바이스 오픈부
	hcsr04_dev = open(HCSR04_DEV_PATH, O_RDONLY);
	keypad_dev = open(KEYPAD_DEV_PATH, O_RDONLY);
	

	if (rgb_dev < 0)			//디바이스 제대로 열렸는지 검사
	{
		printf("failed to open rgb device\n");
		return -1;
	}
	if (hcsr04_dev < 0)
	{
		printf("failed to open hcsr04 device\n");
		return -1;
	}

	if(mag_dev < 0)
   {
      printf("Failed to open mag device\n");
      return -1;
   }
   if (servo_dev < 0)
   { 
      printf("failed to open servo device\n"); 
   }
   if (keypad_dev < 0)
   { 
      printf("failed to open servo device\n"); 
      return -1;
   }



	client_fd = socket(PF_INET, SOCK_STREAM, 0);		//소켓통신 설정
	client_addr.sin_addr.s_addr = inet_addr(IPADDR);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);
	
	if (connect(client_fd, (struct sockaddr*)
		& client_addr, sizeof(client_addr)) == -1)
	{
		printf("Can't connect\n");
		close(client_fd);
		return -1;

	} 
	
	pthread_t p_thread;	//thread
    int thr_id;
    int status;
    int t = 100;

    thr_id = pthread_create(&p_thread, NULL, t_voice, (void *)&t);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
		
	//Initialize LCD(int rows, int cols, int bits, int rs, int enable, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7)
	if (lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0)) {
		printf("lcdInit failed! \n");
		return -1;
	}
	lcdPosition(lcd, 0, 0);           //Position cursor on the first line in the first column

	
	while (1) {
		if(floor[0] == 0)
			ioctl(rgb_dev, RED_ON);		//LED불 빨간색 설정

		read(hcsr04_dev, &buffer2, sizeof(char));
		sonic = atoi(buffer2);
		sonic = sonic/58;
		if(sonic < 30){
			t = 1;
			sleep(1);
			t = 0;
			sleep(1);
			}
		
		printf("enter the mode\n");
		lcdClear(lcd);
		lcdPuts(lcd,"enter mode");
		
		while(1)
		{
			buffer5[0] = '\0';
			read(keypad_dev, &buffer5, sizeof(char));
			mode = buffer5[0];
			if (mode){				  
         
         printf("pressed: %c\n", mode);
         break;
      }
      else
         printf("no key pressed\n");

      sleep(1);
		}

		
		if (mode == 'S') {  //처음 사용자용
			sleep(1);
			buffer5[0] = '\0';
			while (1) {
				printf("enter floor\n");
				lcdClear(lcd);
				lcdPuts(lcd, "enter floor:");
				
				while(1){
					
					read(keypad_dev, &buffer5, sizeof(char));
					wfloor = buffer5[0];
					if (wfloor){ 
						printf("pressed: %c\n", wfloor);
						break;
						}
					else
						printf("no key pressed\n");

						sleep(1);
						}
				if(wfloor == '1'){
					wfloor1 = 1;
				}
				else if(wfloor == '2')
					wfloor1 = 2;
					sleep(1);
					
				if (floor[wfloor1 - 1] == 1) {
					buffer5[0] = '\0';
					while (1) {
						printf("enter student id\n");		//학번 받기
						lcdClear(lcd);
						lcdPuts(lcd, "enter studuent id");
						int n = 0;

						while (1)					//학번 문자열받기
						{
							char cid[15];
							int id;
							read(keypad_dev, &buffer5, sizeof(char));
							char x = buffer5[0];
							if (x)
							{
								printf("pressed: %c\n", x);
								if (x == 'E')
								{
									id = atoi(cid);
									cid[n] = '\n';
									printf("%d\n", id);
									stuid = id;
									cid[n] = '\0'; //flush
									id = 0;
									n = 0;
									break;
								}
								else
								{
									cid[n] = x;
									n++;
									buffer5[0] = '\0';
								}
							}
							else
								printf("no key pressed\n");

							sleep(1);
						}
						sleep(1);
						
						for (int k = 0; k < 9; k++) {		//학번 중복검사
							if (locker[k][1] - stuid == 0) {
								printf("same id\n");
								lcdClear(lcd);
								lcdPuts(lcd, "Same Id!");
								a = k;
								break;
							}
						}
						if (locker[a][1] == stuid)		//다시 학번입력받으러 돌아감
							continue;

						for (int j = 0; j < 9; j++) {		//중복되지 않았으므로 비어있는 곳에 학번 저장
							if (locker[j][1] == 0) {
								printf("save id\n");
								wlocker = j;
								locker[j][1] = stuid;
								break;
							}
						}
						sleep(1);
						printf("enter password\n");		
						lcdClear(lcd);
						lcdPuts(lcd, "enter password:");
						
						int p = 0;

						while (1)					//비번 문자열받기
						{
							
							char cid[5];
							int id;
							buffer5[0] = '\0';
							read(keypad_dev, &buffer5, sizeof(char));
							char y = buffer5[0];
							
							if (y)
							{
								printf("pressed: %c\n", y);
								if (y == 'E')
								{
									if (p != 4) {		//비번 4개인지 보고 예외 처리
										printf("password is not 4\n");
										printf("enter the another password\n");
										lcdClear(lcd);
										lcdPuts(lcd, "enter password again");
										p = 0;
										sleep(1);
										continue;
									}
									id = atoi(cid);
									cid[p] = '\n';
									printf("%d\n", id);
									pwd = id;
									cid[p] = '\0'; //flush
									id = 0;
									p = 0;
									break;
								}
								else
								{
									cid[p] = y;
									p++;
									buffer5[0] = '\0';
								}
							}
							else
								printf("no key pressed\n");

							sleep(1);
						}



						locker[wlocker][2] = pwd;		//비번 저장
						for(int i = 0; i < 4; i++){
							if(locker[i][1] != 0)
								floor[0] = 0;
							}
							for(int i = 4; i < 8; i++){
							if(locker[i][1] != 0)
								floor[1] = 0;
							}
							if(floor[0] == 0)
								ioctl(rgb_dev, RED_ON);
						printf("success!\n");
						lcdClear(lcd);
						lcdPuts(lcd, "Success!");
						break;
					}
					sleep(1);
					mode = 'G';		//기존 사용자용으로 보내서 다시 처음부터 하게만듬
					break;

				}
				else {
					printf("floor is full\n");
					lcdClear(lcd);
					lcdPuts(lcd, "floor is full!");
					continue;
				}
				mode = 'G';		//기존 사용자용으로 보내서 다시 처음부터 하게만듬
				break;
			}


		}

		 if (mode == 'G') {		//기존 사용자
			 sleep(1);
			lcdClear(lcd);
			
			while (1) {
				printf("enter student id\n");
				lcdPuts(lcd, "enter student id");
				
				while(1){
					int b = 0;
					while (1)					//학번 문자열받기
				{
					
					char cid[15];
					int id;
					read(keypad_dev, &buffer5, sizeof(char));
					char x = buffer5[0];
					if (x)
					{
						printf("pressed: %c\n", x);
						if (x == 'E')
						{
							id = atoi(cid);
							cid[b] = '\n';
							printf("%d\n", id);
							stuid = id;
							cid[b] = '\0'; //flush
							id = 0;
							b = 0;
							break;
						}
						else
						{
							cid[b] = x;
							b++;
						}
					}
					else
						printf("no key pressed\n");

					sleep(1);
				}

				for (int m = 0; m < 9; m++) {
					if (locker[m][1] - stuid == 0) {
						wlocker = m;
					}
					
				}
				if(locker[wlocker][1] == 0){		//아이디 중복검사
					printf("id is false\n");
						lcdClear(lcd);
						lcdPuts(lcd, "id again");
						sleep(1);
						continue;
					}
				if(locker[wlocker][1] - stuid == 0){
					break;
				}
				else{
						printf("id is false\n");
						printf("id again\n");
						lcdClear(lcd);
						lcdPuts(lcd, "id again");
						sleep(1);
						continue;
						}
				}

				
				sleep(1);

				printf("enter password\n");
				lcdClear(lcd);
				lcdPuts(lcd, "enter password");
				
						int p = 0;
						while (1)					//비번 문자열받기
						{
							
							char cid[5];
							int id;
							read(keypad_dev, &buffer5, sizeof(char));
							char y = buffer5[0];
							if (y)
							{
								printf("pressed: %c\n", y);
								if (y == 'E')
								{
									if (p != 4) {		//비번 4개인지 보고 예외 처리
										printf("password is not 4\n");
										printf("enter the another password\n");
										lcdClear(lcd);
										lcdPuts(lcd, "enter password again");
										p = 0;
										sleep(1);
										continue;
									}
									id = atoi(cid);
									cid[p] = '\n';
									printf("%d\n", id);
									pwd = id;
									cid[p] = '\0'; //flush
									id = 0;
									p = 0;
									break;
								}
								else
								{
									cid[p] = y;
									p++;
								}
							}
							else
								printf("no key pressed\n");

							sleep(1);
						}

					if (locker[wlocker][2] - pwd == 0) {		//비번이 동일한지 확인
						printf("verifiy\n");
						break;
					}
					else {								//아닐경우 예외처리
						printf("password again\n");
						sleep(1);
						continue;
					}
				}
				sleep(1);

				char* cword;
				cword = malloc(sizeof(int));
				sprintf(cword, "%d", wlocker + 1);
				write(client_fd, cword, 2);		//서버에 사물함번호 wlocker보내야함


				printf("locker is opened, led is green\n");	
				lcdClear(lcd);
				lcdPuts(lcd, "locker open");

				ioctl(servo_dev, 0);
				ioctl(rgb_dev, GREEN_ON);				//LED 초록불 점등
				sleep(1);

				
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
									read(hcsr04_dev, &buffer2, sizeof(char));
									sonic = atoi(buffer2);
									sonic = sonic/58;
									if(sonic > 25)
									{
										t = 2;
										sleep(1);
										t = 0;
										printf("close the door!\n");
										printf("result = %d\n", sonic);
										lcdClear(lcd);
										lcdPuts(lcd, "close the door");
										sleep(1);
									}
								}
								else {								//문이 닫혀있다면
									sleep(2);
									ioctl(servo_dev, 1);
									sleep(1);
									//write(client_fd, cword , 2);
									ioctl(rgb_dev, OFF);
									t = 3;
									sleep(1);
									t = 4;
									printf("bye!\n");
									lcdClear(lcd);
									lcdPuts(lcd, "bye!");
									sleep(1);
									break;
								}
											}
					
						break;
				}

				
			}
			
		else if(mode == 'P'){
			write(client_fd, "9", 2);
			break;
			}
		
		else {       //에러 메세지 출력
			printf("mode is false!\n");
			lcdClear(lcd);
			lcdPuts(lcd, "mode is false");
			continue;
		}

		}
	t = 4;
	sleep(3);
	pthread_join(p_thread, (void **)&status);	
	close(rgb_dev);		//센서 디바이스 닫아줌
	close(hcsr04_dev);
	close(mag_dev);
	close(client_fd);	//소켓통신 닫음
	return 0;
	}

	

