/**
* Copyright (C) 2019, Ossyx
*/
#include "Model.hxx"

Model::Model()
{
}

Model::~Model()
{
  //Destroy the m_meshes
  for (unsigned int i=0; i < m_meshes.size(); ++i)
  {
    delete m_meshes[i];
  }
}

void Model::SetName(std::string const& p_name)
{
  m_name = p_name;
}

void Model::AddMesh(Mesh* p_mesh)
{
  m_meshes.push_back(p_mesh);
}

void Model::AddMaterial(Material* p_material, std::string const& p_name)
{
  m_materials[p_name] = p_material;
}

unsigned int Model::GetMeshCount()
{
  return m_meshes.size();
}

Mesh* Model::GetMesh(unsigned int p_idx)
{
  assert(p_idx < m_meshes.size());
  return m_meshes[p_idx];
}

Material* Model::GetMaterial(std::string const& p_name)
{
  assert(m_materials.find(p_name) != m_materials.end());
  return m_materials[p_name];
}

void Model::AddMeshMaterialLink(unsigned int p_meshIdx, std::string const& p_materialName)
{
  m_meshToMaterialLink[p_meshIdx] = p_materialName;
}

Material* Model::GetMaterialForMesh(unsigned int p_meshId)
{
  assert(p_meshId < m_meshes.size());
  UintStringMap::iterator it = m_meshToMaterialLink.find(p_meshId);
  if (it != m_meshToMaterialLink.end())
  {
    MaterialMap::iterator itMat = m_materials.find(it->second);
    if (itMat != m_materials.end())
    {
      return m_materials[it->second];
    }
  }
  MeshLibLog("No material for mesh "<< p_meshId);
  assert(false);
  return NULL;
}

std::string Model::GetName()
{
  return m_name;
}
