#include "server.h"
#include <iostream>
using namespace std;
Server::Server() {
    // setup variables
    //'buflen_ = 1024;
    //buf_ = new char[buflen_+1];
}

Server::~Server() {
    //delete buf_;
}

void Server::run() {
    // create and run the server
    create();
    serve();
}

//void
//Server::create() {
//}

void
Server::close_socket() {
}

void Server::serve() {
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

      // accept clients
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
        handle(client);

     }
cout << "SOCK GOT CLOSED^^^^" << endl;	
    close_socket();
}


