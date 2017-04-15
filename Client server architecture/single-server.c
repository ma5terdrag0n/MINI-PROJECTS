#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[] ){
  
  if(argc != 2){
          printf("INVALID NUMBER OF ARGUMENTS");
  }
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  
  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,0)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  /*---- Send message to the socket of the incoming connection ----*/
  
  // First receive the file name from the client
  recv(newSocket,buffer,1024,0);
  char fileAddress[1024];
  strcpy(fileAddress,"/home/pritish/Desktop/collegeos/OS_Project_Part_A/LAB2/files/");
  int position = 0;
  for(int i=strlen(fileAddress) ;position < strlen(buffer);i++, ++position){
          fileAddress[i] = buffer[position];
  }
  
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
  printf("****************SENT*****************\n");
  return 0;
}
