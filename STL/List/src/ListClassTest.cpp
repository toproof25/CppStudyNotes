#include <iostream>
#include "../inc/List.hpp"

List<int> l;

void Test01_push_front()
{
 l.push_front(100); 
 l.printList();

 l.push_front(200); 
 l.printList();

 l.push_front(300); 
 l.printList();

 printf("\n\n");
}

void Test02_push_back()
{
 l.push_back(900); 
 l.printList();

 l.push_back(800); 
 l.printList();

 l.push_back(700); 
 l.printList();

 printf("\n\n");
}

void Test03_pop_front()
{
 l.pop_front(); 
 l.printList();

 l.pop_front(); 
 l.printList();

 l.pop_front(); 
 l.printList();

 printf("\n\n");
}
void Test04_pop_back()
{
 l.pop_back(); 
 l.printList();

 l.pop_back(); 
 l.printList();

 l.pop_back(); 
 l.printList();

 printf("\n\n");
}

int main()
{
  Test01_push_front();
  Test02_push_back();
  Test03_pop_front();
  Test04_pop_back();

  
  
  

  return 0;
}