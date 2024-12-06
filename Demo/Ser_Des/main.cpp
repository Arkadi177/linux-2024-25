#include <iostream>
void test1();
void test2();
void test3();
// void test4();

int main(){
  try{
  test1();
  std::cout <<std::endl;
  test2();
  std::cout <<std::endl;
  test3();
  std::cout <<std::endl;
  // test4(); // if this comment all works good because this is not_copiable test
  }catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}
