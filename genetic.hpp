#pragma once

#include "issstate.hpp"

struct Individual
{
  void genInitial()
  {
    yaw = rangedist(0.0, 7.0)(rnd);

    state[0].genInitial();

    for(int i = 1; i < 92; ++i)
      state[i].genFromPrev(state[i-1]);
  }

  Real yaw;
  ISSState state[92];
};
