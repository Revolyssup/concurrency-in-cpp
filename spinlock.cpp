/*
Creating spinlock with atomic flags .Unlike mutexes,a thread which is spinlocked doesn't passively wait and constantly tries to lock the thread untill it finally gets the lock
*/


#include<iostream>
#include<atomic>
#include<thread>
#include<pthread.h>


class spinlock{

    std::atomic_flag flag;
    public:
    //setting the default value of atomic flag to false using ATOMIC_FLAG_UNIT.
    spinlock():
    flag(ATOMIC_FLAG_INIT){}

    /*
    flag.test_and_set() is an atomic operation which returns true only if the flag was initially set to false which we did in constructor.So the first process which accesses the spin.lock() method will get locked and if another thread t2 tries to access spin.lock() it will not get locked untill the flag is again set to false with unlock method.
    */
    void lock(){
        while(flag.test_and_set());
    }

    /*flag.clear() unlocks spin by setting the flag to false so that this second thread t2 which constantly was trying to access spin.lock() can now lock itself by flag.test_and_set().
    */
    void unlock(){
        flag.clear();
    }

};

//initializing spin
spinlock spin;

//declaring a global variable a which we will use as shared resource bw threads.
int a;

//This is the function from which resources are shared between threads.
void someSharedwork(int n){
    spin.lock();

    a=n;
    std::cout<<"Current value of a is "<<a;
    spin.unlock();

    std::cout<<"Work done "<<std::endl;
}

int main()
{   
    /*
    spawning two threads and passing the function someSharedwork as a callable unit with different parameters.
    */
    std::thread t1(someSharedwork,5);
    std::thread t2(someSharedwork,10);

    //syncing threads
    t1.join();
    t2.join();
    return 0;
}

/*
Note: Compile this unit with the flag -lpthread which tells the compiler that it has to link the pthread library.
*/