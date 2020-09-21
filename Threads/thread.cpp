//Usage of some common functions of threads.
#include<thread>
#include<mutex>
#include<iostream>
#include<string>
std::mutex _m;
void getid(std::string t){
    auto id=std::this_thread::get_id();
    _m.lock();
    std::cout<<t<<" has id: "<<id<<std::endl;
    _m.unlock();
}
int main(){
    std::cout << "hardware_concurrency()= "<< std::thread::hardware_concurrency() << std::endl;
    std::thread t1(getid,"t1");
    std::thread t2(getid,"t2");
    std::cout<<"Id of t1 from main is : "<<t1.get_id()<<std::endl;
    std::cout<<"Id of t2 from main is : "<<t2.get_id()<<std::endl;
    std::swap(t1,t2);
    std::cout<<"Id of t1 from main is : "<<t1.get_id()<<std::endl;
    std::cout<<"Id of t2 from main is : "<<t2.get_id()<<std::endl;
    std::cout<<"Before join,is t1 joinable: "<<std::boolalpha<<t1.joinable()<<std::endl;
    t1.join();
    t2.join();
      std::cout<<"Id of main is : "<<std::this_thread::get_id()<<std::endl;
    std::cout<<"After join,is t1 joinable: "<<std::boolalpha<<t1.joinable()<<std::endl;

}