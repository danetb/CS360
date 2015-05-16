#include "inet-server.h"
#include "message-serve.h"
#include <queue>
#include <semaphore.h>
#include <pthread.h>
using namespace std;

#ifndef MSG_SRV_THREAD
#define MSG_SRV_THREAD
class ClientQueue {
    public:
        ClientQueue() {
            sem_init(&s,0,1);
            sem_init(&n,0,0);
        }
        int getNext();
        void add(int);
    private:
       sem_t s;
       sem_t n;
        std::queue<int> clients;
};
int ClientQueue::getNext() {
    //SYNCHRONIZE W
        sem_wait(&n);
        sem_wait(&s);
        if(clients.empty()) {
            cout << "Something went wrong..." << endl;
            sem_post(&s);
            return -1;
        }
        int c = clients.front();
        clients.pop();
        sem_post(&s);
        return c;
     
};
void ClientQueue::add(int c) {
    //SYNCHRONIZE W
    sem_wait(&s);
    clients.push(c);
    sem_post(&s);
    sem_post(&n);
}
class WorkerThread : public MessageAutoServe {
    public:
        WorkerThread(ClientQueue* cq, MessageRequestHandler* rh, MessageModel* mm, bool dbg = false) : MessageAutoServe(rh,dbg) {
            worker_id = ++live_thread_count;
            clients = cq;
            msg_data = mm;
            buflen_ = 1024;
            buf_ = new char[buflen_];
        }
        ~WorkerThread() {
            live_thread_count--;
            if(live_thread_count == 0) {
                delete clients;
                delete msg_data;
            }
           // delete clients;
            //delete msg_data;
        }
        void handle_procedure();
        int getId() {
            return worker_id;
        }
        /* 
           protected:
           int nextClient();
           */
        private:
        static int live_thread_count; 
        int worker_id;
        int current_client;
        char* buf_;
        int buflen_;
        ClientQueue* clients;
        MessageModel* msg_data;
        string get_request();
        bool send_response(string);

};
int WorkerThread::live_thread_count = 0;
void WorkerThread::handle_procedure() {
    // loop to handle all requests
  cout << "thread initiating handle phase: " << worker_id << endl;
  while(true) {
        int clt = clients->getNext();
       // if(clt < 0) return; // queue empty: resume or sleep, proceed w/ signal?
        current_client = clt;
        string request = "";
        while (true) {
           // string request = get_request(clt);
           string request = get_request();
           if(request.empty()) break;	
            if (debug) cout << "request: " << request << endl;
            //string rsp = parseRequest(request);		
            Request* rqstCmd = parseRequest(request);
            string rsp;
            if(rqstCmd == NULL) {
                rsp = "error invalid request\n"; //No request could be matched.
            } else {
                istringstream iss(request);
                string rmv;
                iss >> rmv; 
                if(msg_data == NULL) cout << "BIT OF PROBLEMS" << endl;
                rsp = rqstCmd->parseMethod(iss, request, this, msg_data);
            }
            //
            if(debug) cout << "response: " << rsp << endl;
            //bool success = send_response(clt,rsp);
            bool success = send_response(rsp);
            if (not success)
                break;
        }
    }
    close(current_client);
}
/*
   int WorkerThread::nextClient() {
   return clients.getNext();
   }
   */
string WorkerThread::get_request() {
    //int client = current_client;
    string request = "";
    int nread; 
    memset(buf_, 0, 1024);
    while(true) {
        nread = recv(current_client,buf_,1024,0);
        //	cout << "nread: " << nread << endl;
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else {
                // an error occurred, so break out
                //          cout << "arbitrary errno: " << errno << endl;
                return "";
            }
        } else if (nread == 0) {
            // 
            //        cout << "read " << nread << " bytes from socket." << endl;
            //		the socket is closed

            //	 return ""; 
            break;
        } else if(nread < 1024) {
            request.append(buf_,nread);
            break;
        }
        // be sure to use append in case we have binary data
        request.append(buf_,nread);

    }
    return request;
}

bool WorkerThread::send_response(string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(current_client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

class ThreadedMessageServer : public InetServer {
    public:
            ThreadedMessageServer(int port, bool dbg = false) : InetServer(port) {
                debug = dbg;
            }
        void init_thread_pool(int);
    private:
        bool debug;
        ClientQueue clients;
        void handle(int);
         
//        static void* runThread(void*);
        static void* runThread(void* args) {
            WorkerThread* worker = (WorkerThread*)args;
    // call methods on this class
             worker->handle_procedure();
        }
};

void ThreadedMessageServer::handle(int client) {
    clients.add(client);
}

void ThreadedMessageServer::init_thread_pool(int num_threads) {
    pthread_t threads[num_threads];
    MessageModel* messages = new MessageModel();
    MessageRequestHandler* mrh = new MessageRequestHandler();
    for(int i = 0; i < num_threads; i++) {
        WorkerThread* w = new WorkerThread(&clients,mrh,messages,this->debug);
        pthread_create(&threads[i], NULL, runThread, (void *) w);
    }
}



#endif
