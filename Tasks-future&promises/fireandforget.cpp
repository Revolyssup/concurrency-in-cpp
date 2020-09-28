#include "utils.hpp"

int main(){

    //Because these futures are not bounded to a varible.There is no use in waiting on them on this thread.
    std::async(std::launch::async,[]{ //this automatically goes to a different thread than main thread
         std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout<<std::this_thread::get_id()<<"Thread 1\n";
    }); //future waits till the destructor of its promise gets called. So the main thread will be blocked
    //Although the promise is getting executed in another thread,the future is in this thread and hence is blocking the program
     std::cout<<"bw Thread \n";
     std::async(std::launch::async,[]{//this automatically goes to a different thread
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout<<std::this_thread::get_id()<<"Thread 2\n";
    });
     std::cout<<std::this_thread::get_id()<<"Main Thread \n";
    std::cout<<std::endl;
}