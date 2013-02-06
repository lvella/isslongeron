#pragma once

#include <stdexcept>
#include <map>
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
      }
      file >> str; // endloop
      file >> str; // endfacet
      file >> str;
    } while(str == "facet");

    assert(str == "endsolid");

    static const std::map<int, Vector3> axis_map =
      { {15, { 5 , 0 , 11 }}, // ssarj
	{22, { 5 , 0 , 11 }}, // psarj
	{14, { 0 ,  33350 , -750 }}, // 1A
	{7, { 0 , -33350 , -750 }},  // 2A
	{29, { 0 ,  33350 ,  750 }}, // 3A
	{31, {0 , -33350 ,  750}},   // 4A 
	{43, { 0 ,  48422 ,  755 }},  // 1B 
	{35, { 0 , -48420 ,  683 }},  // 2B
	{13, { 0 ,  48413 , -750 }},  // 3B
	{9, { 0 , -48419 , -817 }},  // 4B
	{37, { -250 , -14600 , -5.5}}, // Port radiator
	{39, { -250 , 14694 , -23 }} // Starboard radiator
      };
    try {
      center = axis_map.at(name);
    } catch (std::out_of_range e) {
    }

    std::cout << "done!" << std::endl;
  }

  void draw() const
  {
    for(auto &f: faces) {
      glColor3ub(id*3+70, id*3+70, id*3+70);
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
    std::ifstream file("tester/ISS_simple.stl");

    std::string str;
    file >> str; //solid 
    do {
      solids.emplace_back(solids.size(), file);
      file >> str; //solid or eof
    } while (!file.eof());

    solids[15].center.z = solids[13].center.z;
    solids[22].center.z = solids[7].center.z;

    const Vector3 axis[] = { { -250 , -14600 , -5.5 },
			     { -250 , 14694 , -23 }};
    solids[37].center = axis[0];
    solids[39].center = axis[1];
  }

  void draw(int n = -1)
  {
    // Face sun
    glRotatef(beta, 1,0,0); // Checked!
    glRotatef(alpha, 0,1,0); // Checked!
    glRotatef(yaw, 0,0,1);

    // Starboard radiator
    glPushMatrix();
    solids[39].rotate(beta > 0 ? 25 : 60, 1, 0, 0);
    solids[39].draw();
    glPopMatrix();

    // Port radiator:
    glPushMatrix();
    solids[37].rotate(45, 1, 0, 0);
    solids[37].draw();
    glPopMatrix();

    // Starboard section
    glPushMatrix();
    solids[15].rotate(state->SARJ[0].a, 0, -1, 0); // Checked!
    solids[15].draw();
    
    // 1A
    glPushMatrix();
    solids[14].rotate(state->BGA[0].a + 90, 1, 0, 0); // Checked!
    solids[14].draw();
    glPopMatrix();

    // 3A
    glPushMatrix();
    solids[29].rotate(state->BGA[2].a + 90, 1, 0, 0); // Checked!
    solids[29].draw();
    glPopMatrix();

    // 1B
    glPushMatrix();
    solids[43].rotate(state->BGA[4].a + 90, 1, 0, 0); // Checked!
    solids[43].draw();
    glPopMatrix();

    // 3B
    glPushMatrix();
    solids[13].rotate(state->BGA[6].a + 90, 1, 0, 0); // Checked!
    solids[13].draw();
    glPopMatrix();

    glPopMatrix();

    // Port section
    glPushMatrix();
    solids[22].rotate(state->SARJ[1].a, 0, 1, 0); // Checked!
    solids[22].draw();
    
    // 2A
    glPushMatrix();
    solids[7].rotate(state->BGA[1].a - 90, 1, 0, 0);
    solids[7].draw();
    glPopMatrix();

    // 4A
    glPushMatrix();
    solids[31].rotate(state->BGA[3].a - 90, 1, 0, 0);
    solids[31].draw();
    glPopMatrix();

    // 2B
    glPushMatrix();
    solids[35].rotate(state->BGA[5].a - 90, 1, 0, 0);
    solids[35].draw();
    glPopMatrix();

    // 4B
    glPushMatrix();
    solids[9].rotate(state->BGA[6].a - 90, 1, 0, 0);
    solids[9].draw();
    glPopMatrix();

    glPopMatrix();

    // Static body
    std::vector<bool> drawn(solids.size(), false);
    drawn[7] = drawn[9] = drawn[13] = drawn[14]
      = drawn[15] = drawn[22] = drawn[29] = drawn[31]
      = drawn[35] = drawn[37] = drawn[39] = drawn[43]
      = true;
    for(auto &s : solids)
      if(!drawn[s.id])
	s.draw();
  }

  std::vector<Solid> solids;
  ISSState* state;

  Real alpha;
  Real yaw;
};
