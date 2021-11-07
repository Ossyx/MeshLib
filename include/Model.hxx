/**
* Copyright (C) 2019, Ossyx
*/
#ifndef MODEL_HXX
#define MODEL_HXX

#include <vector>
#include <map>

#include "Common.hxx"
#include "MeshLibEXPORT.hxx"
#include "Mesh.hxx"
#include "Material.hxx"

namespace rx
{

class MeshLib_EXPORT Model
{
public:
  Model();

  ~Model();
  
  void SetName(std::string const& p_name);

  void AddMesh(MeshPtr p_mesh);

  void AddMaterial(MaterialPtr p_material, std::string const& p_name);

  void AddMeshMaterialLink(unsigned int p_meshId, std::string const& p_materialName);

  unsigned int GetMeshCount();

  MeshPtr GetMesh(unsigned int p_idx);

  MaterialPtr GetMaterial(std::string const& p_name);

  MaterialPtr GetMaterialForMesh(unsigned int p_meshId);

  std::string GetName();


private:

  //Disable copy
  Model(Model const& p_other);

  std::string m_name;

  unsigned int m_meshCount;

  std::vector<MeshPtr> m_meshes;

  typedef std::map<std::string, MaterialPtr> MaterialMap;
  MaterialMap m_materials;

  typedef std::map<unsigned int, std::string> UintStringMap;
  UintStringMap m_meshToMaterialLink;
};

using ModelPtr = std::shared_ptr<Model>;

}

#endif
