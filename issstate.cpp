#include <random>
#include "issstate.hpp"

std::mt19937 rnd(std::random_device{}());
Real beta;

std::ostream&
operator<<(std::ostream& o, const Angular& a) {
  o << a.a; //<< ',' << a.v;
  return o;
}

std::ostream&
operator<<(std::ostream& o, const ISSState& s) {
  o << s.SARJ[0] << ',' << s.SARJ[1];

  for(int i = 0; i < 8; ++i)
    o << ',' << s.BGA[i];

  return o;
}
