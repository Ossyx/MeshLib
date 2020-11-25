#include "Primitives.hxx"
#include <cstring>

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
  m_uvCoords = new float[24*2];

  m_tangents = NULL;
  m_bitangents = NULL;

  m_vertexCount = 24;
  m_normalCount = 24;
  m_tangentsCount = 0;
  m_bitangentsCount = 0;
  m_trianglesCount = 12;
  m_uvCoordsCount = 16;
  
  float v1[3] = { -1.0f, -1.0f, 1.0f };
  float v2[3] = { 1.0f, -1.0f, 1.0f };
  float v3[3] = { 1.0f, 1.0f, 1.0f };
  float v4[3] = { -1.0f, 1.0f, 1.0f };
  float v5[3] = { -1.0f, -1.0f, -1.0f };
  float v6[3] = { 1.0f, -1.0f, -1.0f };
  float v7[3] = { 1.0f, 1.0f, -1.0f };
  float v8[3] = { -1.0f, 1.0f, -1.0f };  
  float uv[8] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    
  size_t vSize = 3 * sizeof(float);
  size_t uvSize = 8 * sizeof(float);
  
  //Front
  std::memcpy( m_vertices + 0, v1, vSize);
  std::memcpy( m_vertices + 3, v2, vSize);
  std::memcpy( m_vertices + 6, v3, vSize);
  std::memcpy( m_vertices + 9, v4, vSize);  
  std::memcpy( m_uvCoords + 0, uv, uvSize);

  m_triangles[0]=0;m_triangles[1]=1;m_triangles[2]=2;
  m_triangles[3]=0;m_triangles[4]=2;m_triangles[5]=3;

  for(unsigned i=0; i < 4; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]= 0.0;m_normals[i*3+2]= 1.0;
  }

  //Back
  std::memcpy( m_vertices + 12, v6, vSize);
  std::memcpy( m_vertices + 15, v5, vSize);
  std::memcpy( m_vertices + 18, v8, vSize);
  std::memcpy( m_vertices + 21, v7, vSize);  
  std::memcpy( m_uvCoords + 8, uv, uvSize);

  m_triangles[6]=6;m_triangles[7]=5;m_triangles[8]=4;
  m_triangles[9]=6;m_triangles[10]=4;m_triangles[11]=7;

  for(unsigned i=4; i < 8; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]= 0.0;m_normals[i*3+2]=-1.0;
  }

  //Top
  std::memcpy( m_vertices + 24, v4, vSize);
  std::memcpy( m_vertices + 27, v3, vSize);
  std::memcpy( m_vertices + 30, v7, vSize);
  std::memcpy( m_vertices + 33, v8, vSize);  
  std::memcpy( m_uvCoords + 16, uv, uvSize);

  m_triangles[12]=8;m_triangles[13]=9;m_triangles[14]=10;
  m_triangles[15]=8;m_triangles[16]=10;m_triangles[17]=11;

  for(unsigned i=8; i < 12; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]=1.0;m_normals[i*3+2]=0.0;
  }

  //Bottom
  std::memcpy( m_vertices + 36, v5, vSize);
  std::memcpy( m_vertices + 39, v6, vSize);
  std::memcpy( m_vertices + 42, v2, vSize);
  std::memcpy( m_vertices + 45, v1, vSize);  
  std::memcpy( m_uvCoords + 24, uv, uvSize);

  m_triangles[18]=14;m_triangles[19]=13;m_triangles[20]=12;
  m_triangles[21]=14;m_triangles[22]=12;m_triangles[23]=15;

  for(unsigned i=12; i < 16; ++i)
  {
    m_normals[i*3]= 0.0;m_normals[i*3+1]=-1.0;m_normals[i*3+2]=0.0;
  }

  //Left
  std::memcpy( m_vertices + 48, v5, vSize);
  std::memcpy( m_vertices + 51, v1, vSize);
  std::memcpy( m_vertices + 54, v4, vSize);
  std::memcpy( m_vertices + 57, v8, vSize);  
  std::memcpy( m_uvCoords + 32, uv, uvSize);

  m_triangles[24]=16;m_triangles[25]=17;m_triangles[26]=18;
  m_triangles[27]=16;m_triangles[28]=18;m_triangles[29]=19;

  for(unsigned i=16; i < 20; ++i)
  {
    m_normals[i*3]=-1.0;m_normals[i*3+1]=0.0;m_normals[i*3+2]=0.0;
  }

  //Right
  std::memcpy( m_vertices + 60, v2, vSize);
  std::memcpy( m_vertices + 63, v6, vSize);
  std::memcpy( m_vertices + 66, v7, vSize);
  std::memcpy( m_vertices + 69, v3, vSize);
  std::memcpy( m_uvCoords + 40, uv, uvSize);

  m_triangles[30]=20;m_triangles[31]=23;m_triangles[32]=22;
  m_triangles[33]=20;m_triangles[34]=22;m_triangles[35]=21;

  for(unsigned i=20; i < 24; ++i)
  {
    m_normals[i*3]=1.0;m_normals[i*3+1]=0.0;m_normals[i*3+2]=0.0;
  }
}

Cube::~Cube()
{
  delete[] m_vertices;
  delete[] m_triangles;
  delete[] m_normals;
  delete[] m_uvCoords;
}

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}


}
