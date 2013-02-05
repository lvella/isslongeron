#include "renderer.hpp"

using namespace std;

ISSDraw model;

static void
face_sun(Real alpha, Real beta, Real yaw)
{
  glRotatef(yaw, 0, 0, 1);
  glRotatef(-alpha, 0, 1, 0);
  glRotatef(beta, 1, 0, 0);
}

void renderer_draw()
{
  model.draw();
}
