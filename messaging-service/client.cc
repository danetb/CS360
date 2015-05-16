#include "client.h"
#include <sstream>
Client::Client(bool debug = false) {
	// setup variables
	buflen_ = 1024;
	buf_ = new char[buflen_+1];
	this->debug_flag = debug;
}


Client::~Client() {
}

void
Client::create() {
}

void Client::close_socket() {
}
string Client::getLastResponse() {
	return response;
}
bool Client::send_request(string request) {
	// prepare to send request
	const char* ptr = request.c_str();
	int nleft = request.length();
	int nwritten;
	// loop to be sure it is all sent
	while (nleft) {
		if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
			if (errno == EINTR) {
				// the socket call was interrupted -- try again
				continue;
			} else {
				// an error occurred, so break out
				cout << "Socket write error." << endl;	
				//perror("write");
				return false;
			}
		} else if (nwritten == 0) {
			// the socket is closed
			cout << "Socket is closed." << endl;
			return false;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	if(debug_flag) cout << request << endl;
	return true;
//	return sock_state.success;
}

bool Client::get_response() {
	string response = "";
	// read until we get a newline
	while (response.find("\n") == string::npos) {
		int nread = recv(server_,buf_,1024,0);
		if (nread < 0) {
			if (errno == EINTR)
				// the socket call was interrupted -- try again
				continue;
			else {
				cout << "Socket read error." << endl;
				// an error occurred, so break out
				return false;// sock_state.SOCK_ERROR;
			}
		} else if (nread == 0) {
			// the socket is closed
		//	return sock_state.REMOTE_SOCKET_CLOSED;
			return false;
		}
		// be sure to use append in case we have binary data
		response.append(buf_,nread);
		this->response = response;
		if(debug_flag) cout << response << endl;
	}
	// a better client would cut off anything after the newline and
	// save it in a cache
	//cout << response;
	return true;//sock_state.SUCCESS;
}
