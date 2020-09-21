#include<thread>
#include<atomic>
#include<iostream>
#include<vector>

std::atomic<int> counter={0};
void add(){
    for(int i=0;i<1000;i++) counter.fetch_add(1,std::memory_order_relaxed); //There has to be no ordering constraints on counter as doesnt matter which thread,when increments it.
}

int main(){
    std::vector<std::thread> v;
    auto start=std::chrono::high_resolution_clock::now();
    for(int i=0;i<10;i++){
        v.emplace_back(add);
    }
    for(auto& t : v){
        t.join();
    }
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    std::cout<<"Total time taken: "<<duration.count()<<"\n";
    std::cout<<"The counter value is: "<<counter<<"\n";
}