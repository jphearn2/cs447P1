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
    string filename;
    if(argc < 2){
        cout << "ERROR: invalid paramaters.\n./recvServer <UDP port>\n";
        exit(0);
    }
    cout << "Greetings Master\n";
    port = atoi(argv[1]);

    sockID = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockID < 0){
        cout << "ERROR: creating socket\n";
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);


    if(bind(sockID, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        cout << "ERROR: binding\n";
        exit(0);
    }
    
    while(true){
        bzero(buffer, 1024);
        recvfrom(sockID, buffer, 1024, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
        //cout << n << endl;
        cout << buffer;
    }
    

    close(sockID);
    return 0;
}