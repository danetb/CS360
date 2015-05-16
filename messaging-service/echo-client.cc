#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include "messageUI.h"
using namespace std;

int main(int argc, char **argv)
{
    int option;
    bool debug = false;
    // setup default arguments
    int port = 3000;
    string host = "localhost";

    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc,argv,"s:p:d")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                host = optarg;
                break;
	    case 'd':
		debug = true;
		break;
            default:
                cout << "client [-s host] [-p port] [-d debug]" << endl;
                exit(EXIT_FAILURE);
        }
    }
//    InetClient client = InetClient(host, port);
  //  client.create();
	cout << "Will connect to port: " << port << " at host: " << host << endl; 
	MessageRequester* mr = new MessageRequester(host, port, debug);
	
	MessageUI* ui = new MessageUI(mr);
	ui->begin();
	//cout << "invoking..." << endl;
	//	mr->invoke_message(" asdjisfdaijofdsioijo  oifdjsisdo fiosd f  faosidj");
	return 0;
}

