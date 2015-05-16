#include "message-model.h"
#include <sstream>


#ifndef MRQ_HANDLER
#define MRQ_HANDLER
class MessageRequestHandler {
	private:
		MessageModel* model;
	public:
		MessageRequestHandler(MessageModel*);
		string handle_put(string,string,int,string);
		string handle_list(string);
		string handle_get(string,int);
		string handle_reset();
};
MessageRequestHandler::MessageRequestHandler(MessageModel* mm_p) {
	this->model = mm_p;
}
string MessageRequestHandler::handle_put(string name, string subject, int length, string message) {
	if(model->model_put(name,subject,length,message))
		return "OK\n";
	else return "error unable to satisfy put request\n"; 	
}
string MessageRequestHandler::handle_list(string name) {
	string response = model->model_list(name);
	/*vector<Message>* vmsg_p = model->model_list(name);
	ostringstream oss;
	oss << vmsg_p->size();
	string response = "list " + oss.str() + "\n";
	for(int i = 0; i < vmsg_p->size(); i++) {
		cout << "listElem88888" << endl;
		oss << i;
		response.append(oss.str() + " " + vmsg_p->at(i).getSubject()) + "\n";
	}*/
	return "list " + response;	
}
string MessageRequestHandler::handle_get(string name, int index) {
	Message* m_ptr = model->model_get(name,index);
	if(m_ptr == NULL) return "error no such message for that user\n";
	ostringstream oss;
	//oss << m_ptr->getLength();
//	int msgSize = m_ptr->getMessageBody().length(); 
	int msgSize = m_ptr->getLength();
	string returnedMsgBody = m_ptr->getMessageBody();
/*
	string returnedMsgBody;
	if(msgSz < 1) {
		returnedMsgBody = "\n";
		msgSz = 1;
		//return "error message body is empty\n";
	} else {
		returnedMsgBody = m_ptr->getMessageBody();
	}
*/
	oss << msgSize;
	string response = "message " + m_ptr->getSubject() + " " + oss.str() + "\n" + returnedMsgBody;
	cout << "mptr->getMessageBody->length: " << m_ptr->getMessageBody().length() << endl;
	cout << "mptr->getLength: " << m_ptr->getLength() << endl;
	return response;
}
string MessageRequestHandler::handle_reset() {
	if(model == NULL) return "error reset failure: model is NULL\n";
	delete model;
	model = new MessageModel();
	if(model == NULL) return "error reset failure: model constructor failed\n";
	return "OK\n";
}
#endif


