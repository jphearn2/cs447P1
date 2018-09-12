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

  int sockid, tcpPort, udpPort, client, n;

  struct sockaddr_in serv_addr, cli_addr;
  sockid = socket(AF_INET, SOCK_STREAM, 0);

  

  return 0;
}