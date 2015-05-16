#include "message-handler.h"
#include "inet-server.h"
#ifndef MSG_AUTOSRV
#define MSG_AUTOSRV
#define INVALID_REQUEST "error request bad format\n"





class MessageAutoServe : public InetServer {
	private:
		class Request {
			//friend MessageAutoServe::MessageAutoServe(MessageRequestHandler*,int
			//		,bool);
			public:
				Request(MessageRequestHandler* rh_p) {
					this->rh_p = rh_p;
				}
				//string parseMethod(istringstream& iss,string& reqst) {return "sdfoijfsdjio";}
				//string requestName() {return "fuck";}
				virtual string parseMethod(istringstream& iss,string& reqst) =0;// { return "OK\n";}
				virtual string requestName() =0;// {return "request";}
			protected:
				MessageRequestHandler* rh_p;		

				/*
				   protected:
				   MessageRequestHandler* getRequestHandler() {
				   return rh_p;
				   }	
				   private:
				   static MessageRequestHandler* rh_p;
				   */
		};

		//	MessageRequestHandler* Request::rh_p = NULL;

		class PutRequest : public Request {
			public:
			//	PutRequest(MessageRequestHandler*);
			PutRequest(MessageRequestHandler* rh) : Request(rh) {}	
				string requestName() {
					return "put";
				}
				string parseMethod(istringstream& ss, string& reqst) {
					string name;
					string subject;
					int length;
							
					if(!(ss >> name)) return INVALID_REQUEST;
					if(!(ss >> subject)) return INVALID_REQUEST;	
					if(!(ss >> length)) return INVALID_REQUEST;
					string message = reqst.erase(0, reqst.find("\n") + 1);
				//	if(message.empty()) {
						//As far as we're concerned, empty message are permissible.
				//		message = "\n";
						//	return "";	
				//	}
					cout << "put messsage length: " << message.length() << endl;
					cout << "put length Variable: " << length << endl;
					 
					return rh_p->handle_put(name,subject,length,message);
				}

		};
		//PutRequest::PutRequest(MessageRequestHandler* rh) : Request(rh) {}
		class ListRequest : public Request {
			public:
				ListRequest(MessageRequestHandler* rh) : Request(rh) {}
				string requestName() {
					return "list";
				}
				string parseMethod(istringstream& ss, string& reqst) {
					string name;
					if(!(ss >> name)) return INVALID_REQUEST;
					return rh_p->handle_list(name);

				}

		};
		class GetRequest : public Request {
			public:
				GetRequest(MessageRequestHandler* rh) : Request(rh) {}
				string requestName() {
					return "get";
				}
				string parseMethod(istringstream& ss, string& reqst) {
					string name;
					int index;
					if(!(ss >> name)) return INVALID_REQUEST;
					if(!(ss >> index)) return INVALID_REQUEST;
					if(index < 1) return INVALID_REQUEST; //Index must be >=1
					return rh_p->handle_get(name,index);
				}


		};
		class ResetRequest : public Request {
			public:
				ResetRequest(MessageRequestHandler* rh) : Request(rh) {}
				string requestName() {
					return "reset";
				}
				string parseMethod(istringstream& ss, string& reqst) {
					return rh_p->handle_reset();
				}

		};
		//	PutRequest* pr;
		//	ListRequest* lr;
		//	GetRequest* gr;
		//	ResetRequest* rr;
		//vector<Request*> rq_list;
		Request* rq_list[4];
		int MAX;

