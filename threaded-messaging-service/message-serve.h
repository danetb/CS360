#include "message-handler.h"
#include "message-model.h"
//#include "inet-server.h"
#ifndef MSG_AUTOSRV
#define MSG_AUTOSRV
#define INVALID_REQUEST "error request bad format\n"

class MessageAutoServe {
    protected:
        bool debug;
        //	virtual void handle_procedure(int) =0;
        virtual string get_request() =0;
        //      virtual bool send_response(int,string) =0;
        class Request {
            public:
                Request(MessageRequestHandler* rh_p) {
                    this->rh_p = rh_p;
                    //   r_cache = NULL;
                }
                //			virtual string parseMethod(istringstream& iss,string& reqst, MessageAutoServe* mServ_ptr, MessageModel* mod) =0;// { return "OK\n";}
                virtual string parseMethod(istringstream& iss,string& reqst, MessageAutoServe* mServ_ptr, MessageModel* mod) =0;// { return "OK\n";}
        //virtual string parseMethod() =0;
                virtual string requestName() =0;// {return "request";}
            protected:
                 MessageRequestHandler* rh_p;		

        };
    private:
        class PutRequest : public Request {
            public:
                PutRequest(MessageRequestHandler* rh/*,MessageAutoServe* mas_p*/) : Request(rh) {
                }	
                string requestName() {
                    return "put";
                }
                string parseMethod(istringstream& ss, string& reqst, MessageAutoServe* mServ_ptr, MessageModel* mod) {
                    //string parseMethod() {
                    //                         if(r_cache == NULL) return "error internal problem r_cache unitialized";
                    //                       istringstream& iss = r_cache->ss;
                    //                     string& rqst = r_cache->reqst;
                    //    MessageAutoServe* mSrv = r_cache->mServ_ptr;
                    string name;
                    string subject;
                    int length;
                    int msglen;		
                    if(!(ss >> name)) return INVALID_REQUEST;
                    if(!(ss >> subject)) return INVALID_REQUEST;	
                    if(!(ss >> msglen)) return INVALID_REQUEST;
                    string message = reqst.erase(0, reqst.find("\n") + 1);
                    //	if(mServ_ptr == NULL) return "error interal problem mSrv unitialized";	
                    while(message.length() < msglen) {
                        //	message.append(mServ_ptr->get_request(mServ_ptr->current_client));
                        message.append(mServ_ptr->get_request());
                    }
                    length = message.length();
                    return rh_p->handle_put(name,subject,length,message,mod);
                }
                };
                class ListRequest : public Request {
                    public:
                        ListRequest(MessageRequestHandler* rh) : Request(rh) {}
                        string requestName() {
                            return "list";
                        }
                        string parseMethod(istringstream& ss, string& reqst, MessageAutoServe* mServ_ptr, MessageModel* mod) {
                            //string parseMethod() {
                            //                              if(r_cache == NULL) return "error internal problem r_cache unitialized";
                            //                                istringstream& iss = r_cache->ss;
                            string name;
                            if(!(ss >> name)) return INVALID_REQUEST;
                            return rh_p->handle_list(name, mod);
                        }
                        };
                        class GetRequest : public Request {
                            public:
                                GetRequest(MessageRequestHandler* rh) : Request(rh) {}
                                string requestName() {
                                    return "get";
                                }
                                string parseMethod(istringstream& ss, string& reqst, MessageAutoServe* mServ_ptr, MessageModel* mod) {
                                    //        string parseMethod() {
                                    //                if(r_cache == NULL) return "error internal problem r_cache unitialized";
                                    //               istringstream& iss = r_cache->ss;
                                    string name;
                                    int index;
                                    if(!(ss >> name)) return INVALID_REQUEST;
                                    if(!(ss >> index)) return INVALID_REQUEST;
                                    if(index < 1) return INVALID_REQUEST; //Index must be >=1
                                    return rh_p->handle_get(name,index,mod);
                                }
                                };
                                class ResetRequest : public Request {
                                    public:
                                        ResetRequest(MessageRequestHandler* rh) : Request(rh) {}
                                        string requestName() {
                                            return "reset";
                                        }
                                        string parseMethod(istringstream& ss, string& reqst, MessageAutoServe* mServ_ptr, MessageModel* mod) {
                                            //string parseMethod() {

                                            return rh_p->handle_reset(mod);
                                        }

                                        };
                                        Request* rq_list[4];
                                        int MAX;
                                        Request* createRequest(string& request) {
                                            string cpy = request;
                                            int firstOccurence = -1;
                                            Request* rqst = NULL;
                                            int pstn;
                                            for(int i = 0; i < MAX; i++) {
                                                pstn = cpy.find(rq_list[i]->requestName());
                                                if(pstn >= 0 && ((pstn < firstOccurence) || firstOccurence < 0)) {
                                                    firstOccurence = pstn;
                                                    rqst = rq_list[i];
                                                    cpy = cpy.substr(0, (pstn)+1);
                                                }
                                            }
                                            if(firstOccurence >= 0) request.erase(0,firstOccurence);

                                            return rqst;
                                        }

