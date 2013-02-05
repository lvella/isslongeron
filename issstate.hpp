#pragma once

#include <iostream>
#include <random>
#include <algorithm>

typedef float Real;

typedef std::uniform_real_distribution<Real> rangedist;

extern std::mt19937 rnd;
extern Real beta;

struct Angular
{
  void genFromPrev(const Angular &prev, Real maxvel, Real maxaccel)
  {
    Real t0 = std::min<Real>(60.0, (-maxvel - prev.v) / -maxaccel);
    Real t1 = std::min<Real>(60.0, (maxvel - prev.v) / maxaccel);

    rangedist vel(prev.v - maxaccel * t0, prev.v + maxaccel * t1);
    rangedist angle
      (prev.a + vel.min() * (60.0 - t0) + prev.v * t0 - maxaccel * 0.5 * t0 * t0,
       prev.a + vel.max() * (60.0 - t1) + prev.v * t1 + maxaccel * 0.5 * t1 * t1);

    
    std::cerr << t0 << ' ' << t1 << ' ' << prev.v << ' ' << vel.min() << ' ' << vel.max() << '\n';

    v = vel(rnd);
    a = angle(rnd);
    while(a > 360.0)
      a -= 360.0;
    while(a < 0.0)
      a += 360.0;
  }

  Real a; // Angle
  Real v; // Velocity
};

std::ostream& operator<<(std::ostream& o, const Angular& a);

struct ISSState
{
  void genInitial()
  {
    rangedist angle(0, 360);
    rangedist srajvel(-0.15, 0.15);
    rangedist bgavel(-0.25, 0.25);

    for(auto &e : SARJ) {
      e.a = angle(rnd);
      e.v = srajvel(rnd);
    }

    for(auto &e : BGA) {
      e.a = angle(rnd);
      e.v = bgavel(rnd);
    }
  }

  void genFromPrev(const ISSState &prev)
  {
    for(int i = 0; i < 2; ++i)
      SARJ[i].genFromPrev(prev.SARJ[i], 0.15, 0.005);

    for(int i = 0; i < 8; ++i)
      BGA[i].genFromPrev(prev.BGA[i], 0.25, 0.01);
  }

  Angular SARJ[2]; // 0: starboard, 1: port
  Angular BGA[8]; // A: 0..3, b: 4..7
};

std::ostream& operator<<(std::ostream& o, const ISSState& s);
