//Using shared mutex
#pragma once
#include "utils.hpp"

std::map<std::string,int> teleBook{{"Dijkstra", 1972}, {"Scott", 1976}, 
                                   {"Ritchie", 1983}}; //This will be the shared hashmap

std::shared_timed_mutex teleBookMutex; //Locks will pass around this mutex while accessing teleBook

void writeToteleBook(const std::string& name,int number){
    std::lock_guard<std::shared_timed_mutex> writerLock(teleBookMutex);
    std::cout<<"\nAdding......";
    teleBook[name]=number;
    std::cout<<".......done\n";
}

void readFromTeleBook(const std::string& name){
    std::shared_lock<std::shared_timed_mutex> readerLock(teleBookMutex);
    std::cout<<name<<" : "<<teleBook[name]<<"\n";
}


int main(){

  std::cout << std::endl;

  std::thread reader1([]{ readFromTeleBook("Scott"); });
  std::thread reader2([]{ readFromTeleBook("Ritchie"); });
  std::thread w1([]{ writeToteleBook("Scott",1968); });
  std::thread reader3([]{ readFromTeleBook("Dijkstra"); });
  std::thread reader4([]{ readFromTeleBook("Scott"); });
  std::thread w2([]{ writeToteleBook("Bjarne",1965); });
  std::thread reader5([]{ readFromTeleBook("Scott"); });
  std::thread reader6([]{ readFromTeleBook("Ritchie"); });
  std::thread reader7([]{ readFromTeleBook("Scott"); });
  std::thread reader8([]{ readFromTeleBook("Bjarne"); });

  reader1.join();
  reader2.join();
  reader3.join();
  reader4.join();
  reader5.join();
  reader6.join();
  reader7.join();
  reader8.join();
  w1.join();
  w2.join();

  std::cout << std::endl;

  std::cout << "\nThe new telephone book" << std::endl;
  for (auto teleIt: teleBook){
    std::cout << teleIt.first << ": " << teleIt.second << std::endl;
  }

  std::cout << std::endl;

}