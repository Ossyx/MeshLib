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
#include <filesystem>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#define cimg_use_png
#define cimg_use_jpeg
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

  static ModelPtr LoadOBJModel(std::filesystem::path const& p_file,
    std::string const& p_name);

  static void ComputeNormalMap(cimg_library::CImg<unsigned char> const& p_bumpMap,
    Texture<unsigned char>& p_normalMap);
 
  static std::vector<MaterialPtr> LoadMaterialCollection(std::filesystem::path const& p_jsonFile);
  
  static MaterialPtr LoadMaterial(Json::Value& pDescription, std::string const& pKey, std::filesystem::path const& pDirectory);

private:

  ModelLoader(ModelLoader const&);

  static MeshPtr LoadFromAiMesh(aiMesh* p_aiMesh);

  static void LoadJsonAttribute(Material& p_material, Json::Value const& p_value);

  static void AttributeAsFloat(Material& p_material, Json::Value const& p_value);

  static void AttributeAsVec3(Material& p_material, Json::Value const& p_value);
  
  template <typename T>
  static void AttributeAsTexture(Material& p_material, Json::Value const& p_value);
};

template <typename T>
void ModelLoader::AttributeAsTexture(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  std::filesystem::path value = p_value["value"].asString();
  bool yinvert = true;
  if( p_value.isMember("yinvert") )
  {
    yinvert = p_value["yinvert"].asBool();
  }
  rxLogInfo("Loading "<< name <<" texture "<< value);

  auto tex = TextureFactory::Create<T>(p_material.GetDirectory() / value, yinvert);
  p_material.SetData(name, tex);
}

}

#endif
