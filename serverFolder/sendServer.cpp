#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <string>

using namespace std;

static void * connection(void * clientSock);

int main(int argc, char *argv[]){
  if(argc != 2){
    cout << "ERROR: invalid paramaters.\n./server <TCP port>\n";
    exit(0);
  }
  
  cout << "Greetings Master\n";

  int tcpSock,clientSock, tcpPort, client, n;
  char buffer[1024] = {0};
  struct sockaddr_in tcp_addr, cli_addr;
  pthread_t thread_id;
  tcpSock = socket(AF_INET, SOCK_STREAM, 0);

  tcpPort = atoi(argv[1]);

  tcp_addr.sin_family = AF_INET;
  tcp_addr.sin_port = htons(tcpPort);
  tcp_addr.sin_addr.s_addr = INADDR_ANY;
  if(bind(tcpSock, (struct sockaddr *) &tcp_addr, sizeof(tcp_addr)) < 0){
    cout << "ERROR: binding error\n";
    exit(0);
  }

  listen(tcpSock, 5);
  cout << "after listen\n";

  client = sizeof(cli_addr);
  while(clientSock = accept(tcpSock, (struct sockaddr *) &cli_addr, (socklen_t * )&client)){
    if(pthread_create(&thread_id, NULL, &connection, &clientSock) < 0){
      cout << "ERROR: creating thread\n";
      exit(0);
    }
  }
  pthread_join(thread_id, NULL);
  return 0;
}

void * connection(void * clientSock){
  int client = *(int*) clientSock;
  int n, indexL, indexU;
  char buffer[1024];
  string responce;
  if(client < 0){
    cout << "ERROR: accept error\n";
    exit(0);
  }
  bzero(buffer, 1024);
  n = write(client, "220 smtp.447f18.edu Mail Server Ready", 37);
  if(n < 0){
    cout << "ERROR: write to stream\n";
    exit(0);
  }
  bzero(buffer, 1024);
  n = read(client, buffer, 1024);
  //cout << buffer << endl;
  responce = buffer;
  //cout << responce << endl;
  indexL = responce.find("helo");
  indexU = responce.find("HELO");

  if(indexL != 0 && indexU != 0){
    n = write(client, "503 closing connection",1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  
  bzero(buffer, 1024);
  n = write(client, "250 Hello",1024);

  bzero(buffer, 1024);
  n = read(client, buffer, 1024); // read mail from
  //cout << buffer << endl;
  responce = buffer;
  //cout << responce << endl;
  indexL = responce.find("mail from");
  indexU = responce.find("MAIL FROM");
  if(indexL != 0 && indexU != 0){
    n = write(client, "503 closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  
  bzero(buffer, 1024);
  n= write(client, "250 OK", 1024);
}
