#include"./scopedThread.hpp"
#include<iostream>
void print(){
    std::cout<<"Bruh\n";
}
int main(){
    std::thread t1(print);
    atstl::scoped_thread mythread(t1);
}