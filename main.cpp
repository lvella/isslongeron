#include "genetic.hpp"
#include "display.hpp"
#include "renderer.hpp"

int main()
{
  Individual i;

  beta = 70;
  i.genInitial();
  model.state = &i.state[0];
  
  display_mainloop();
}
