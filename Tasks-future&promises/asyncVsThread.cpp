#include "utils.hpp"

int main(){

  std::cout << std::endl;

  int res;
  std::thread t([&]{ res = 2000 + 11; }); //shared variable bw threds
  t.join(); //waits
  std::cout << "res: " << res << std::endl;             

  auto fut= std::async([]{ return 2000 + 11; }); //comm channel between promise and future
  std::cout << "fut.get(): " << fut.get() << std::endl;   //Here fut.get() is blocking
std::cout << "fut.gnsdnnet(): " << std::endl; 
  std::cout << std::endl;   

}
/*****Notes********/
//1.Communication between the creator thread and the created thread requires the use of a shared variable.
// The task communicates via its data channel which is implicitly protected; 
//therefore, a task must not use a protection mechanism like a mutex.
