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
long long size = 1024*1024;
char* NumTasks = new char[size];
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
            sleep(0.00000001);
        }
    }

    void mutexTraversal(short countThreads){
        ind = 0;
        for(int i = 0; i < countThreads; i++){
            threads[i] = thread(mutexAction);
        }
        for(int i = 0; i < countThreads; i++){
            threads[i].join();
        }
        }
//////////////////////////////////////////// atomic
    void atomicAction(){
        while(true){

            int i = atomicInd++;

            if(i >= size) return;

            NumTasks[i]++;
            sleep(0.00000001);
        }
    }

    void atomicTraversal(short countThreads){
        atomicInd = 0;
        for(int i = 0; i < countThreads; i++){
            threads[i] = thread(atomicAction);
        }
        for(int i = 0; i < countThreads; i++){
            threads[i].join();
        }
        }
//////////////////////////////////////////// 
double time(void (*f)(short), short a){
    int start_time, end_time;
    start_time = clock();
    f(a);
    end_time = clock();
    return (end_time - start_time)/1000000.0;
}


int main(){
    cout << "4 - " <<time(mutexTraversal, 4) << " c" << '\t' << time(atomicTraversal,4) << '\n';
    cout << "8 - " <<time(mutexTraversal, 8) << " c" << '\t' << time(atomicTraversal,8) << '\n';
    cout << "16 - " <<time(mutexTraversal, 16) << " c" << '\t' << time(atomicTraversal,16) << '\n';
    cout << "32 - " <<time(mutexTraversal, 32) << " c" << '\t' << time(atomicTraversal,32) << '\n';
    for(int i = 0; i < 1024; i++)
        cout << int(NumTasks[i]);
    cout << '\n';

}

 