		Request* getProperRequest(string& request) {
			/*Request* result;
			istringstream iss(request);
			string test;
			bool ct = false;
			while(iss >> test) {
				for(i = 0; i < MAX; i++) {
					if(test == rq_list[i]->requestName()) { //test for first protocol token occurence
						ct = true;
						result = rq_list[i];
						break;	
					}
				}
				if(ct) break;	
			}
			
			*/
			string cpy = request;
			int firstOccurence = -1;
			Request* rqst = NULL;
			int pstn;
			for(int i = 0; i < MAX; i++) {
				pstn = cpy.find(rq_list[i]->requestName());
				//pstn = cpy.find(rq_list.at(i).requestName());
				if(pstn >= 0 && ((pstn < firstOccurence) || firstOccurence < 0)) {
					firstOccurence = pstn;
					rqst = rq_list[i];
			//		cout << "new request: " <<rqst->requestName() << endl;
					cpy = cpy.substr(0, (pstn)+1);
				}
			//	cout << "pstn: " <<  pstn << endl;
			//	cout << "firstPostion: " << firstOccurence << endl;
			}
			if(firstOccurence >= 0) request.erase(0,firstOccurence);
			//cout << "REQUEST STRING: " << request  << endl;
			//cout << "last request naem: " << rqst->requestName() << endl;	
			return rqst;
		}
	public:
		MessageAutoServe(MessageRequestHandler*,int,bool);
		~MessageAutoServe();	
		string parseRequest(string request) {
			//istringstream iss(request);
			//cout << "parsing message" << endl;
			
			Request* rqst = getProperRequest(request);
			if(rqst != NULL) {
				istringstream iss(request);
				string rmv;
				iss >> rmv;
				return rqst->parseMethod(iss, request);
			}
			//cout << "NO Requests were matched." << endl;
			return "error invalid request\n"; //No request could be matched.
			//		//bool b = true;	
			//		while(ss >> rq_type)
			//		{
			/*	int firstOccurence = -1;
				int position;
				char requestIndicator = '0';
				position = request.find("put");
				if(position >= 0 && ((position < firstOccurence) || firstOccurence == -1)) {
				firstOccurence = position;
				requestIndicator = 'p';
				}
				position = request.find("list");
				if(position >= 0 && ((position < firstOccurence) || firstOccurence == -1)) {
				firstOccurence = position;
				requestIndicator = 'l';
				}
				position = request.find("get");
				if(position >= 0 && ((position < firstOccurence) || firstOccurence == -1)) {
				firstOccurence = position;
				requestIndicator = 'g';
				}	
				position = request.find("reset");
				if(position >= 0 && ((position < firstOccurence) || firstOccurence == -1)) {
				firstOccurence = position;
				requestIndicator = 'r';
				}
				*/

			/*	
				istringstream ss(request);
				string rq_type;
				ss >> rq_type;
				if(!rq_type.compare("put")) {
				return parsePut(request,ss);//if(!parsePut(ss)) b = false;	
				} else if(!rq_type.compare("list")) {
				return parseList(ss);//if(!parseList(ss)) b = false;
				} else if(!rq_type.compare("get")) {
				return parseGet(ss);//if(!parseGet(ss)) b = false;
				} else if(!rq_type.compare("reset")) {
				return req_handle->handle_reset();
				} else {
				return "OK\n";//b = false;
				}
				*/
			//return b;
		}

		//	string parseRequest(string);
		protected:
		void handle(int);
		private:
		//PutRequest pr;
		//ListRequest lr;
		//GetRequest gr;
		//ResetRequest rr;
		bool debug;
		//MessageRequestHandler* req_handle;
		void rsp_output(char* rsp) {
			istringstream iss(rsp);
		}



		};

		MessageAutoServe::MessageAutoServe(MessageRequestHandler* rh, int port_
				, bool debug = false) : InetServer(port_)  {
			this->debug = debug;
			//		static const MessageRequestHandlerRequest::
			//			Request::rh_p = rh_p;
			//	req_handle = rh_p;
		
 			Request* pr;
			Request* lr;
			Request* gr;
			Request* rr;
 	//		Request * pr;
			pr = new PutRequest(rh);
			lr = new ListRequest(rh);
			gr = new GetRequest(rh);
			rr = new ResetRequest(rh);
		
		//	rq_list = {new PutRequest(rh),new ListRequest(rh),new GetRequest(rh),new ResetRequest(rh)};
			rq_list[0] = pr;
			rq_list[1] = lr;
			rq_list[2] = gr;
			rq_list[3] = rr;
			MAX = 4;
		
		/*
			rq_list.push_back(PutRequest(rh));
			rq_list.push_back(ListRequest(rh));
			rq_list.push_back(GetRequest(rh));
			rq_list.push_back(ResetRequest(rh));	
		*/
		}
		MessageAutoServe::~MessageAutoServe() {
		
 			for(int i = 0; i < MAX; i++) {
				delete rq_list[i];
			}
		
		}
		void MessageAutoServe::handle(int client) {
			// loop to handle all requests
			while (1) {
				// get a request
				string request = get_request(client);
				// break if client is done or an error occurred

				if (request.empty())
					break;
				if (debug) cout << "request: " << request << endl;
				// -HANDLE- response
				string rsp = parseRequest(request);
				if(debug) cout << "response: " << rsp << endl;	
				bool success = send_response(client,rsp);
				// break if an error occurred
				if (not success)
					break;
			}
			close(client);
		}

#endif
