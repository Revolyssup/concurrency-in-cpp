#include "utils.hpp"
std::mutex m;
void prod(std::promise<double>&& promise,double a ,double b){ //This function takes in a promise and uses that promises to set a value in the channel.
    m.lock();
    std::cout<<"\nMultiplication occured in thread: "<<std::this_thread::get_id();
    m.unlock();
    promise.set_value(a*b);
}

void division(std::promise<double>&& promise,double a ,double b){//This function takes in a promise and uses that promises to set a value in the channel.
    m.lock();   
    std::cout<<"\nDivision occured in thread: "<<std::this_thread::get_id();
    m.unlock();
    promise.set_value(a/b);
}

int main(){
    int a=10;
    int b=0;
    std::promise<double> prodProm; // creating promises
    std::promise<double> divideProm;

    std::future<double> prodFut=prodProm.get_future(); //setting there futures. Now channel is completed
    std::future<double> divideFut=divideProm.get_future();
    //The futures remain in main thread whereas the promises are executed in another thread.Therefore establishing an intra thread promise-future commmunication.
    std::thread t1(prod,std::move(prodProm),a,b);
     std::thread t2(division,std::move(divideProm),a,b);
     t1.join();
     t2.join();
     
    std::cout<<"\na*b : "<<prodFut.get()<<"\n";
    std::cout<<"a/b : "<<divideFut.get()<<"\n";
}

