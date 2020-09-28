#include "utils.hpp"
int main(){
    std::cout<<std::endl;
    auto begin=std::chrono::system_clock::now();
    auto futLazy=std::async(std::launch::deferred,[]{return std::chrono::system_clock::now();}); //This will be executed when the fut.get() will be get
    auto futEager=std::async([]{return std::chrono::system_clock::now();}); //This will be executed immediately
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    auto lazydur=futLazy.get()-begin;
    auto lazyEager=futEager.get()-begin;
    std::cout<<"Future lazy  "<<lazydur.count()<<"\n";
    std::cout<<"Future duration  "<<lazyEager.count()<<"\n";
    std::cout<<std::endl;
}