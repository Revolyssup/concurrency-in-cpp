//Testing out thread_local which creates thread local variable.Therefore any changes made to that variable from a specific thread will only be
//contained to the lifetime of that thread and only for that thread.
#include "utils.hpp"
thread_local std::string s("Hy from ");
std::mutex m;

void addTothread(const std::string& s2){
    s+=s2;
    //Locking
    std::lock_guard<std::mutex> l(m);
    std::cout<<"local string is: "<<s<<std::endl;
    std::cout<<"address of above said local string is: "<<&s<<std::endl;
}
int main(){
      std::cout << std::endl;

  std::thread t1(addTothread,"t1"); 
  std::thread t2(addTothread,"t2"); 
  std::thread t3(addTothread,"t3"); 
  std::thread t4(addTothread,"t4"); 

  t1.join();
  t2.join();
  t3.join();
  t4.join();
}