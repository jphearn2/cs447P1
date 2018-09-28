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

int parser(string * dbfolder, string * folder, char buffer[], int * countN);
void recvEmail(string dbfolder, string folder, int count);

int main(int argc, char* argv[]){
    int sockID, port, n, count;
    socklen_t len;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;
    string dbfolder, folder;
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
        int ret;
        bzero(buffer, 1024);
        recvfrom(sockID, buffer, 1024, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
        //cout << n << endl;
        cout << buffer;

        ret = parser(&dbfolder, &folder, buffer, &count);
        if(ret == -1){
            sendto(sockID, "400 bad request\n", 1024, MSG_CONFIRM, (struct sockaddr *) &cliaddr, len);
        }
        else{
            recvEmail(dbfolder, folder, count);
            //cout << "test\n";
            sendto(sockID, "HTTP/1.1 200 OK\n", 1024, MSG_CONFIRM, (struct sockaddr *) &cliaddr, len);
        }
    }
    
    close(sockID);
    return 0;
}

void recvEmail(string dbfolder, string folder, int count){
    DIR * dr;
    int fileCount = 0;
    struct dirent *file;
    stringstream filename;
    mkdir(folder.c_str(), ACCESSPERMS);

    dr = opendir(dbfolder.c_str());
    while(file = readdir(dr)){
        fileCount++;
    }
    fileCount--;
    closedir(dr);
    for(int i = 0; i < count; i++){
        filename.str("");
        
    }

    //cout << "";
    return;
}

int parser(string * dbfolder, string * folder, char buffer[], int * countN){
    string get, host, count;
    regex r1("GET [\\/a-zA-Z0_9]{2,}\\/ HTTP 1.1|get [\\/a-zA-Z0_9]{2,}\\/ http 1.1");
    regex r2("HOST: [0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}|host: [0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}|");
    regex r3("COUNT: [0-9]+|count: [0-9]+");
    regex r4("GET /| HTTP 1.1|get /| http 1.1");
    regex r5("Count: |count: ");
    get = strtok(buffer, "\n");
    host = strtok(NULL, "\n");
    count = strtok(NULL, "\n");
    if(!regex_match(get, r1) || !regex_match(host, r2) || !regex_match(count, r3)){
        return -1;
    }
    *countN = atoi((regex_replace(count, r5, "")).c_str());

    *dbfolder = regex_replace(get, r4, "");
    *folder = (*dbfolder).substr(3);

    //cout << *dbfolder << " " << *folder <<endl;

    //cout << get << host << count << endl;


    return 0;
}   