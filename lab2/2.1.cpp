#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <unistd.h>
#include <string>
#include <ctime>

using namespace std;

int ind = 0;
atomic<int> atomicInd (0);
long long size = 1024*102;
uint8_t* NumTasks = new uint8_t[size];
mutex m;
thread* threads = new thread[32];
////////////////////////////////////////// mutex
    void mutexAction(){
        while(true){

            m.lock();
            int i = ind++;
            m.unlock();
            
            if(i >= size) return;
            NumTasks[i]++;
            this_thread::sleep_for(chrono::nanoseconds(10));
        }
    }

//////////////////////////////////////////// atomic
    void atomicAction(){
        while(true){

            int i = atomicInd++;

            if(i >= size) return;

            NumTasks[i]++;
            this_thread::sleep_for(chrono::nanoseconds(10));
        }
    }

    double traversal(void (*f)(), short countThreads){
        int start_time, end_time;
        start_time = clock();
        ind = 0;
        atomicInd = 0;
        for(int i = 0; i < countThreads; i++){
            threads[i] = thread(f);
        }
        for(int i = 0; i < countThreads; i++){
            threads[i].join();
        }
        end_time = clock();
        return (end_time - start_time)/1000000.0;
    }

int main(){
    cout << "4 - " <<traversal(mutexAction, 4) << " c" << '\t' << traversal(atomicAction,4) << '\n';
    cout << "8 - " <<traversal(mutexAction, 8) << " c" << '\t' << traversal(atomicAction,8) << '\n';
    cout << "16 - " <<traversal(mutexAction, 16) << " c" << '\t' << traversal(atomicAction,16) << '\n';
    cout << "32 - " <<traversal(mutexAction, 32) << " c" << '\t' << traversal(atomicAction,32) << '\n';
    for(int i = 0; i < 1024; i++)
        cout << int(NumTasks[i]);
    cout << '\n';

}

 