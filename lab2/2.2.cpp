#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <unistd.h>
#include <string>
#include <ctime>
#include <queue>
#include <condition_variable>

typedef long long ll;

using namespace std;

mutex m;
mutex mConsum;
condition_variable cv;
condition_variable cvConsum;
thread consumers[6];
thread producers[6];
ll TaskNum = 4*1024;
double* prodTime = new double[4]; 
int workingProducers = 1;

class Queue{
protected:
    queue<uint8_t> _queue;
public:
    virtual void push(uint8_t val) = 0;
    virtual bool pop(uint8_t& val) = 0;
    ll size(){
        return _queue.size();
    }
};

class DynamicQueue : public Queue{
public:
    void push(uint8_t val){
        m.lock();
        _queue.push(val);
        m.unlock();
    }
    bool pop(uint8_t& val){
        m.lock();
        if(_queue.empty()){
            m.unlock();
            this_thread::sleep_for(chrono::milliseconds(10));
            m.lock();
            if(_queue.empty()){
                m.unlock();
                return false;
            }

        }
        val = _queue.back();
        _queue.pop();
        m.unlock();
        return true;       
    }
    ll size(){
        return _queue.size();
    }
};

class FixQueue : public Queue{
private:
    short QueueSize = 16;
public:
    FixQueue(short size){
        QueueSize = size;
    }
    FixQueue(){}
    void push(uint8_t val){
        unique_lock<mutex> ul(m);
        cv.wait(ul, [this] {return (_queue.size() < QueueSize);});
        _queue.push(1);
    }
    bool pop(uint8_t& val){
       m.lock();
        if(_queue.empty()){
            m.unlock();
            this_thread::sleep_for(chrono::milliseconds(10));
            m.lock();
            if(_queue.empty()){
                m.unlock();
                return false;
            }
        }
        val = _queue.back();
        _queue.pop();
        m.unlock();
        cv.notify_all();
        return true;
    }
};

double time(void (*f)(short), short a){
    int start_time, end_time;
    start_time = clock();
    f(a);
    end_time = clock();
    return (end_time - start_time)/1000000.0;
}

void queueProducerAction(Queue* q, double *time){
    int start_time, end_time;
    start_time = clock();
    for(ll i = 0; i < TaskNum; i++){
        q->push(1);
    }
    workingProducers--;
    cout << "one worker is done\n";
    end_time = clock();
    *time = (end_time - start_time)/1000000.0;
}

void queueConsumerAction(Queue* q){
    uint8_t val;
    while(q->pop(val) || workingProducers > 0){ 
    }
    cout << "one consumer is done\n";
}

void check(Queue* q){
    while(q->size() > 0){
        cout << q->size() << '\n';
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void instancing(Queue* q, int countConsumer, int countProducer){


    for(int i = 0; i < countConsumer; i++){
        consumers[i] = thread(queueConsumerAction,q);
    }

    workingProducers = 0;
    for(int i = 0; i < countProducer; i++){
        producers[i] = thread(queueProducerAction,q, &prodTime[i]);
        workingProducers++;
    }

    // thread t(check, q);
    for(int i = 0; i < countConsumer; i++){
        consumers[i].join();
    }
    for(int i = 0; i < countProducer; i++){
        producers[i].join();
    }
    // t.join();
}

int main(){
    Queue* queue = new FixQueue();
    int prod = 6;
    int consum = 6;
    instancing(queue, prod,consum);     
    double alltime = 0;
    for(int i = 0; i < prod; i++){
        alltime += prodTime[i];
    }
    cout << alltime << '\n';
    return 0;
}
