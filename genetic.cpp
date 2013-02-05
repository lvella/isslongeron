#include <iostream>
#include <random>
#include <algorithm>
#include "genetic.hpp"

std::ostream&
operator<<(std::ostream& o, const Individual& s) {
  for(auto &e : s.state)
    o << "0.0," << beta << ',' << s.yaw << ',' << e << '\n';

  return o;
}
