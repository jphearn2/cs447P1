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

static void * connection(void * clientSock);
static int parseCode(char buffer[], string * args);
static bool validEmail(string * email);
static void sendEmail(struct email e);

typedef struct email{
  string from, to, email;
}email;

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
  //responce.erase(responce.size()-2);
  //cout << "test";
  indexL = responce.find("helo");
  indexU = responce.find("HELO");
  if(indexL == 0 || indexU == 0){
    *args = responce.substr(5);
    return 1;
  }
  
  indexL = responce.find("mail from");
  indexU = responce.find("MAIL FROM");
  if(indexL == 0 || indexU == 0){
    *args = responce.substr(10);
    return 2;
  }

  indexL = responce.find("rcpt to");
  indexU = responce.find("RCPT TO");
  if(indexL == 0 || indexU == 0){
    *args = responce.substr(8);
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

bool validEmail(string * email){
  regex r("<[a-zA-Z0-9]+@([a-zA-Z0-9]+\\.)+[a-zA-Z0-9]+>\n");
  //cout << *email << endl;
  if(regex_match(*email, r)){
    return true;
  }

  return false;
}

void * connection2(void * clientSock){
  int i = 0;
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
  email e;
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
  if(n < 0){
    cout << "ERROR: read to stream\n";
    exit(0);
  }
  //cout << buffer << endl;
  cmd = parse(buffer, &args);
  //cout << "";
  if(!validEmail(&args)){
    n = write(client, "555 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  e.from = args;
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
  n = read(client, buffer, 1024); // read rcpt to
  //cout << buffer << endl;
  cmd = parse(buffer, &args);
  //cout << cmd << endl;
  if(!validEmail(&args)){
    n = write(client, "555 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  e.to = args;
  if(cmd == -1){
    n = write(client, "500 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  else if(cmd == 3){
    n= write(client, "250 OK", 1024);
  }
  else{
    n = write(client, "503 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }

  bzero(buffer, 1024);
  n = read(client, buffer, 1024); // read Data
  //cout << buffer << endl;
  cmd = parse(buffer, &args);
  //cout << cmd << endl;
  if(cmd == -1){
    n = write(client, "500 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }
  else if(cmd == 4){
    n= write(client, "354 ", 1024);
  }
  else{
    n = write(client, "503 Closing connection", 1024);
    shutdown(client, SHUT_RDWR);
    return 0;
  }

  while(i < 2){
    bzero(buffer, 1024);
    n = read(client, buffer, 1024);
    if(buffer[0] == '\n'){
      i++;
    }
    else{
      i =0;
    }
    e.email += buffer;
    cout << buffer;
  }
  sendEmail(e);
  
  return 0;
}

void * connection(void * clientSock){
  int client = *(int*) clientSock;
  int n, cmd, order = 1;
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
  email e;
  while(cmd != 0){
    //cout << "start\n";
    if(order == cmd){
      switch(cmd){
        case 1:
        //cout << "helo";
          msg = "250 Hello " + args;
          n = write(client, msg.c_str(),1024);
          order++;
          break;
        case 2:
          //cout << args << endl;
          //cout << "mail\n";
          if(!validEmail(&args)){
            n = write(client, "555 Invalid email", 1024);
            break;
          }
          //cout << args << endl;
          n= write(client, "250 OK", 1024);
          e.from = args;
          //cout << e.from;
          order++;
          break;
        case 3:
          //cout << "rcpt\n";
          if(!validEmail(&args)){
            n = write(client, "555 Invalid email", 1024);
            break;
          }
          n= write(client, "250 OK", 1024);
          e.to = args;
          order++;
          break;
        case 4:
          cout << "data\n";
          n= write(client, "354 Send message content; end with <CRLF><CRLF>", 1024);
          int i = 0;
          while(i < 2){
            //cout << "read\n";
            bzero(buffer, 1024);
            n = read(client, buffer, 1024);
            if(buffer[0] == '\n'){
              i++;
            }
            else{
              i =0;
            }
            e.email += buffer;
            if(i != 2){
              n= write(client, "", 1024);
            }
            cout << buffer;
          }
          //cout << e.to << endl;
          sendEmail(e);
          e.to = "";
          e.from = "";
          e.email = "";
          order = 1;
          n = write(client, "250 OK, message accepted for delivary", 1024);
      }
    }
    else if(cmd != -1){
      n = write(client, "503 Wrong ordered command", 1024);
    }
    else{
      n = write(client, "500 Nonrecognized command", 1024);
    }
    bzero(buffer, 1024);
    n = read(client, buffer, 1024); 
    cmd = parse(buffer, &args);
  }
  n= write(client, "221 Bye", 1024);
  return 0;
}

static void sendEmail(struct email e){
  regex r1("<|>|@([a-zA-Z0-9]+\\.)+[a-zA-Z0-9]+>\n");
  string dir = "db/" + regex_replace(e.to, r1,"");
  stringstream filename;
  fstream fs;
  DIR * dr;
  time_t currTime = time(NULL);
  int count = 0;
  struct dirent *file;
  mkdir("db", ACCESSPERMS);
  if(mkdir(dir.c_str(), ACCESSPERMS) == -1){
    cout << "didn't make dir\n";
  }
  dr = opendir(dir.c_str());
  while(file = readdir(dr)){
    //cout << "test\n";
    count++;
  }
  closedir(dr);
  count--;
  filename << count;
  //cout << filename.str() << endl;
  filename << ".email";
  //cout << filename.str() << endl;
  fs.open(dir + "/" + filename.str(), fstream::out);
  fs << "Date:" << ctime(&currTime);
  fs << "From: " << e.from;
  fs << "To: " << e.to << endl;
  fs << e.email;

  fs.close();
  
}
