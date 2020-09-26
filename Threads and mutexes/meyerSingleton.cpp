#include<iostream>
class Meyer{
    public:
    static Meyer& getinstance(){
        static Meyer one;
        return one;
    } 
    void print(){std::cout<<this<<"\n";}
    private:
    Meyer()=default;
    ~Meyer()=default;
    Meyer(const Meyer&)=delete; //Deleting copy construtor.
    Meyer& operator=(const Meyer&)=delete;
};

int main(){
    Meyer::getinstance().print();
     Meyer::getinstance().print();
      Meyer::getinstance().print(); //All will have same address
}