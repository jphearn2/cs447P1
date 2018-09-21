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
static int parseCode(char buffer[], string * args);

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

static int parse(char buffer[], string * args){
  int indexL, indexU;
  string responce = buffer;
  indexL = responce.find("helo");
  indexU = responce.find("HELO");
  if(indexL == 0 || indexU == 0){
    *args = responce.substr(5);
    return 1;
  }

  indexL = responce.find("mail from");
  indexU = responce.find("MAIL FROM");
  if(indexL == 0 || indexU == 0){
    return 2;
  }

  indexL = responce.find("rcpt to");
  indexU = responce.find("RCPT TO");
  if(indexL == 0 || indexU == 0){
    return 3;
  }

  indexL = responce.find("data");
  indexU = responce.find("DATA");
  if(indexL == 0 || indexU == 0){
    return 4;
  }

  indexL = responce.find("quit");
  indexU = responce.find("QUIT");
  if(indexL == 0 || indexU == 0){
    return 0;
  }

  indexL = responce.find("\n");
  if(indexL == 0){
    return 5;
  }
  
  return -1;
}

void * connection(void * clientSock){
  int client = *(int*) clientSock;
  int n, cmd;
  char buffer[1024];
  string args, msg;
  
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
  cmd = parse(buffer, &args);
  if(cmd == -1){
    n = write(client, "500 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  else if(cmd == 1){
    msg = "250 Hello " + args;
    n = write(client, msg.c_str(),1024);
  }
  else{
    n = write(client, "503 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  
  bzero(buffer, 1024);
  n = read(client, buffer, 1024); // read mail from
  cmd = parse(buffer, &args);
  if(cmd == -1){
    n = write(client, "500 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  else if(cmd == 2){
    n= write(client, "250 OK", 1024);
  }
  else{
    n = write(client, "503 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  
  bzero(buffer, 1024);
  
}
