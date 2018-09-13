#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main(int argc, char *argv[]){
  if(argc != 3){
    cout << "ERROR: invalid paramaters.\n./server <TCP port> <UDP port>\n";
    exit(0);
  }
  
  cout << "Greetings Master\n";

  int tcpSock, tcpPort, udpPort, client, n;

  struct sockaddr_in tcp_addr, cli_addr;
  tcpSock = socket(AF_INET, SOCK_STREAM, 0);

  tcpPort = atoi(argv[1]);
  udpPort = atoi(argv[2]);

  tcp_addr.sin_family = AF_INET;
  tcp_addr.sin_port = htons(tcpPort);
  tcp_addr.sin_addr.s_addr = INADDR_ANY;
  if(bind(tcpSock, (struct sockaddr *) &tcp_addr, sizeof(tcp_addr)) < 0){
    cout << "ERROR: binding error\n";
  }

  listen(tcpSock, 5);
  cout << "after listen\n";
  return 0;
}