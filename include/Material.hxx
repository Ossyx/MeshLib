/**
* Copyright (C) 2019, Ossyx
*/
#ifndef MATERIAL_HXX
#define MATERIAL_HXX

#include <cstring>
#include <string>
#include <unordered_map>
#include <map>
#include <filesystem>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

#include "MeshLibEXPORT.hxx"
#include "Common.hxx"
#include "GLSLTypeStore.hxx"
#include "Texture.hxx"

namespace rx
{

class MeshLib_EXPORT Material : public GLSLTypeStore
{
public:

  Material();

  virtual ~Material();

  typedef std::shared_ptr<Texture<unsigned char>> ByteTexture;
  typedef std::shared_ptr<Texture<float>> FloatTexture;
  typedef std::shared_ptr<Texture<unsigned short>> UShortTexture;
  typedef std::unordered_map<std::string, std::string> StringMap;

  void SetName(std::string const& p_name);
  
  void SetDirectory(std::filesystem::path const& p_path);   

  void SetData(std::string const& p_name, ByteTexture p_data);

  void SetData(std::string const& p_name, FloatTexture p_data);

  void SetData(std::string const& p_name, UShortTexture p_data);

  std::string GetName() const;
  
  std::filesystem::path GetDirectory() const;
  
  template <typename T>
  std::shared_ptr<Texture<T>> GetTextureData(std::string const& pName) const;
  
  template <typename T>
  bool HasTextureData(std::string const& pName) const;

private:

  std::string m_name;
  
  std::filesystem::path m_directory;

  typedef std::map<std::string, ByteTexture>  ByteTextureMap;
  ByteTextureMap m_ucharTextures;

  typedef std::unordered_map<std::string, FloatTexture>  FloatTextureMap;
  FloatTextureMap m_floatTextures;

  typedef std::unordered_map<std::string, UShortTexture>  UShortTextureMap;
  UShortTextureMap m_ushortTextures;
};

using MaterialPtr = std::shared_ptr<Material>;

}
#endif
