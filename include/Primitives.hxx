#ifndef PRIMITIVES_HXX
#define PRIMITIVES_HXX

#include "Mesh.hxx"

namespace rx
{

class Square : public Mesh
{
public:
  Square();
  virtual ~Square();
};


class Cube : public Mesh
{
public:
  Cube();
  virtual ~Cube();
};

class Sphere : public Mesh
{
public:
  Sphere();
  virtual ~Sphere();
};

}

#endif
