#pragma once

#include <netinet/in.h>

#include "client.h"

class InetClient : public Client {

public:
    
protected:
    InetClient(string, int, bool);
    ~InetClient();

    void create();
    void close_socket();

private:
    string host_;
    int port_;
};
