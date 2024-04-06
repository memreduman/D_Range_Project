/*
 *	This program is written for D-Range Project running on BeagleBone Black
 *	by Emre DUMAN
 *
 */

//General Libraries and fork exec //Libraries for socket programming
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <pthread.h> //for threading , link with lpthread

#include <fcntl.h>
#include <sys/mman.h>

#include <time.h>

//PRU MEMORY
#define PRU_ADDR        0x4B200000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN         0x80000         // Length of PRU memory
#define PRU0_DRAM       0x00000         // Offset to DRAM for PRU0
#define PRU1_DRAM       0x02000			// Offset to DRAM for PRU1
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory

#define PRU_ADDR1       0x4B280000      // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN1        0x80000         // Length of PRU memory
#define PRU2_DRAM       0x00000         // Offset to DRAM for PRU0
#define PRU3_DRAM       0x02000			// Offset to DRAM for PRU1
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory

//Communatication with PRU
#define START_COMMAND   0x1AD37ACD
#define ACKNOWLEDGE   	0x0ABEF6FF

//#define distance		pru1DRAM_32int_ptr[2] //The distance value address
#define stop_distance	50 //in cm
#define DEBUG 1


	//// 	GLOBAL VARIABLES 	 ////
char *streaming_mode = "OFF";
char tcp_msg[1000] = "\0";

volatile unsigned int    *pru0DRAM_32int_ptr;        // Points to the start of local DRAM
volatile unsigned int    *pru1DRAM_32int_ptr;        // Points to the start of local DRAM
volatile unsigned int    *pru2DRAM_32int_ptr;        // Points to the start of local DRAM
volatile unsigned int    *pru3DRAM_32int_ptr;        // Points to the start of local DRAM
unsigned int    		 *prusharedMem_32int_ptr;    // Points to the start of the shared memory
	//// END OF GLOBAL VARIABLES ////

	//// FUNCTIONS ARE DECLEREAD HERE ////
void *connection_handler(void *);
	//// END OF FUNCTION DECLERATION ////

