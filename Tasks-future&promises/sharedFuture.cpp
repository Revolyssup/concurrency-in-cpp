//One or more futures can subscribe to a single promise.Small example given below.
//This can be used when many threads depend on some same value returned by some function.
#include "utils.hpp"
double twice;
double oneFifth;
void setData(std::promise<double>&& setter,int n){
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    setter.set_value(n);
}
void tw(std::shared_future<double>&& getter){
    twice=2*getter.get();
}
void of(std::shared_future<double> &&getter){
    oneFifth=0.2*getter.get();
}
int main(){
    int n;
    std::cout<<"Enter n:\n";
    std::cin>>n;
    std::promise<double> set;
    std::shared_future<double> f1= set.get_future();
    std::shared_future<double> f2= f1;

    std::thread t1(setData,std::move(set),n);
    std::thread t2(tw,std::move(f1));
    std::thread t3(of,std::move(f2));
    t1.join();
    t2.join();
    t3.join();
    std::cout<<"twice and onefifth are : "<<twice<<" "<<oneFifth<<" \n";
}
