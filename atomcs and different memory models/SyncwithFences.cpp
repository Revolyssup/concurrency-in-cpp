#include<thread>
#include<iostream>
#include<atomic>

std::atomic<int> val={0};
std::atomic<int> val2={0};

void producer(){//                                                                                                              ||
    val.store(10,std::memory_order_relaxed); //Weakened the memory order as we gonna sync from next line anyways                ||
    std::atomic_thread_fence(std::memory_order_release);//                                                                      \/=======               
    val2.store(20,std::memory_order_relaxed);//                                                                                         ||
}//                                                                                                                                     ||  Synchronization
void consumer(){//                                                                                                                      ||
    while(!val2.load(std::memory_order_relaxed));//                                                                                     ||        
    std::atomic_thread_fence(std::memory_order_acquire);//                                                                      ||========  
    std::cout<<val2<<"<==val2 | val==>  "<<val.load(std::memory_order_relaxed)<<"\n";//                                         ||    
}//                                                                                                                             \/

int main(){
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
}