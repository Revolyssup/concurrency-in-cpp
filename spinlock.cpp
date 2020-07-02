/*
Creating spinlock with atomic flags .Unlike mutexes,a thread which is spinlocked doesn't passively wait and constantly tries to lock the thread untill it finally gets the lock
*/


#include<iostream>
#include<atomic>
#include<thread>
#include<pthread.h>

/*
    This class acts as an interface for threads therefore locking and unlocking them according to atomic flag.
*/
class spinlock{

    std::atomic_flag flag;
    public:
    //setting the default value of atomic flag to false using ATOMIC_FLAG_INIT.
    spinlock():
    flag(ATOMIC_FLAG_INIT){}

    /*

    flag.test_and_set() is an atomic operation which sets flag to true and returns true
    only if the flag was initially set to false which we did in constructor.
    So the first process which accesses the spin.lock() method will get locked and
    if another thread t2 tries to access spin.lock() ,while loop returns true to t2/t3 so t2/t3 is caught in the rat race.
    When t1 completes,it executes flag.clear() and sets flag to false.And instantly t2/t3 gets locked and starts executing
    and sets the flag again to true with flag.test_and_set() and now the remaining other thread is caught in the rat race.

    */
    void lock(){
        while(flag.test_and_set());
    }

    /*flag.clear() unlocks spin by setting the flag to false so that 
    this second thread t2 which constantly was trying to access spin.lock() 
    can now lock itself by spin.lock().
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
    spawning three threads and passing the function someSharedwork as a callable unit with different parameters.
    */
    std::thread t1(someSharedwork,5);
    std::thread t2(someSharedwork,10);
    std::thread t3(someSharedwork,15);
    //syncing threads
    t1.join();
    t2.join();
    t3.join();
    return 0;
}

/*
Note: Compile this unit with the flag -lpthread which tells the compiler that it has to link the pthread /POSIX thread library.
*/

/*
A VERY IMPORTANT NOTE: Since the flag t2 and t3 are competing to get the lock while t1 executes,
therefore any thread can be locked after t1 sets the flag to false again.

Which implies there can be two possible outputs of above code:

Output 1-
    Current value of a is 5 Work done 
    Current value of a is 10 Work done
     Current value of a is 15 Work done 

Output 2-
    Current value of a is 5 Work done 
    Current value of a is 15 Work done
     Current value of a is 10 Work done      
*/



/*
Something to remember:
    One of the advantages of using atomic_flags instead of normal bool flags is that
    the method test_and_set() is an atomic operation that means it checks the current value of flag and
    sets is accordingly and instead of being two different operations,this whole thing is one atomic operation.
    Atomic operation is basically the operation which can't be further divided(or interrupted) as the term atom-ic.
*/