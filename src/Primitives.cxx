#include "Primitives.hxx"

namespace rx
{

Square::Square()
{
  m_vertices = new float[4*3];
  m_triangles = new unsigned[2*3];
  m_normals = new float[4*3];

  m_uvCoords = NULL;
  m_tangents = NULL;
  m_bitangents = NULL;

  m_vertexCount = 4;
  m_normalCount = 4;
  m_tangentsCount = 0;
  m_bitangentsCount = 0;
  m_trianglesCount = 2;
  m_uvCoordsCount = 0;

  m_vertices[0]=-0.5f; m_vertices[1]=0.0f; m_vertices[2]=-0.5f;
  m_vertices[3]=-0.5f; m_vertices[4]=0.0f; m_vertices[5]= 0.5f;
  m_vertices[6]=0.5f; m_vertices[7]=0.0f; m_vertices[8]=0.5f;
  m_vertices[9]=0.5f; m_vertices[10]=0.0f; m_vertices[11]=-0.5f;

  for(unsigned i=0; i < 4; ++i)
  {
    m_normals[i*3]=0.0f; m_normals[i*3+1]=1.0f; m_normals[i*3+2]=0.0f;
  }

  m_triangles[0]=0;m_triangles[1]=1;m_triangles[2]=3;
  m_triangles[3]=3;m_triangles[4]=1;m_triangles[5]=2;
}

Square::~Square()
{
}

Cube::Cube()
{
  //Create a unit cube with 24 vertex (4*6faces)
  m_vertices = new float[24*3];
  m_triangles = new unsigned[3*12];
  m_normals = new float[24*3];

  m_uvCoords = NULL;
  m_tangents = NULL;
  m_bitangents = NULL;

  m_vertexCount = 24;
  m_normalCount = 24;
  m_tangentsCount = 0;
  m_bitangentsCount = 0;
  m_trianglesCount = 12;
  m_uvCoordsCount = 0;

  //Front
  m_vertices[0]=-0.5; m_vertices[1]= -0.5; m_vertices[2]= 0.5f;
  m_vertices[3]=0.5; m_vertices[4]= -0.5; m_vertices[5]= 0.5f;
  m_vertices[6]=0.5; m_vertices[7]= 0.5; m_vertices[8]= 0.5f;
  m_vertices[9]=-0.5; m_vertices[10] = 0.5; m_vertices[11]= 0.5f;

  m_triangles[0]=0;m_triangles[1]=1;m_triangles[2]=2;
  m_triangles[3]=0;m_triangles[4]=2;m_triangles[5]=3;

  for(unsigned i=0; i < 4; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]= 0.0;m_normals[i*3+2]= 1.0;
  }

  //Back
  m_vertices[12]=-0.5; m_vertices[13]= -0.5; m_vertices[14]=-0.5f;
  m_vertices[15]=0.5; m_vertices[16]= -0.5; m_vertices[17]=-0.5f;
  m_vertices[18]=0.5; m_vertices[19]= 0.5; m_vertices[20]=-0.5f;
  m_vertices[21]=-0.5; m_vertices[22] = 0.5; m_vertices[23]=-0.5f;

  m_triangles[6]=6;m_triangles[7]=5;m_triangles[8]=4;
  m_triangles[9]=6;m_triangles[10]=4;m_triangles[11]=7;

  for(unsigned i=4; i < 8; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]= 0.0;m_normals[i*3+2]=-1.0;
  }

  //Top
  m_vertices[24]=-0.5; m_vertices[25]=0.5; m_vertices[26]=0.5;
  m_vertices[27]=0.5; m_vertices[28]=0.5; m_vertices[29]=0.5;
  m_vertices[30]=0.5; m_vertices[31]=0.5; m_vertices[32]=-0.5;
  m_vertices[33]=-0.5; m_vertices[34]=0.5; m_vertices[35]=-0.5;

  m_triangles[12]=8;m_triangles[13]=9;m_triangles[14]=10;
  m_triangles[15]=8;m_triangles[16]=10;m_triangles[17]=11;

  for(unsigned i=8; i < 12; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]=1.0;m_normals[i*3+2]=0.0;
  }

  //Bottom
  m_vertices[36]=-0.5; m_vertices[37]=-0.5; m_vertices[38]=0.5;
  m_vertices[39]=0.5; m_vertices[40]=-0.5; m_vertices[41]=0.5;
  m_vertices[42]=0.5; m_vertices[43]=-0.5; m_vertices[44]=-0.5;
  m_vertices[45]=-0.5; m_vertices[46]=-0.5; m_vertices[47]=-0.5;

  m_triangles[18]=14;m_triangles[19]=13;m_triangles[20]=12;
  m_triangles[21]=14;m_triangles[22]=12;m_triangles[23]=15;

  for(unsigned i=12; i < 16; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]=-1.0;m_normals[i*3+2]=0.0;
  }

  //Left
  m_vertices[48]=-0.5; m_vertices[49]=0.5; m_vertices[50]=-0.5;
  m_vertices[51]=-0.5; m_vertices[52]=-0.5; m_vertices[53]=-0.5;
  m_vertices[54]=-0.5; m_vertices[55]=-0.5; m_vertices[56]=0.5;
  m_vertices[57]=-0.5; m_vertices[58]=0.5; m_vertices[59]=0.5;

  m_triangles[24]=16;m_triangles[25]=17;m_triangles[26]=18;
  m_triangles[27]=16;m_triangles[28]=18;m_triangles[29]=19;

  for(unsigned i=16; i < 20; ++i)
  {
    m_normals[i*3]=-1.0;m_normals[i*3+1]=0.0;m_normals[i*3+2]=0.0;
  }

  //Right
  m_vertices[60]=0.5; m_vertices[61]=0.5; m_vertices[62]=-0.5;
  m_vertices[63]=0.5; m_vertices[64]=-0.5; m_vertices[65]=-0.5;
  m_vertices[66]=0.5; m_vertices[67]=-0.5; m_vertices[68]=0.5;
  m_vertices[69]=0.5; m_vertices[70]=0.5; m_vertices[71]=0.5;

  m_triangles[30]=20;m_triangles[31]=23;m_triangles[32]=22;
  m_triangles[33]=20;m_triangles[34]=22;m_triangles[35]=21;

  for(unsigned i=20; i < 24; ++i)
  {
    m_normals[i*3]=1.0;m_normals[i*3+1]=0.0;m_normals[i*3+2]=0.0;
  }
}

Cube::~Cube()
{
}

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}


}
