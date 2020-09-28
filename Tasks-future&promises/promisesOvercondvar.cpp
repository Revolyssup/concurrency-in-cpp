/*The advantage of a condition variable to a promise and future is that you can use condition variables to synchronize threads multiple times. 
In contrast to that, a promise can send its notification only once, so you have to use more promise and future pairs to get the functionality of a condition variable. 
If you use the condition variable for only one synchronization, the condition variable is a lot more difficult to use in the right way. A promise and future pair needs no shared variable and, 
therefore, it doesn’t have a lock, and isn’t prone to spurious or lost wakeups. In addition to that, tasks can handle exceptions. There are lots of reasons to prefer tasks to condition variables.*/
#include "utils.hpp"

//Here I will implement the same worker thread relationship which i did in ./atomcs and different memory modelss/mutex-condVar.cpp in a much more simplified way.
std::vector<int> sharedVec;

void setDataReady(std::promise<void>&& done){
    std::cout<<"Preparing data..."<<std::endl;

    //sleeps the thread to get async behaviour
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    sharedVec={1,2,3,4,5,6,7,8,9,10};
    std::cout<<"Data prepared"<<std::endl;
    done.set_value(); //This will send the notification to the future.
}
void waitForWork(std::future<void> &&isdone,std::promise<void> &&update,int i,int n){
    isdone.wait(); //Will wait on its promise given in set Data ready
    std::cout<<"Waiting for data ..."<<std::endl;

    sharedVec[i]=n;
 
    std::cout<<"Work done!!!"<<std::endl;
    update.set_value(); //This will send notification to the further future.
}

void waitForUpdate(std::future<void> &&isupdate,int i,int n){
    isupdate.wait();
     std::cout<<"Updating data..."<<std::endl;
    sharedVec[i+1]=n+1;
    std::cout<<"update done"<<std::endl;
   
}

int main(){
    int i,n;
    std::cout<<"Enter the index to change: \n";
    std::cin>>i;
    std::cout<<"Enter new value at that index: \n";
    std::cin>>n;
    std::promise<void> done;
    std::promise<void> update;

    std::future<void> isdone=done.get_future();
    std::future<void> isupdate=update.get_future();
    std::thread t1(setDataReady,std::move(done));
    std::thread t2(waitForWork,std::move(isdone),std::move(update),i,n);
    std::thread t3(waitForUpdate,std::move(isupdate),i,n);
    t1.join();
    t2.join();
    t3.join();
     for(auto i:sharedVec){
        std::cout<<i<<" ";
    }
}

//Note: Promieses ,futures or in general threads cannot be copied .They can only passed as rvalue to a function with std::move