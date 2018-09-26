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
#include <regex>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include <sstream>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]){
    int sockID, port, n;
    socklen_t len;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;
    if(argc < 2){
        cout << "ERROR: invalid paramaters.\n./recvServer <UDP port>\n";
        exit(0);
    }
    cout << "Greetings Master\n";
    port = atoi(argv[1]);
    if((sockID = socket(AF_INET, SOCK_DGRAM, 0) < 0)){
        cout << "ERROR: creating socket\n";
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);


    if(bind(sockID, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        cout << "ERROR: binding\n";
        exit(0);
    }

    while(true){
        n = recvfrom(sockID, buffer, 1024, 0, (struct sockaddr *) &cliaddr, &len);
        cout << buffer;
    }
    return 0;
}