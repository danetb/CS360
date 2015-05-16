#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
//#include "inet-server.h"

#include "message-model.h"
#include "message-serve.h"
#include "message-handler.h"



string msg_put(string name, string subject, string message) {
	ostringstream oss;
	oss << message.length();
	string mssg = "put " + name + " " + subject + " " + oss.str() + "\n" + message;
	//	if(invoke_message(mssg)) return buf_;
	return mssg;
}
string msg_list(string name) {
	string mssg = "list " + name + "\n";
	//if(invoke_message(mssg)) return buf_;
	return mssg;
}
string msg_get(string name, int index) {
	ostringstream oss;
	oss << index;
	string mssg = "get " + name + " " + oss.str() + "\n";
	return mssg;
	//	if(invoke_message(mssg)) return buf_; 
	//	return "";
}





using namespace std;

int main(int argc, char **argv)
{
	int option, port;

	// setup default arguments
	port = 3000;
	bool debug = false;

	// process command line options using getopt()
	// see "man 3 getopt"
	while ((option = getopt(argc,argv,"p:d")) != -1) {
		switch (option) {
			case 'p':
				port = atoi(optarg);
				break;    
			case 'd':
				debug = true;
				break;
			default:
				cout << "server [-p port]" << endl;
				exit(EXIT_FAILURE);
		}
	}
	MessageModel* mmod = new MessageModel();

	MessageRequestHandler* msg_r = new MessageRequestHandler(mmod);
	MessageAutoServe msg_srv = MessageAutoServe(msg_r,port,debug);
	msg_srv.run();
	//MessageModel mm = MessageModel();
	/*	string name;
		string subj;
		int len;
		string msssg;
		string divid;
	 */	
	/*	string divid;
		string pickout;
		string nme;
		string subj;
		int len;
		string message;
		while(true) {
		getline(cin,pickout);
		istringstream iss((pickout));
		iss >> nme;
		iss >> subj;
	//iss >> len;
	getline(iss, message);
	//mmod->model_put(nme,subj,62,message);
	divid =	msg_put(nme,subj,message);
	cout << "Put_reQuest:\n" << divid << endl;

	string respn = msg_srv.parseRequest(divid);
	cout <<"Put_RESPONSE:\n" << respn << endl;
	//	msg_srv.parseRequest(divid);
	divid = msg_list(nme);
	cout << "List_reQuest:\n" << divid << endl;
	respn = msg_srv.parseRequest(divid);
	cout <<"List_RESPONSE:\n" << respn << endl;
	istringstream ibbs(respn);
	ibbs >> message;
	ibbs >> len;
	divid = msg_get(nme, len);
	cout << "Get_reQUEST:\n" << divid << endl;
	respn = msg_srv.parseRequest(divid);
	cout << "parseSucceede\n";
	cout << "Get_RESPONSE:\n" << respn << endl;
	}
	 */
}

