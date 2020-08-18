#include <cstring>
#include <iostream>
#include <sstream>
#include "ThreadPool.h"

std::string foo(int x, int y){
  auto myid = std::this_thread::get_id();
  std::stringstream ss;
  ss << myid;
  std::string str = "THREAD ID:"+ ss.str() +" TEST Exec func with sum args:"+std::to_string(x+y)+  "IN WORK\n";
  std::cout<<str;
  std::string buffer = "RESULT THREAD ID"+ss.str()+ "(TEST Exec func with args):"+std::to_string(x)+ std::to_string(y);
  return buffer;
}


void test(){
  std::cout<<"TEST Exec Func"<<std::endl;
  ThreadPool pool(4);
  std::vector<std::future<std::string>> res_threads;

  for (int i = 0; i < 16; ++i) {
    auto lambda = [i]() {
      auto myid = std::this_thread::get_id();
      std::stringstream ss;
      ss << myid;
      std::string str = "THREAD ID:"+ ss.str() +" NUMBER:"+std::to_string(i)+" IN WORK\n";
      std::cout<<str;
      std::this_thread::sleep_for(std::chrono::seconds(2));
      return "THREAD ID:"+ss.str()+" END WORK";
    };
    auto res_thread = pool.Exec(std::move(lambda));
    res_threads.emplace_back(std::move(res_thread));
  }
  for (auto& res : res_threads) {
    std::cout << res.get() <<"<-RESULT FUTURES"<< "\n";
  }

  std::cout<<"\n\n\nTEST Exec Func with args"<<std::endl;
  res_threads.clear();
  int a=1;
  int b =100;
  auto res = pool.Exec(foo, a, b);
  res_threads.emplace_back(std::move(res));
  for (auto& res : res_threads) {
    std::cout << res.get() <<"<-RESULT FUTURES"<< "\n";
  }
  std::cout<<"\n\n\nAll tests passed"<<std::endl;

}


int main(int argc, char* argv[]) {
  switch (argc) {
    case 1:
      break;
    case 2:
      if (!strcmp(argv[1], "test"))
        test();
      break;
    default:
      std::cerr << "Incorrect arguments" << std::endl;
      return 1;
  }
  return 0;
}