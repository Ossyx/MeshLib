#ifndef PRIMITIVES_HXX
#define PRIMITIVES_HXX

#include "Mesh.hxx"
#include "MeshLibEXPORT.hxx"

namespace rx
{

class MeshLib_EXPORT Square : public Mesh
{
public:
  Square();
  virtual ~Square();
};


class MeshLib_EXPORT Cube : public Mesh
{
public:
  Cube();
  virtual ~Cube();
};

class MeshLib_EXPORT Sphere : public Mesh
{
public:
  Sphere();
  virtual ~Sphere();
};

}

#endif