                                        void rsp_output(char* rsp) {
                                            istringstream iss(rsp);
                                        }
                                    public:
                                        // int current_client;
                                        MessageAutoServe(MessageRequestHandler*,bool);
                                        ~MessageAutoServe();
                                        //		string parseRequest(string request) {
                                        Request* parseRequest(string& request) {
                                            Request* rqst = createRequest(request);
                                            if(rqst != NULL) {
                                                //istringstream iss(request);
                                                //	string rmv;
                                                //	iss >> rmv;
                                                //Set request fields for parseMethod execution:
                                                //istringstream, request(string), this
                                                //  rqst->setStream(iss);
                                                //  rqst->setField(request);
                                                // rqst->setRequestGetter(this);
                                                return rqst;
                                                //return rqst->parseMethod(iss, request, this);
                                            }
                                            //return "error invalid request\n"; //No request could be matched.
                                        }  

                                        ///*
                                        //*/
                                };

                                MessageAutoServe::MessageAutoServe(MessageRequestHandler* rh,/* int port_
                                                                                                ,*/ bool debug = false) /*: InetServer(port_)*/  {
                                    this->debug = debug;
                                    Request* pr;
                                    Request* lr;
                                    Request* gr;
                                    Request* rr;
                                    pr = new PutRequest(rh);
                                    lr = new ListRequest(rh);
                                    gr = new GetRequest(rh);
                                    rr = new ResetRequest(rh);
                                    rq_list[0] = pr;
                                    rq_list[1] = lr;
                                    rq_list[2] = gr;
                                    rq_list[3] = rr;
                                    MAX = 4;
                                }
                                MessageAutoServe::~MessageAutoServe() {
                                    for(int i = 0; i < MAX; i++) {
                                       cout << "MessageAutoServe:DESTRUCT" << endl;
                                       delete rq_list[i];
                                    }
                                }
                                /*
                                   void MessageAutoServe::handle_procedure() {
                                // loop to handle all requests
                                while(true) {
                                int clt = nextClient();
                                if(clt < 0) return; // queue empty: resume or sleep, proceed w/ signal?
                                current_client = clt;
                                string request = "";
                                while (1) {
                                string request = get_request(clt);
                                if(request.empty()) break;	
                                if (debug) cout << "request: " << request << endl;
                                //string rsp = parseRequest(request);		
                                Request* rqstCmd = parseRequest(request);
                                //rqstCmd->parseMethod(iss, request, this);
                                //
                                if(debug) cout << "response: " << rsp << endl;
                                bool success = send_response(clt,rsp);
                                if (not success)
                                break;
                                }
                                }
                                close(client);
                                }
                                */
                                //}

#endif
