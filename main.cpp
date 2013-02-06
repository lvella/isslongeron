#include "genetic.hpp"
#include "display.hpp"
#include "renderer.hpp"

int main_alt()
{
  Individual ind;
  ind.genInitial();

  display_init();

  std::ifstream fd("current.csv");

  {
    std::string ign;
    fd >> ign;
  }

  ISSDraw model;

  {
    char ign;
    fd >> model.alpha;
    fd >> ign >> beta;
    fd >> ign >> model.yaw;
    fd >> ign >> ind.state[0].SARJ[0].a;
    fd >> ign >> ind.state[0].SARJ[1].a;
    for(int j = 0; j < 8; ++j) {
      fd >> ign >> ind.state[0].BGA[j].a;
    }
  }

  model.state = &ind.state[0];

  while(handle_input())
    frame(model);
}
  

int main(int argc, char *argv[])
{
  beta = -70;

  if(argc >= 2)
    beta = atoi(argv[1]);

  Individual ind;
  ind.genInitial();

  ISSDraw model;
  model.yaw = ind.yaw;

  display_init();

  {
    std::ofstream fd("current.csv");
    fd << "line\n";
    for(int i = 0; i < 92; ++i) {
      fd << model.alpha <<','<< beta <<','<< model.yaw << ','
	 << ind.state[i].SARJ[0] << ',' << ind.state[i].SARJ[1];

      for(int j = 0; j < 8; ++j)
	fd << ',' << ind.state[i].BGA[j];
      fd << '\n';
    }
  }

  int i = 0;
  
  while(handle_input()) {
    model.state = &ind.state[i];
    model.alpha = i * 360.0 / 92.0;
    i = (i+1) % 92;

    frame(model);
  }
}
