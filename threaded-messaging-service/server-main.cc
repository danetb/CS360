#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
//#include "inet-server.h"
#include "message-model.h"
#include "message-serve.h"
#include "message-handler.h"
#include "thread-msg-srv.h"

using namespace std;

int main(int argc, char **argv)
{
	int option;
	// setup default arguments
	int port = 3000;
        int threads = 10;
	bool debug = false;
	// process command line options using getopt()
	// see "man 3 getopt"
	while ((option = getopt(argc,argv,"p:t:d")) != -1) {
		switch (option) {
			case 'p':
				port = atoi(optarg);
				break;   
                        case 't':
                                threads = atoi(optarg);
                                break;
			case 'd':
				debug = true;
				break;
			default:
				cout << "server [-p port] [-t threads] [-d debug:true]" << endl;
				exit(EXIT_FAILURE);
		}
	}
//	MessageModel* mmod = new MessageModel();

	//MessageRequestHandler* msg_r = new MessageRequestHandler(mmod);
       // MessageRequestHandler* msg_r = new MessageRequestHandler();
        ThreadedMessageServer* tMsg_Srv = new ThreadedMessageServer(port,debug);
//	MessageAutoServe msg_srv = MessageAutoServe(msg_r,port,debug);
	tMsg_Srv->init_thread_pool(threads);
        tMsg_Srv->run();
	//cout << "hello?" << endl;
        return 0;	
}

