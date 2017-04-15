#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int semaphores[10000];

void signal(int index);
void wait(int index);
int port;
int welcomeSocket ;
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
int newSocket;
char buffer[1024];

void *runner(void *param);

int main(int argc, char *argv[] ){
  
  memset(semaphores, 1 , sizeof semaphores);

  if(argc != 2){
          printf("INVALID NUMBER OF ARGUMENTS");
  }
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  
  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));


  // It will listen for only 10 connections at a time :: MULTITHREADING

  pthread_t tommy;
  for(int i=0;i<10;i++){
    if( pthread_create( &tommy , NULL ,  runner , (void*) i) < 0)
        {
            printf("Could not create connection with client %d",i);
            return 1;
        }
        // sleep(10);
        pthread_join(tommy, NULL);
  }
  pthread_join(tommy, NULL);
  return 0;
}


void *runner(void *threadid){
      if(listen(welcomeSocket,0)==0)
        printf("Listening Thread %d\n",(int)threadid);
      else{
        pthread_exit(1);  
      }
      socklen_t addr_size;
      // welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
      /*---- Accept call creates a new socket for the incoming connection ----*/
      addr_size = sizeof serverStorage;
      newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
       // First receive the file name from the client
      recv(newSocket,buffer,1024,0);
      char fileAddress[1024];
      strcpy(fileAddress,"/home/pritish/Desktop/collegeos/OS_Project_Part_A/LAB2/files/");
      int position = 0;
      for(int i=strlen(fileAddress) ;position < strlen(buffer);i++, ++position){
              fileAddress[i] = buffer[position];
      }
      
      int fileNumber = buffer[3] - '0' ;
      while(semaphores[fileNumber] <= 0){
        //do nothing except wait
      } // Have fun ! Wasting CPU :(
      --semaphores[fileNumber];
      // Open file name in server to read the content of the file
      FILE *fp;
      fp = fopen(fileAddress,"r");
      char data[1024];
      char ch = fgetc(fp);
      int counter = 0;
      while (ch != EOF && counter < 1024){
            data[counter++] = ch;
            ch = fgetc(fp);
      }
      fclose(fp); 
     // send data to the client !
      send(newSocket,data,strlen(data),0);
      printf("SENT BY SERVER THREAD %d\n", (int)(threadid));
      ++semaphores[fileNumber];
      pthread_exit(0);
}
