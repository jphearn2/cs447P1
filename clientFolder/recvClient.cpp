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
    string get, host, num, req;
    if(argc < 3){
        cout << "ERROR: invalid paramaters.\n./recvClient <IP address> <UDP port>\n";
        exit(0);
    }
    cout << "Greetings Master\n";
    port = atoi(argv[2]);
    sockID = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockID < 0){
        cout << "ERROR:creating socket\n";
        exit(0);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <=0){
        cout << "ERROR: invalid server address\n";
        exit(0);
    }

    bzero(buffer, 1024);
    getline(cin, get);
    getline(cin, host);
    getline(cin, num);
    req = get + '\n' + host + '\n' + num +'\n';
    strcpy(buffer,req.c_str());
    //fgets(buffer, 1024, stdin);
    cout << buffer;
    sendto(sockID, buffer, 1024, MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));
    cout << buffer;

    close(sockID);
    return 0;
}