#include<iostream>
#include<memory> //for shared pointers
#include<thread>
#include<atomic>
#include<vector>
/*The atomic operations can also be performed on Shared pointers because the control block they maintain,
increments or decrements its value in a thread safe way*/
/*Below are all the atomic operations that can be performed on shared pointers*/

/*std::atomic_is_lock_free(std::shared_ptr)
std::atomic_load(std::shared_ptr)
std::atomic_load_explicit(std::shared_ptr)
std::atomic_store(std::shared_ptr)
std::atomic_store_explicit(std::shared_ptr)
std::atomic_exchange(std::shared_ptr)
std::atomic_exchange_explicit(std::shared_ptr)
std::atomic_compare_exchange_weak(std::shared_ptr)
std::atomic_compare_exchange_strong(std::shared_ptr)
std::atomic_compare_exchange_weak_explicit(std::shared_ptr)
std::atomic_compare_exchange_strong_explicit(std::shared_ptr)*/

/*Below code demonstrates how using above given atomic operations on shared pointers ensure no race arounf conditions*/

int main(){
    std::shared_ptr<int> ptr=std::make_shared<int>(15);
    std::vector<int> storeptr;
    for(int i=0;i<5;i++){ //creating 5 threads
          std::thread ([&ptr,i]{ //passed ptr by reference,so changes will be made to it.
            // ptr=std::make_shared<int>(i); 
            std::this_thread::sleep_for(std::chrono::milliseconds((i)));
            std::shared_ptr<int> temp=std::make_shared<int>(i);
            std::atomic_store(&ptr,temp);
            std::cout<<"The ptr value at iteration is "<<*(ptr)<<"\n";
          }).detach();
    }
std::this_thread::sleep_for(std::chrono::milliseconds(5));
}