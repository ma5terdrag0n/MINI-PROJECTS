#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char* argv[]){
  if(argc != 3){
          printf("Wrong Number Of Arguments !!");
          return -1;
  }
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  /*---- Read the message from the server into the buffer ----*/
  
  send(clientSocket, argv[2] , strlen(argv[2]), 0);
  recv(clientSocket, buffer, 1024, 0);
  printf("Enter the path where you want to store the downloaded file :\n");
  char destination[1024];
  scanf("%s",destination);
  
  /*---- Print the received message ----*/
//  printf("Data received: %s\n",buffer);   
  FILE *fp;
  int j = 0;
  fp = fopen(destination,"w");
  fprintf(fp, "%s\n", buffer);
  fclose (fp);
  return 0;
}
