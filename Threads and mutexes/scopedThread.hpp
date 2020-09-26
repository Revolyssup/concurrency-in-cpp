//This is a wrapper class aroung a thread.
#pragma once
#include "utils.hpp"
namespace atstl{
    class scoped_thread{
        std::thread t;
        public:
        explicit scoped_thread(std::thread _t):t(std::move(_t)){
            if(!t.joinable()) throw std::logic_error("No such thread exists");
        }
        ~scoped_thread(){ t.join();}
        scoped_thread(scoped_thread&)=delete; //deleting its copy constructor
        scoped_thread& operator=(scoped_thread const &)= delete;
    };
}