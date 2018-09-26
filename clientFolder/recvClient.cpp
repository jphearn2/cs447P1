#include <cstdlib>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

using namespace std;

int main(int argc, char* argv[]){
    int sockID, port, n;
    socklen_t len;
    char buffer[1024];
    struct sockaddr_in servaddr;
    if(argc < 3){
        cout << "ERROR: invalid paramaters.\n./recvClient <IP address> <UDP port>\n";
        exit(0);
    }
    cout << "Greetings Master\n";

    if((sockID = socket(AF_INET, SOCK_DGRAM, 0) < 0)){
        cout << "ERROR: creating socket\n";
        exit(0);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    bzero(buffer, 1024);
    fgets(buffer, 1024, stdin);
    sendto(sockID, buffer, 1024, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
    return 0;
}