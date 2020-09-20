/*
Creating spinlock with atomic flags .Unlike mutexes,a thread which is spinlocked doesn't passively wait and constantly tries to lock the thread untill it finally gets the lock
*/


#include<iostream>
#include<atomic>
#include<thread>


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
        while(flag.test_and_set(std::memory_order_acquire)) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    /*flag.clear() unlocks spin by setting the flag to false so that 
    this second thread t2 which constantly was trying to access spin.lock() 
    can now lock itself by spin.lock().
    */
    void unlock(){
        flag.clear(std::memory_order_release);
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
    std::cout<<"Current value of a is "<<a<<"\n";
    spin.unlock();

    std::cout<<"Work done \n";
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
    1.) One of the advantages of using atomic_flags instead of normal bool flags is that
    the method test_and_set() is an atomic operation that means it checks the current value of flag and
    sets is accordingly and instead of being two different operations,this whole thing is one atomic operation.
    Atomic operation is basically the operation which can't be further divided(or interrupted) as the term.

    2.) This spinlock uses pull principle.That means the thread doesn't sit idle listen for a message 
    from the thread currently accessing spinlock,it constantly tries to access the spinlock.
    Unlike this,when threads use the same mutex,they wait for messages enabled by condition_variable from 
    the thread currently active on mutex. 
*/


/*--------------------One more interesting thing---------------------------------------------------------------*/
/*
We can simulate a mutex-like and predictable behaviour by putting one of the threads to sleep for few ms,
before competing for the lock.

================================================================================================================
void someSharedworkfort2(int n){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    spin.lock();
    a=n;
    std::cout<<"Current value of a is "<<a;
    spin.unlock();

    std::cout<<"Work done "<<std::endl;
}
================================================================================================================
If we pass the above function as callable unit to t2,then the sequence obtained always will be
t1->t3->t2.

You can uncomment the above code and pass it to t2 ,to see for yourself.
*/