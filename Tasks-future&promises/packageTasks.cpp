#include "utils.hpp"
class Sum{
    public:
    int operator()(int beg,int end){
        long long int sum{0};
      for (int i = beg; i < end; ++i ) sum += i;
      return sum; 
    }
};

int main(){
    Sum sum1;
    Sum sum2;
    Sum sum3;
    Sum sum4;
    Sum sumsingle;
    //Packaging the tasks
    std::packaged_task<int(int,int)> task1(sum1);
    std::packaged_task<int(int,int)> task2(sum2);
    std::packaged_task<int(int,int)> task3(sum3);
    std::packaged_task<int(int,int)> task4(sum4);

    //creating the future
    auto future1=task1.get_future();
    auto future2=task2.get_future();
    auto future3=task3.get_future();
    auto future4=task4.get_future();

    //Executing promise in different threads
    std::deque<std::packaged_task<int(int,int)>> allTasks;
    allTasks.push_back(std::move(task1));
    allTasks.push_back(std::move(task2));
    allTasks.push_back(std::move(task3));
    allTasks.push_back(std::move(task4));
    int begin=0;
    int inc=2500;
    int end=begin+inc;
    while(!allTasks.empty()){
        std::packaged_task<int(int,int)> task=std::move(allTasks.front());
        allTasks.pop_front();
        std::thread sumThread(std::move(task),begin,end);
        begin=end;
        end+=inc;
        sumThread.detach();
    }

    //pick the results
    auto sum=future1.get()+future2.get()+future3.get()+future4.get();
    std::cout<<"The sum: "<<sum<<std::endl;
     std::cout<<"The sum: "<<sumsingle(0,10000)<<std::endl;
}