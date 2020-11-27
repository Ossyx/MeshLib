/**
* Copyright (C) 2019, Ossyx
*/
#include "Mesh.hxx"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace rx
{

Mesh::Mesh()
{
}

Mesh::Mesh(std::string const& p_name, float* p_vPtr, float* p_nPtr, unsigned int* p_tIdPtr,
  float* p_uvCoords,  float* p_tangentPtr, float* p_bitangentPtr, unsigned int p_vertexCount,
  unsigned int p_normalCount, unsigned int p_tangentCount, unsigned int p_bitangentCount,
  unsigned int p_trianglesCount, unsigned int p_uvCoordsCount):
  m_name(p_name),
  m_vertices(p_vPtr),
  m_normals(p_nPtr),
  m_triangles(p_tIdPtr),
  m_uvCoords(p_uvCoords),
  m_tangents(p_tangentPtr),
  m_bitangents(p_bitangentPtr),
  m_vertexCount(p_vertexCount),
  m_normalCount(p_normalCount),
  m_tangentsCount(p_tangentCount),
  m_bitangentsCount(p_bitangentCount),
  m_trianglesCount(p_trianglesCount),
  m_uvCoordsCount(p_uvCoordsCount)
{
}

Mesh::~Mesh()
{
  if (m_vertices != NULL)
  {
    delete [] m_vertices;
  }
  if (m_triangles != NULL)
  {
    delete [] m_triangles;
  }
  if (m_normals != NULL)
  {
    delete [] m_normals;
  }
  if (m_uvCoords != NULL)
  {
    delete [] m_uvCoords;
  }
  if (m_tangents != NULL)
  {
    delete [] m_tangents;
  }
  if (m_bitangents != NULL)
  {
    delete [] m_bitangents;
  }
}

float const* Mesh::GetVertices() const
{
  return m_vertices;
}

float const* Mesh::GetNormals() const
{
  return m_normals;
}

float const* Mesh::GetTangents() const
{
  return m_tangents;
}

float const* Mesh::GetBitangents() const
{
  return m_bitangents;
}

float const* Mesh::GetUVCoords() const
{
  return m_uvCoords;
}

unsigned int const* Mesh::GetTrianglesIndex() const
{
  return m_triangles;
}

unsigned int Mesh::GetVertexCount() const
{
  return m_vertexCount;
}

unsigned int Mesh::GetTriangleCount() const
{
  return m_trianglesCount;
}

unsigned int Mesh::GetNormalCount() const
{
  return m_normalCount;
}

unsigned int Mesh::GetTangentCount() const
{
  return m_tangentsCount;
}

unsigned int Mesh::GetBitangentCount() const
{
  return m_bitangentsCount;
}

unsigned int Mesh::GetUVCoordsCount() const
{
  return m_uvCoordsCount;
}

bool Mesh::HasNormals() const
{
  return m_normals != NULL;
}

bool Mesh::HasTangents() const
{
  return m_tangents != NULL;
}

bool Mesh::HasBitangents() const
{
  return m_bitangents != NULL;
}

bool Mesh::HasUVCoords() const
{
  return m_uvCoords != NULL;
}

std::string Mesh::GetName() const
{
  return m_name;
}

void Mesh::SetName(const std::string& p_name)
{
  m_name = p_name;
}


}
