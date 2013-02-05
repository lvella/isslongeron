#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <GL/gl.h>
#include "issstate.hpp"

struct Vector3
{
  Real x;
  Real y;
  Real z;
};

struct Face
{
  Vector3 verts[3];
};

struct Solid
{
  Solid(int name, std::ifstream &file)
  {
    id = name;
    std::cout << "loading solid " << name << "...";
    std::cout.flush();

    Vector3 max = {-10000000, -10000000, -10000000};
    Vector3 min = {10000000, 10000000, 10000000};

    std::string str;
    file >> str; // facet
    do {
      for(int i = 0; i < 6; ++i) {
	file >> str;
      }

      faces.emplace_back();
      Face &f = faces.back();

      for(int i = 0; i < 3; ++i) {
	file >> str;
	assert(str == "vertex");
	Real x, y, z;
	file >> x >> y >> z;
	f.verts[i].x = x;
	f.verts[i].y = y;
	f.verts[i].z = z;
	//cout << f.verts[i].x << ' ' << f.verts[i].y << ' ' << f.verts[i].z << endl;
	if(x > max.x)
	  max.x = x;
	else if(x < min.x)
	  min.x = x;

	if(y > max.y)
	  max.y = y;
	else if(y < min.y)
	  min.y = y;

	if(z > max.z)
	  max.z = z;
	else if(z < min.z)
	  min.z = z;
      }
      file >> str; // endloop
      file >> str; // endfacet
      file >> str;
    } while(str == "facet");

    assert(str == "endsolid");

    center.x = (min.x + max.x) * 0.5;
    center.y = (min.y + max.y) * 0.5;
    center.z = (min.z + max.z) * 0.5;

    std::cout << "done, center: "
	      << center.x << ' ' << center.y << ' ' << center.z << std::endl;
  }

  void draw() const
  {
    for(auto &f: faces) {
      glBegin(GL_TRIANGLES);
      for(auto &v: f.verts) {
	glVertex3f(v.x, v.y, v.z);
      }
      glEnd();
    }
  }

  void rotate(Real angle, Real x, Real y, Real z) const
  { 
    glTranslatef(center.x, center.y, center.z);
    glRotatef(angle, x, y, z);
    glTranslatef(-center.x, -center.y, -center.z);
  }

  std::vector<Face> faces;
  Vector3 center;
  int id;
};

struct ISSDraw
{
  ISSDraw()
  {
    ploop = 45;
    if(beta > 0)
      sloop = 25;
    else
      sloop = 60;

    std::ifstream file("tester/ISS_simple.stl");

    std::string str;
    file >> str; //solid 
    do {
      solids.emplace_back(solids.size(), file);
      file >> str; //solid or eof
    } while (!file.eof());

    solids[15].center.z = solids[13].center.z;
    solids[22].center.z = solids[7].center.z;
  }

  void draw(int n = -1)
  {
    // Starboard radiator
    glPushMatrix();
    solids[39].rotate(sloop, 1, 0, 0);
    solids[39].draw();
    glPopMatrix();

    // Port radiator:
    glPushMatrix();
    solids[37].rotate(ploop, 1, 0, 0);
    solids[37].draw();
    glPopMatrix();

    // Starboard section
    glPushMatrix();
    solids[15].rotate(state->SARJ[0].a, 0, 1, 0);
    solids[15].draw();
    
    // 1A
    glPushMatrix();
    solids[14].rotate(state->BGA[0].a, 1, 0, 0);
    solids[14].draw();
    glPopMatrix();

    // 3A
    glPushMatrix();
    solids[29].rotate(state->BGA[2].a, 1, 0, 0);
    solids[29].draw();
    glPopMatrix();

    // 1B
    glPushMatrix();
    solids[43].rotate(state->BGA[4].a, 1, 0, 0);
    solids[43].draw();
    glPopMatrix();

    // 3B
    glPushMatrix();
    solids[13].rotate(state->BGA[6].a, 1, 0, 0);
    solids[13].draw();
    glPopMatrix();

    glPopMatrix();

    // Port section
    glPushMatrix();
    solids[22].rotate(state->SARJ[1].a, 0, 1, 0);
    solids[22].draw();
    
    // 2A
    glPushMatrix();
    solids[7].rotate(state->BGA[1].a, 1, 0, 0);
    solids[7].draw();
    glPopMatrix();

    // 4A
    glPushMatrix();
    solids[31].rotate(state->BGA[3].a, 1, 0, 0);
    solids[31].draw();
    glPopMatrix();

    // 2B
    glPushMatrix();
    solids[35].rotate(state->BGA[5].a, 1, 0, 0);
    solids[35].draw();
    glPopMatrix();

    // 4B
    glPushMatrix();
    solids[9].rotate(state->BGA[6].a, 1, 0, 0);
    solids[9].draw();
    glPopMatrix();

    glPopMatrix();
  }

  std::vector<Solid> solids;
  ISSState* state;

  Real ploop;
  Real sloop;
};

void renderer_draw();

extern ISSDraw model;
