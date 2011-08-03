#include <testutility.hpp>
#include <fixedqueue.hpp>

BOOST_AUTO_TEST_CASE(FIXED_QUEUE){

  fixed_queue<int> q(6);
  
  for(int i = 0; i < 10; ++i){
    q.push_back(i);
  }
  
  for(fixed_queue<int>::iterator i = q.begin();
      i != q.end();
      ++i){
    std::cout << *i << std::endl;
  }

  std::cout <<q[1] << std::endl;
  std::cout <<q.reverse_at(1) << std::endl;
  for(fixed_queue<int>::reverse_iterator i = q.rbegin();
      i != q.rend();
      ++i){
    std::cout << *i << std::endl;
  }

  
}
