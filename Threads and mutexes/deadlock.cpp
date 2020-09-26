/*Deadlock simulation*/
#pragma once
#include "utils.hpp"
class CriticalData{
    //some data
    public:
    std::mutex m;
};

void deadlock(CriticalData& c1,CriticalData& c2){
    c1.m.lock();
    std::cout<<"First mutex has locked the thread\n";
    c2.m.lock();
    std::cout<<"Second mutex has locked the thread inside first lock\n";
    c2.m.unlock();
    c1.m.unlock();
}
/*In this example thread 1 locks m1 and wants to lock m2.
And thread 2 locks m2 and wants to lock m1. Both are neither giving m1 nor m2,hence both threads are in a standstill.
This problem in wrong ordering mutexes corresponding to standstill Or any way that can stop the unlocking is known as deadlock*/
int main(){
    CriticalData c1;
    CriticalData c2;
    std::thread t1([&]{deadlock(c1,c2);});
    std::thread t2([&]{deadlock(c2,c1);}); //Different order
    t1.join();
    t2.join();
    return 0;
}

/*Tip: Never call an untrustworthy function inside a locked mutex because
if that function throws an exception then mutex will again go in a deadlock.*/