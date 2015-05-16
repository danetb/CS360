#include "xshell.h"
#include "message-proto.h"
#include <sstream>
class MessageUI: public XShell {
	public:

		MessageUI(MessageRequester*);
		string getLastResponse();
		void parseCommand();
	private:
		MessageRequester* requester;
		void printMessage(string);
		string get_user_message() {
			cout << "- Type your message. End with a blank line -" << endl;
			string message = "";
			string tmp;
			getline(cin, tmp);
			while(tmp != "") {
				message.append(tmp + "\n");
				getline(cin, tmp);
			}
			return message;
		}
		void rsp_output(char* rsp) {
			istringstream iss(rsp);
		}
		string parseError(string&);
		string parseSendResponse();
		string parseListResponse();
		string parseReadResponse();
		string parseResetResponse();

		bool parseSendRequest(istringstream& ss) {
			string name;
			string subject;
			string message;
			if(!(ss >> name)) return false;
			if(!(ss >> subject)) return false;	
			message = get_user_message();
			if(!requester->msg_put(name,subject,message)) noResponseAction();
			return true;
			//	if(dbg) cout << rsp;
			//			cout << rsp << endl;
			//	format_response_send(rsp);
			//return true;
		}
		bool parseListRequest(istringstream& ss) {
			string name;
			if(!(ss >> name)) return false;
			if(!requester->msg_list(name)) noResponseAction();
			return true;
			//			cout << rsp << endl;

			//return true;
		}
		bool parseReadRequest(istringstream& ss) {
			string name;
			int index;
			if(!(ss >> name)) return false;
			if(!(ss >> index)) return false;

			if(!requester->msg_get(name,index)) noResponseAction();
			return true;

			//			cout << rsp << endl;

			//return true;
		}
		void parseQuit() {
			exit(0);
		}
		bool parseResetRequest() {
			if(!requester->msg_reset()) noResponseAction();
			return true;
		}
		void noResponseAction() {
			//No response obtained due to socket-level error, severred connection, etc...
			//...
			exit(0);
		}
		//InetClient* inet_client;
};
MessageUI::MessageUI(MessageRequester* mr) : XShell() {
	requester = mr;
}
string MessageUI::getLastResponse() {
	return requester->getLastResponse();
}
void MessageUI::printMessage(string msg) {
	cout << msg << endl;
}
void MessageUI::parseCommand() {
	string line;
	getline(cin, line);
	if(line.empty()) return;
	istringstream ss(line);
	string command;
	ss >> command;
	string user;
	string subject;
	string message;
	int index;
	if(!command.compare("send")) {
		if(parseSendRequest(ss)) { 
			cout << parseSendResponse();
		}	
	} else if(!command.compare("list")) {
		if(parseListRequest(ss)) { 
			cout << parseListResponse();
		}
	} else if(!command.compare("read")) {
		if(parseReadRequest(ss)) { 
			cout << parseReadResponse();
		}	
	} else if(!command.compare("quit")) {
		exit(0);
	} else if(!command.compare("reset")) {
		if(parseResetRequest()) {
			cout << parseResetResponse();	
		}
	} else {
		cout << "I don't recognize that command." << endl;
	}
}
#define INVALID_RESPONSE_STR "Invalid response received."
string MessageUI::parseError(string& str) {
	return "Server problem: " + str;
}
string MessageUI::parseSendResponse() {
	string response = getLastResponse();
	istringstream iss(response);
	string m;
	iss >> m;
	if(m == "OK") {
		return "";
	} else if(m == "error") return parseError(response);
	return INVALID_RESPONSE_STR;
}
string MessageUI::parseListResponse() {
	string response = getLastResponse();
	string line1 = response.substr(0, response.find("\n"));
	istringstream iss1(line1);
	string test;
	int tstI;
	iss1 >> test;
	if(test == "list");
	else if(test == "error") return parseError(response);
	else return INVALID_RESPONSE_STR;
	if(!(iss1 >> tstI)) return INVALID_RESPONSE_STR;
	string body = response.erase(0, response.find("\n") + 1);
	istringstream issB(body);
	string indxSubjPair;
	int recCount = 1;
	while(getline(issB,indxSubjPair)) {
		istringstream issPairDiv(indxSubjPair);
		int indx;
		string sbj;
		if(!(issPairDiv >> indx)) return INVALID_RESPONSE_STR;
		if(!(issPairDiv >> sbj)) return INVALID_RESPONSE_STR;
		recCount++;
	}
	if((recCount-1) != tstI) return INVALID_RESPONSE_STR;	
	return body; //To be printed out.	
}
string MessageUI::parseReadResponse() {
	string response = getLastResponse();
	string line1 = response.substr(0, response.find("\n"));
	istringstream iss1(line1);
	string subj;  //Don't take this naming too seriously.
	int msgLength;
	iss1 >> subj;
	if(subj == "message"); 
	else if(subj == "error") return parseError(response);
	else return INVALID_RESPONSE_STR;
	if(!(iss1 >> subj)) return INVALID_RESPONSE_STR; //test for [subject]
	if(!(iss1 >> msgLength)) return INVALID_RESPONSE_STR; //test for [length]
	//We -could- verify the message length at this point. But such behavior is not specified for the client.
	string msgBody = response.erase(0, response.find("\n") + 1);
	return subj + "\n" + msgBody;

}
string MessageUI::parseResetResponse() {
	string response = getLastResponse();
	string line1 = response.substr(0, response.find("\n"));
	istringstream iss1(line1);
	string test;
	iss1 >> test;
	if(test == "OK") return "";
	else if(test == "error") return parseError(response);
	else return INVALID_RESPONSE_STR;
}




