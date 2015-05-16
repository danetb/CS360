#include "message-model.h"
#include <sstream>
#include <semaphore.h>
#ifndef MRQ_HANDLER
#define MRQ_HANDLER

class MessageRequestHandler {
	private:
                sem_t s;
//		MessageModel* model;
	public:
		MessageRequestHandler();
		string handle_put(string,string,int,string,MessageModel*);
		string handle_list(string,MessageModel*);
		string handle_get(string,int,MessageModel*);
		string handle_reset(MessageModel*);
};
MessageRequestHandler::MessageRequestHandler(/*MessageModel* mm_p*/) {
//	this->model = mm_p;
        sem_init(&s,0,1);
}
string MessageRequestHandler::handle_put(string name, string subject, int length, string message, MessageModel* model) {
    sem_wait(&s);
    if(model->model_put(name,subject,length,message)) {
		sem_post(&s);
                return "OK\n";
    }
                sem_post(&s);
                return "error unable to satisfy put request\n";
             	
}
string MessageRequestHandler::handle_list(string name, MessageModel* model) {
        sem_wait(&s);
        string response = model->model_list(name);
        sem_post(&s);
        return "list " + response;	
}
string MessageRequestHandler::handle_get(string name, int index, MessageModel* model) {
        sem_wait(&s);
        Message* m_ptr = model->model_get(name,index);
	if(m_ptr == NULL) {
           sem_post(&s); 
            return "error no such message for that user\n";
        }
	ostringstream oss;
	int msgSize = m_ptr->getLength();
	string returnedMsgBody = m_ptr->getMessageBody();
	oss << msgSize;
	string response = "message " + m_ptr->getSubject() + " " + oss.str() + "\n" + returnedMsgBody;
        sem_post(&s);
        return response;
}
string MessageRequestHandler::handle_reset(MessageModel* model) {
	//if(model == NULL) return "error reset failure: model is NULL\n";
        //delete model;
	//model = new MessageModel();
        sem_wait(&s);
	model->model_reset();
        sem_post(&s);
       // if(model == NULL) return "error reset failure: model constructor failed\n";
	return "OK\n";
}
#endif


