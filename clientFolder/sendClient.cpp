#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>

using namespace std;

int main(int argc, char* argv[]){

  int sockID, port, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[1024];

  if(argc != 3){
    cout << "Incorect usage\n./sendClient <server-hostname> <server-port>\n";
    exit(0);
  }
  port = atoi(argv[2]);

  sockID = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <=0){
    cout << "ERROR: invalid server address\n";
    exit(0);
  }
  
  if(connect(sockID,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
    cout << "ERROR: connect error\n";
    exit(0);
  }


  n = read(sockID, buffer, 1024);
  if(n < 0){
    cout << "ERROR: read from stream\n";
    exit(0);
  }
  cout << buffer << endl;
  bzero(buffer, 1024);
  fgets(buffer, 1024, stdin);
  
  n= write(sockID, buffer, 1024);  // write hello
  if(n < 0){
    cout << "ERROR: write to stream\n";
    exit(0);
  }  
  bzero(buffer, 1024);
  n = read(sockID, buffer, 1024);  // read hello
  if(n < 0){
    cout << "ERROR: read from stream\n";
    exit(0);
  }
  cout << buffer << endl;
  bzero(buffer, 1024);
  fgets(buffer, 1024, stdin);
  n = write(sockID, buffer, 1024);  // write mail from
  bzero(buffer,1024);
  n = read(sockID, buffer, 1024);  // read mail from responce
  cout << buffer << endl;
  
  return 0;
}
