#pragma once

#include <netinet/in.h>

#include "server.h"
#ifndef INET_SERV
#define INET_SERV
class InetServer : public Server {

public:

	InetServer(int port) : Server(){
    // setup variables
    		port_ = port;
	}
    //InetServer(int);
    ~InetServer();
void create();
    void close_socket();
protected:
    virtual void handle(int)=0;
private:
    int port_;
};
#endif
