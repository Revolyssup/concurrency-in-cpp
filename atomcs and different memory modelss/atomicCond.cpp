/*
This type of thread synchronization is similar to spinlock as it also follows pull principle.
Here instead of using atomic_flags ,we use the data type atomic<bool> which
sets an atomic flag via the constructor and just like spinlock the waiting thread
constantly tries to lock itself.Here I have simulated a mutex like behaviour by putting the thread
t2 to sleep.If you don't put t2 at sleep,it will follow pull principle and will constantly try to lock itself,
which will keep our thread busy.
*/

#include<atomic>
#include<thread>
#include<iostream>
#include<vector>


std::atomic<bool> dataReady(false);
std::vector<int>  sharedVec(5);

void printingThread(){
    std::cout<<"Waiting for input...\n";
    while(!dataReady.load())
    {
        /*As we know that the thread t1 will not change the dataReady for 5000 ms
        so there is no use of keeping the thread t2 busy for that time. 
        If you put it to sleep at 4999ms the entire operation will take 9999ms instead of
        5000ms because the flag would not be true after 4999 ms and t2 will again sleep for 4999ms*/
        std::this_thread::sleep_for(std::chrono::milliseconds(5001));
    }
    for(auto i: sharedVec)
        {
            std::cout<<i<<" ";
        }
        std::cout<<"Printed all!!"<<std::endl;

}

void inputThread(std::vector<int> input)
{
    std::cout<<"Inputting data...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    sharedVec=input;
    dataReady=true;
    std::cout<<"Data prepared\n";
}

int main()
{
    std::vector<int> myarr={1,2,3,4,5};
    std::thread t2(printingThread);
    std::thread t1(inputThread,myarr);
  

    t1.join();
    t2.join();
    
}

/*
Note: Compile this unit with the flag -lpthread which tells the compiler that it has to link the pthread /POSIX thread library.
*/