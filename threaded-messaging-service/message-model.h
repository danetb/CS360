#include <vector>
#include <string>
//#include <semaphore.h>
using namespace std;

#ifndef MSG_MODEL
#define MSG_MODEL
class Message {
	public:
		string getSubject() {
			return subject;
		}
		int getLength() {
			return length;
		}
		string getMessageBody() {
			return message;
		}
		Message() {}
		Message(string subject, int length, string message) {
			this->subject = subject;
			this->length = length;
			this->message = message;
		}
	protected:
		string subject;
		int length;
		string message;
};

class MessageUser {
	public:
		MessageUser(string name, string subject="", int length=0, string message = "") {
			this->name = name;
			messages.push_back(Message(subject,length,message));
		}
		void addMessage(Message);
		string getName() {
			return name;
		}
		vector<Message>* get_msg_vector_ref() {
			return &messages;
		}
        string formList();
	private:

		string name;
		vector<Message> messages;
};	
void MessageUser::addMessage(Message m) {
	messages.push_back(m);
}

string MessageUser::formList() {
	ostringstream oss;
	oss << messages.size();
	string response = oss.str() + "\n";
	oss.str("");
	for(int i = 0; i < messages.size(); i++) {
		oss << (i+1);
		response.append(oss.str() + " " + messages.at(i).getSubject() + "\n");
		oss.str("");
	}
	return response;
}
class MessageModel {
	public:
	        MessageModel() {
  //                  sem_init(&s,0,1);
                 //    sem_init(&n,0,1);    
                 }
  //              ~MessageModel() {
//                    cout << "Model destroyed!!!!!" << endl;
    //            }
                bool model_put(string,string,int,string);
		string model_list(string);
		Message* model_get(string,int);
                void model_reset();
		MessageUser* getUserByName(string);
	private:
    //            sem_t s;
               // sem_t n;
		vector<MessageUser> users;

};
MessageUser* MessageModel::getUserByName(string name) {
	std::vector<MessageUser>::iterator it = users.begin();
	for(it; it != users.end(); it++) {
		if(name.compare((*it).getName()) == 0) {
			return &(*it);
		}
	}
	return NULL;
}

bool MessageModel::model_put(string name, string subject, int length, string message) {
        //sem_wait(&n);
       // sem_wait(&s);

        if(users.empty()) {
		users.push_back(MessageUser(name,subject,length,message));
          //     sem_post(&s);
//                sem_post(&n);
                return true;
	}	
	std::vector<MessageUser>::iterator it = users.begin();
	int compVal;// = name.compare((*it).getName());
	for(it; it != users.end(); it++) {
		compVal = name.compare((*it).getName());
		if(compVal < 0) {	//User doesn't exist. Add user and store content.
			users.insert(it, MessageUser(name,subject,length,message));
	//	        sem_post(&s);
  //                      sem_post(&n);
                        return true;
		} else if(compVal == 0) {	//User exists. Add to their list of messages.
			(*it).addMessage(Message(subject,length,message));
	//		sem_post(&s);
    //                    sem_post(&n);
                        return true;
		}
	}
	users.push_back(MessageUser(name,subject,length,message));
      //  sem_post(&s);
      //  sem_post(&n);
	return true;
}

string MessageModel::model_list(string name) { //Message headings of user.
    //sem_wait(&s);
    MessageUser* mu = getUserByName(name);
	if(mu == NULL) {
            //sem_post(&s);
            return "0\n";
	}
       string fl = mu->formList(); 
    //sem_post(&s);
        return fl;
}

Message* MessageModel::model_get(string name,int index) {
	//sem_wait(&s);
        index--;  //corrected for usage. (e.g. 1 => 0)
	std::vector<MessageUser>::iterator it = users.begin();
	for(it; it != users.end(); it++) {
		if((*it).getName() == name) {
                        return &((*it).get_msg_vector_ref())->at(index);     	                


        //                sem_post(&s);
          //              return result
		}
	}

    //sem_post(&s);
	return NULL;
}
void MessageModel::model_reset() {
  //sem_wait(&s);
    users.clear();
    //sem_post(&s);
}
#endif
