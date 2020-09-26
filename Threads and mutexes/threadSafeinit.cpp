//There are 3 ways to do thread safe initializations
//1. initialization of constexp variables or methods- This is because most of it is evaluated at compile time,thus making things safe at runtime.
//Note: A class can be constexp only if it doesnt have a constructor and all its methods are constexpr.

//2. Using std::call_once along with std::once_flag. callonce takes onceflag as first argument and a function as second.
//Once the function is called,the onceflag is set to false and hence no more instances can be created.Thus making the operation thread-safe
//as not all threads will be able to call that function.

//Using callonce to create a singleton class,that can have only once instance.

#pragma once
#include "utils.hpp"
class Singleton{
    private:
    static std::once_flag initflag; 
    static Singleton* one;//pointer to the single instance;
    Singleton()=default;
    ~Singleton()=default;

    public:
    Singleton(const Singleton&)=delete; //deleting copy constructor
    Singleton& operator=(const Singleton&)=delete;
    static void initSingleton(){
        one=new Singleton();
    }

    static Singleton* getInstance(){
        std::call_once(initflag,[](){initSingleton();});
        return one;
    }
    void print(){std::cout<<"hy\n";}
};

Singleton* Singleton::one=nullptr;
std::once_flag Singleton::initflag;
int main(){
    Singleton* add1=Singleton::getInstance();
    Singleton* add2=Singleton::getInstance();
    (*add1).print();
    std::cout<<"The address of Singleton's instance is :"<<add1<<std::endl;
    std::cout<<"The address of Singleton's instance is :"<<add2<<std::endl; //Both pointers point to same instance of that class
    return 0;
}