void delay(float number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

int start_pwm(int duty_1, int duty_2, char* direction, unsigned char direction2) {
    volatile unsigned int *pruDRAM_32int_ptr = pru3DRAM_32int_ptr;
    int on_time_1 = 0; // Left wheel
    int on_time_2 = 0; // Right wheel

    if(direction2==1) 		pruDRAM_32int_ptr[4] = 1;
    else if(direction2==0) 	pruDRAM_32int_ptr[4] = 0;
    else{
    	printf("\n Wrong direction2");
    	exit(1);
    }


	if(!strcmp(direction,"right")){

		if((0<=duty_1) && (duty_1 <= 10))
		on_time_1 = duty_1*250000;
		if((0<=duty_2) && (duty_2 <= 10))
		on_time_2 = duty_2*250000;

	}

	else if(!strcmp(direction,"center")){

			on_time_1 = (duty_1-1)*250000;
			on_time_2 = (duty_1-1)*250000;

		}

	else if(!strcmp(direction,"left")){

		if((0<=duty_1) && (duty_1 <= 10))
		on_time_2 = duty_1*250000;
		if((0<=duty_2) && (duty_2 <= 10))
		on_time_1 = duty_2*250000;

	}

	else if(!strcmp(direction,"stop")){

		on_time_2 = 0;
		on_time_1 = 0;

	}

    // write to PRU shared memory

    pruDRAM_32int_ptr[2] = on_time_1;  // On time
    pruDRAM_32int_ptr[3] = on_time_2; // on time

    //printf("On time - 1: %d, On time - 2: %d\n",pruDRAM_32int_ptr[2],pruDRAM_32int_ptr[3]);

    return 0;
}

int ping_pru(volatile unsigned int *pruDRAM_ptr){
	unsigned int msg = START_COMMAND;
	unsigned int answer = ACKNOWLEDGE;
	int i;
	pruDRAM_ptr[0]=msg;

	for(i=0;i<50;i++){
		if(pruDRAM_ptr[1]==answer) return 1; //wait for acknowledge;
	}
	return -1;
}

void init_pru(unsigned int pru_select){

		if(pru_select==0){

			if((ping_pru(pru0DRAM_32int_ptr))==1){
						printf("PRU0 is initialized !\n");
						pru0DRAM_32int_ptr[0]=0; //Clear start command
						pru0DRAM_32int_ptr[1]=0; //Clear acknowledge


					}
					else {
						printf("PRU0 could not initialized !\n");
						pru0DRAM_32int_ptr[0]=0; //Clear start command
						pru0DRAM_32int_ptr[1]=0; //Clear acknowledge
					}
		}
		else if(pru_select==1){
			if((ping_pru(pru1DRAM_32int_ptr))==1){

						printf("PRU1 is initialized !\n");
						pru1DRAM_32int_ptr[0]=0; //Clear start command
						pru1DRAM_32int_ptr[1]=0; //Clear acknowledge


			}
			else {
						printf("PRU1 could not initialized !\n");
						pru1DRAM_32int_ptr[0]=0; //Clear start command
						pru1DRAM_32int_ptr[1]=0; //Clear acknowledge
			}

		}
		else if(pru_select==2){
					if((ping_pru(pru2DRAM_32int_ptr))==1){

								printf("PRU2 is initialized !\n");
								pru2DRAM_32int_ptr[0]=0; //Clear start command
								pru2DRAM_32int_ptr[1]=0; //Clear acknowledge


					}
					else {
								printf("PRU2 could not initialized !\n");
								pru2DRAM_32int_ptr[0]=0; //Clear start command
								pru2DRAM_32int_ptr[1]=0; //Clear acknowledge
					}

				}
		else if(pru_select==3){
							if((ping_pru(pru3DRAM_32int_ptr))==1){

										printf("PRU3 is initialized !\n");
										pru3DRAM_32int_ptr[0]=0; //Clear start command
										pru3DRAM_32int_ptr[1]=0; //Clear acknowledge


							}
							else {
										printf("PRU3 could not initialized !\n");
										pru3DRAM_32int_ptr[0]=0; //Clear start command
										pru3DRAM_32int_ptr[1]=0; //Clear acknowledge
							}

						}


}

void start_led(char* direction){
	if(!strncmp(direction,"left",4)) pru2DRAM_32int_ptr[2]=1;
		else if(!strncmp(direction,"right",5))pru2DRAM_32int_ptr[2]=2;
		else if (!strncmp(direction,"stop",4))pru2DRAM_32int_ptr[2]=3;
		else if (!strncmp(direction,"backward",8))pru2DRAM_32int_ptr[2]=4;
		else pru2DRAM_32int_ptr[2]=5;
}

int main(int argc, char *argv[]){

	// ************ SOCKET PROGRRAMING ******************
	    int socket_desc , client_sock , c;
	    struct sockaddr_in server , client;

	    //Create socket
	    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	    if (socket_desc == -1)
	    {	if(DEBUG)
	        printf("Could not create socket");
	    }
	    if(DEBUG)
	    puts("Socket created");

	    //Prepare the sockaddr_in structure
	    server.sin_family = AF_INET;
	    //inet_addr("127.0.0.1")
	    server.sin_addr.s_addr = INADDR_ANY;
	    server.sin_port = htons( 1234 );

	    //Bind
	    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	    {
	        //print the error message
	    	if(DEBUG)
	        perror("Bind failed:Error");
	        return 1;
	    }
	    if(DEBUG)
	    puts("Bind done");

	    //Listen
	    listen(socket_desc , 3);

	    //Accept and incoming connection
	    puts("*******************************************");
	    puts("BeagleBone : D-Range Project is starting...");
	    puts("Waiting for incoming TCP connections...");

	    c = sizeof(struct sockaddr_in);
		pthread_t thread_id;

		//Wait for incoming TCP connection
			client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

			//Assign a handler
			if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
			        {
			            perror("Could not create thread");
			            return 1;
			        }
			puts("Handler assigned");

			if (client_sock < 0)
			    {
			        perror("Accept failed");
			        return 1;
			    }
	// ************ SOCKET PROGRRAMING ******************

    // MEMORY initialization

			// MEMORY initialization
	unsigned int    *pru;       // Points to start of PRU memory.
	unsigned int    *pru1;       // Points to start of PRU memory.
	int fd;

	fd = open ("/dev/mem", O_RDWR | O_SYNC);
	if (fd == -1) {
		printf ("ERROR: could not open /dev/mem.\n\n");
		return 1;
	}
	pru = mmap (0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
	pru1 = mmap (0, PRU_LEN1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR1);

	if (pru == MAP_FAILED) {
		printf ("ERROR: could not map memory.\n\n");
		return 1;
	}

	if (pru1 == MAP_FAILED) {
		printf ("ERROR: could not map memory for PRU2\n\n");
		return 1;
	}

	close(fd);
	printf ("Using /dev/mem.\n");
	pru0DRAM_32int_ptr =     pru  +  PRU0_DRAM/4 + 0x200/4;   // Points to 0x200 of PRU0 memory
	pru1DRAM_32int_ptr =     pru  +  PRU1_DRAM/4 + 0x200/4;   // Points to 0x200 of PRU1 memory
	pru2DRAM_32int_ptr = 	 pru1 +  PRU2_DRAM/4 + 0x200/4;
	pru3DRAM_32int_ptr = 	 pru1 +  PRU3_DRAM/4 + 0x200/4;
	//prusharedMem_32int_ptr = pru + PRU_SHAREDMEM/4; // Points to start of shared memory
	// MEMORY initialization is over




	pru1DRAM_32int_ptr[2]=0; // Distance in cm
	pru3DRAM_32int_ptr[2]=0;
	pru3DRAM_32int_ptr[3]=0;

    init_pru(1);  // Initialize PRU1 for URM37
    init_pru(2);  // Initialize PRU2 for LEDs
    init_pru(3);  // Initialize PRU3 for PWMs

	int left_wheel=2;
	int right_wheel=10;

	///
	///
	/// Call the video streaming program
	///
	///
	while(strcmp(streaming_mode,"ON")); // Wait the video to be streamed...
	puts("Streaming mode ON");


	pid_t pid = fork(); //Create a child process to stream Video
	if(pid < 0){
		perror("\nFork Failed.");
		exit(1);
	}
	else if(pid == 0){
		// Child now exec's
		char* args[] = {"./streamUDP",NULL};
		execv(args[0],args);
	}
	// A child program is created for video streaming

	///
	///
	///
	/// The main Program starts from here
	///
	///
	///

	while(1){

		// Read Distance
		if(pru1DRAM_32int_ptr[2] < stop_distance){
			start_pwm(right_wheel,left_wheel,"center",0); // stop when there is obstacle
			start_led("backward");
			//delay(500);
			//start_pwm(right_wheel,left_wheel,"stop",1);
			//delay(400);
		}
		else{
			// Read message
			if(strcmp(tcp_msg,"\0")){ // Wait for the first message
				start_pwm(right_wheel,left_wheel,tcp_msg,1); // Drive motors
				if(strcmp(tcp_msg,"stop")) start_led(tcp_msg);
				delay(500);
				start_pwm(right_wheel,left_wheel,"stop",1); // Drive motors
				delay(400);
			}

		}



	}



	if(munmap(pru, PRU_LEN)) {
		printf("munmap failed\n");
	}
	else{
		printf("munmap succeeded\n");
	}
	return 0;
}

/*
 * This will handle connection for each client
 *
 *
*/
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[1000];

    char  client_message_prev[1000];

    //Send some messages to the client
    message = "Greetings! I am D-Range Security Robot\n";
    write(sock , message , strlen(message));

    message = "Are you ready for streaming video ?\n";
    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 1000 , 0)) > 0 )
    {
        //end of string marker
		//client_message[read_size] = '\0';

    	if( !strcmp(streaming_mode,"ON") && strncmp(client_message_prev,client_message,1)){
    		strcpy(tcp_msg,client_message); //Copy message if there is different message than before
    		if(DEBUG)
    		//puts(client_message);           //Display it
    		puts(tcp_msg);
    	}
    	if((!strcmp(client_message,"Yes")) &&  !strcmp(streaming_mode,"OFF")){
    		 streaming_mode = "ON";
    	}

    	strncpy(client_message_prev,client_message,1);
    	//clear the message buffer
		memset(client_message, 0, 1000);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("Recv failed");
    }
    //return 0;
    start_pwm(0,0,"stop",1); // If the connection is lost stop the DC motors
    system("pkill ffmpeg");
    exit(1);
}

