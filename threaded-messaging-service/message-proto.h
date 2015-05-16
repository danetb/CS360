#include "inet-client.h"
#include <sstream>

class MessageRequester : public InetClient {
	public:
	MessageRequester(string h, int p, bool d = false) : InetClient(h,p,d) {
		create();
	}
		bool msg_put(string name, string subject, string message) {
			ostringstream oss;
			oss << message.length();
			string mssg = "put " + name + " " + subject + " " + oss.str() + "\n" + message;
			return invoke_message(mssg);
		}
		bool msg_list(string name) {
			string mssg = "list " + name + "\n";
			return invoke_message(mssg);
		}
		bool msg_get(string name, int index) {
			ostringstream oss;
			oss << index;
			string mssg = "get " + name + " " + oss.str() + "\n";
			return invoke_message(mssg);	
		}
		bool msg_reset() {
			return invoke_message("reset\n");
		}
		bool invoke_message(string mssg) {
			if(send_request(mssg)) {
				if(get_response())
					return true;
			}
			return false;	
		}
};


