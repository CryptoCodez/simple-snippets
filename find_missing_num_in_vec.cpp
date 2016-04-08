// Copyright (c) Marc Puttkammer
// Use -std=c++11 To Compile

// C++ Std Header Files
//
#include <vector>
#include <iostream>

int missing( std::vector<int>& vec ) {
  auto r = [&vec]{ int r_; for( auto n : vec ) r_ += n; return r_ / ( vec.size() - 1 ) ; };
  return r();
}

int main() {
  std::vector<int> a { 0, 1, 2, 4, 5 };
  std::cout << missing( a );
  return 0;
}
