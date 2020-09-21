/*With acquire-release we can synchronize operations on a given atomic variable with less ordering constraints than sequential consistency therefore grtr speed*/
/*Below is given spinlock implementation with acquire-release semantics.*/
#include<thread>
#include<atomic>
#include<iostream>
#include<chrono>
class spinlock{
    std::atomic_flag flag;
    public:
    spinlock():flag(ATOMIC_FLAG_INIT){};
    void lock(){
        while(flag.test_and_set(std::memory_order_acquire)){}
    }
    void unlock(){
        flag.clear(std::memory_order_release);
    }
};

spinlock spin;
int sharedVar;
void sharedWork(int a){
    spin.lock();
    sharedVar=a;
    std::cout<<a<<"\n";
    spin.unlock();
}
int main(){
    auto start=std::chrono::high_resolution_clock::now();
    std::thread t1(sharedWork,10);
    std::thread t2(sharedWork,20);
    t1.join();
    t2.join();
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    std::cout<<"Total time taken: "<<duration.count();
}