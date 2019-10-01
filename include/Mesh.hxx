#ifndef MESH_HXX
#define MESH_HXX

#include <vector>
#include <string>

#include "Common.hxx"

class Mesh
{
public:
  Mesh(float* p_vPtr, float* p_nPtr, unsigned int* p_tIdPtr, float* p_uvCoords,
       float* p_tangentPtr, float* p_bitangentPtr, unsigned int p_vertexCount,
       unsigned int p_normalCount, unsigned int p_tangentCount, unsigned int p_bitangentCount,
       unsigned int p_trianglesCount, unsigned int p_uvCoordsCount);
  
  ~Mesh();
  
  float const* GetVertices() const;  
  float const* GetNormals() const;
  float const* GetTangents() const;
  float const* GetBitangents() const;
  unsigned int const* GetTrianglesIndex() const;  
  float const* GetUVCoords() const;
  
  unsigned int GetVertexCount() const;  
  unsigned int GetTriangleCount() const;  
  unsigned int GetNormalCount() const;
  unsigned int GetTangentCount() const;
  unsigned int GetBitangentCount() const;
  unsigned int GetUVCoordsCount() const;
  
  bool HasNormals() const;  
  bool HasUVCoords() const;
  bool HasTangents() const;
  bool HasBitangents() const;
  
private:
  unsigned int* m_triangles;  
  float* m_vertices;
  float* m_normals;
  float* m_uvCoords;
  float* m_tangents;
  float* m_bitangents;
  
  unsigned int m_vertexCount;
  unsigned int m_normalCount;
  unsigned int m_tangentsCount;
  unsigned int m_bitangentsCount;
  unsigned int m_trianglesCount;
  unsigned int m_uvCoordsCount;
};

#endif
