#include <vector>
#include <string>
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
			if(!subject.empty() && length > 0) {
				messages.push_back(Message(subject,length,message));
			}
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
	bool model_put(string,string,int,string);
	string model_list(string);
	Message* model_get(string,int);
	MessageUser* getUserByName(string);
private:
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
	if(users.empty()) {
		users.push_back(MessageUser(name,subject,length,message));
		return true;
	}	
	std::vector<MessageUser>::iterator it = users.begin();
	int compVal;// = name.compare((*it).getName());
	// else {
	//	it++;	
		for(it; it != users.end(); it++) {
			compVal = name.compare((*it).getName());
			if(compVal < 0) {	//User doesn't exist. Add user and store content.
				users.insert(it, MessageUser(name,subject,length,message));
			//	cout << "New User Added:" << users.size() << endl;
				return true;
			} else if(compVal == 0) {	//User exists. Add to their list of messages.
			//	cout << "New Message Added to User:" << users.size() << endl;
				(*it).addMessage(Message(subject,length,message));
				return true;
			}
		}
		cout << "New User Added(END):" << users.size() << endl;
		users.push_back(MessageUser(name,subject,length,message));
		return true;
	//}
	
}

string MessageModel::model_list(string name) { //Message headings of user.
	MessageUser* mu = getUserByName(name);
	if(mu == NULL) return "0\n";
	return mu->formList();
//	std::vector<MessageUser>::iterator it = users.begin();
//	for(it; it != users.end(); it++) {
//		if(name.compare((*it).getName()) == 0) return (*it).formList();
//	}
}

Message* MessageModel::model_get(string name,int index) {
	index--;  //corrected for usage. (e.g. 1 => 0)
	std::vector<MessageUser>::iterator it = users.begin();
	for(it; it != users.end(); it++) {
		if((*it).getName() == name) {
			return &((*it).get_msg_vector_ref())->at(index);
		}
	}
	return NULL;
}
#endif
