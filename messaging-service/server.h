#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

using namespace std;
#ifndef SERV_
#define SERV_
class Server {
public:
    Server();
    ~Server();

    void run();
    void serve();
    
protected:
   string get_request(int);
    bool send_response(int, string);
	 virtual void create();
    virtual void close_socket();
    virtual void handle(int) =0;
    

    int server_;
    int buflen_;
    char* buf_;
};
#endif
