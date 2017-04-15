#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>


int semaphores[10000];
int port;
char DownloadSingleFile[111];
void *runner(void *param);

int main(int argc, char* argv[]){
  if(argc != 3){
          printf("Wrong Number Of Arguments !!");
          return -1;
  }
  memset(semaphores, 1, sizeof semaphores);
  strcpy(DownloadSingleFile, argv[2]);
  port = atoi(argv[1]);
  pthread_t pid;
  for(int i=0;i<10;i++){
    if( pthread_create( &pid , NULL ,  runner , (void*) i) < 0)
        {
            printf("Client Thread %d Could not create connection with Server !!",i);
            return 1;
        }
        // sleep(10);
        pthread_join(pid,0);
  }
  pthread_join(pid,0);
  return 0;
}



void *runner(void *tid){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  /*---- Read the message from the server into the buffer ----*/


  while(semaphores[DownloadSingleFile[3] + '0'] <= 0){

  }
  semaphores[DownloadSingleFile[3] + '0']--;
  send(clientSocket,  DownloadSingleFile , strlen(DownloadSingleFile), 0);
  recv(clientSocket, buffer, 1024, 0);
  char fileName[100] = "/home/pritish/Desktop/collegeos/OS_Project_Part_A/LAB2/";
  int i=0;
  i=strlen(fileName);
  fileName[i++] = (int)tid + '0';
  fileName[i++] = '.';
  fileName[i++] = 't';
  fileName[i++] = 'x';
  fileName[i++] = 't';
  
  printf("%s RECEIVED : BY CLIENT THREAD %d", fileName, (int)tid);

  /*---- Print the received message ----*/
//  printf("Data received: %s\n",buffer);   
  FILE *fp;
  int j = 0;
  fp = fopen(fileName,"w");
  fprintf(fp, "%s\n", buffer);
  fclose (fp);
  pthread_exit(0);
}
