#pragma once
#include "utils.hpp"

class CriticalData{
    //some data
    public:
    std::mutex m;
};

void deadlock(CriticalData& c1,CriticalData& c2){
    std::unique_lock<std::mutex> l1(c1.m,std::defer_lock); //Unique_lock manages the scope of the lock
    std::cout<<"First mutex has locked the thread\n";
    std::unique_lock<std::mutex> l2(c2.m,std::defer_lock);
    std::cout<<"Second mutex has locked the thread inside first lock\n";
    std::lock(l1,l2); //It is an atomic operation which makes sure that one lock doesnt happen during the other is locked.Either they lock at same time or
                     //if one of them goes out of scope.
}
int main(){
    CriticalData c1;
    CriticalData c2;
    std::thread t1([&]{deadlock(c1,c2);});
    std::thread t2([&]{deadlock(c2,c1);}); //Different order
    t1.join();
    t2.join();
    return 0;
}