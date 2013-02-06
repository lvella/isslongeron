#include "genetic.hpp"
#include "display.hpp"
#include "renderer.hpp"

int main()
{
  Individual ind;

  beta = -70;
  ind.genInitial();
  model.yaw = ind.yaw;

  display_init();

  int i = 0;
  while(handle_input()) {
    model.state = &ind.state[i];
    model.alpha = i * 360.0 / 92.0;
    if(++i == 92)
      i = 0;

    frame();
  }
}
