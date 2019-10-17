/**
* Copyright (C) 2019, Ossyx
*/
#ifndef MODELLOADER_HXX
#define MODELLOADER_HXX

#include <vector>
#include <map>
#include <string>
#include <thread>
#include <mutex>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#include <CImg.h>

#include "Mesh.hxx"
#include "Material.hxx"
#include "Model.hxx"
#include "Common.hxx"

namespace rx
{

class ModelLoader
{
public:

  ModelLoader();

  ~ModelLoader();

  void LoadOBJModel(std::string const& p_directory, std::string const& p_file,
    std::string const& p_name);

  Model* FindModel(std::string const& p_meshName);

  static void ComputeNormalMap(cimg_library::CImg<unsigned char> const& p_bumpMap,
    Texture<unsigned char>& p_normalMap);

private:

  ModelLoader(ModelLoader const&);

  void LoadFromAiMesh(Model& p_model, aiMesh* p_aiMesh);

  //Json loader
  void LoadFromJsonMaterial(Model& p_model, std::string const& p_directory,
  std::string const& p_objFile);

  void LoadJsonAttribute(Material& p_material, Json::Value const& p_value);

  void AttributeAsFloat(Material& p_material, Json::Value const& p_value);

  void AttributeAsVec3(Material& p_material, Json::Value const& p_value);

  void AttributeAsByteTexture(Material& p_material, Json::Value const& p_value);

  void AttributeAsUShortTexture(Material& p_material, Json::Value const& p_value);

  template <typename T>
  void LoadTextureFromFile(std::string const& p_directory, std::string const& p_fileName,
    Texture<T>& p_texture);

  bool FindAndGetJsonMaterialFile(std::string const& p_objFilePath,
    std::ifstream& p_inputStream);

  typedef std::map<std::string, Model*> ModelMap;
  ModelMap m_modelMap;

  std::string m_directory;
};

}

#endif
