/*Synchronizing two threads accessing shared variable using
 condition_variable library and non-atomic bool*/
#include<iostream>
#include<condition_variable>
#include<thread>
#include<vector>
#include <sstream> 

/*This conVar will create a channel between the threads where they can listen
 and notify each other through messeges.*/
std::condition_variable conVar;

/*All the thread will share this mutex and pass off lock from one thread to another.*/
std::mutex mutex_;

/*This vector will be shared among three threads:
    t1-which will initialize its values.
    t2-which will mutate a given position to a given value.(given as main's arguments) 
    t3-which will update another position*/
std::vector<int> sharedVec;


//initializing non-atomic bool to false.
bool dataReady{false};
bool updateReady{false};

/*
This callable unit is allotted to t1 and it initializes the vector and then 
gives off control through conVar*/
void setDataReady(){
    std::cout<<"Preparing data..."<<std::endl;

    //sleeps the thread to get async behaviour
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    sharedVec={1,2,3,4,5,6,7,8,9,10};
    //sets a unique lock with common mutex
    std::unique_lock<std::mutex> lock(mutex_);
    dataReady=true;
    std::cout<<"Data prepared"<<std::endl;
    //notify via conVar
    conVar.notify_all();
}


/*This function is given to t2 and it waits for t1 to notify it and set its flag to true*/
void waitForWork(int i,int n){
    std::cout<<"Waiting for data ..."<<std::endl;

    //setting a unique lock with the common mutex  
    std::unique_lock<std::mutex> lock(mutex_); //locks the mutex
   
    /*waiting here for notification.along with a lamda function which returns true when t1 sets the flag to true.*/
    conVar.wait(lock,[]{return dataReady;}); //Unlocks the mutex and goes in wait state.Will evaluate predicate when get notified to see if it should acquire the lock back.
    //doing work when the lock is given to this function(t2)
    sharedVec[i]=n;
    //setting flag true for t3
    updateReady=true;
    std::cout<<"Work done!!!"<<std::endl;
    conVar.notify_all();
}


void waitForUpdate(int i,int n){
   
    std::unique_lock<std::mutex> lock(mutex_);
    conVar.wait(lock,[]{return updateReady;});
     std::cout<<"Updating data..."<<std::endl;
    sharedVec[i+1]=n+1;
    
    
    std::cout<<"update done"<<std::endl;
   
}

int main(int argc,char** argv){
     std::string a=(std::string)argv[1];
     std::string b=(std::string)argv[2];

     std::stringstream c(a);
     std::stringstream d(b);

    int i=0;
    int n=0;

    c>>i;
    d>>n;
    std::thread t1(setDataReady);
     std::thread t2(waitForWork,i,n);
     std::thread t3(waitForUpdate,i,n);

    t1.join();
    t2.join();
    t3.join();

    for(auto i:sharedVec){
        std::cout<<i<<" ";
    }

}  

/*
Note: Compile this unit with the flag -lpthread which tells the compiler that it has to link the pthread /POSIX thread library.
*/

/*Uses Push principle. Threads continue according to notifications pushed to conVar.Unlike spinlock which constanly 
tries to take control.*/

/*
Other note:In spinlock any thread could get locked after t1 completed as thet competed to get control.
            But here,we can switch controls based on particular bool flags.
*/