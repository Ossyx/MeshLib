/**
* Copyright (C) 2019, Ossyx
*/
#ifndef MATERIAL_HXX
#define MATERIAL_HXX

#include <cstring>
#include <string>
#include <unordered_map>
#include <map>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

#include "Common.hxx"

namespace rx
{

template <typename T>
struct Texture
{
  Texture();

  ~Texture();

  Texture(Texture<T> const& p_other);

  Texture<T>& operator=(Texture<T> const& p_other);

  //B&W Textures
  void Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_data);

  //RGB Textures
  void Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_dataR, T* p_dataG, T* p_dataB);

  //RGBA Textures
  void Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_dataR, T* p_dataG, T* p_dataB, T* p_dataA);

  std::string m_filepath;
  unsigned int m_height;
  unsigned int m_width;
  unsigned int m_channelCount;
  T* m_data;
};

//Template implementation of texture data structure
template <typename T>
Texture<T>::Texture():
  m_filepath(""),
  m_width(0),
  m_height(0),
  m_channelCount(0),
  m_data(NULL)
{
}

template <typename T>
Texture<T>::~Texture()
{
  if (m_data != NULL)
  {
    delete [] m_data;
  }
}

template <typename T>
Texture<T>& Texture<T>::operator=(Texture<T> const& p_other)
{
  //Operator do a shallow copy
  this->m_filepath = p_other.m_filepath;
  this->m_width = p_other.m_width;
  this->m_height = p_other.m_height;
  this->m_channelCount = p_other.m_channelCount;

  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = p_other.m_width*p_other.m_height*p_other.m_channelCount;
  m_data = new T[size];
  std::memcpy(this->m_data, p_other.m_data, size*sizeof(T));
  return *this;
}

template <typename T>
void Texture<T>::Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_data)
{
  this->m_filepath = p_filepath;
  this->m_width = p_width;
  this->m_height = p_height;
  this->m_channelCount = 1;
  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = m_width*m_height*m_channelCount;
  m_data = new T[size];

  for (int i = 0; i < m_width*m_height; ++i)
  {
    m_data[i] = p_data[i];
  }
}

template <typename T>
void Texture<T>::Initialize(std::string const& p_filepath,
  unsigned int p_width, unsigned int p_height,
  T* p_dataR, T* p_dataG, T* p_dataB)
{
  this->m_filepath = p_filepath;
  this->m_width = p_width;
  this->m_height = p_height;
  this->m_channelCount = 3;
  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = m_width*m_height*m_channelCount;
  m_data = new T[size];

  //Interleave RGB data
  for (int i = 0; i < m_width*m_height; ++i)
  {
    m_data[i*m_channelCount] = p_dataR[i];
    m_data[i*m_channelCount + 1] = p_dataG[i];
    m_data[i*m_channelCount + 2] = p_dataB[i];
  }
}

template <typename T>
void Texture<T>::Initialize(std::string const& p_filepath,
  unsigned int p_width, unsigned int p_height,
  T* p_dataR, T* p_dataG, T* p_dataB, T* p_dataA)
{
  this->m_filepath = p_filepath;
  this->m_width = p_width;
  this->m_height = p_height;
  this->m_channelCount = 4;
  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = m_width*m_height*m_channelCount;
  m_data = new T[size];

  //Interleave RGBA data
  for (int i = 0; i < m_width*m_height; ++i)
  {
    m_data[i*m_channelCount] = p_dataR[i];
    m_data[i*m_channelCount + 1] = p_dataG[i];
    m_data[i*m_channelCount + 2] = p_dataB[i];
    m_data[i*m_channelCount + 3] = p_dataA[i];
  }
}

template <typename T>
Texture<T>::Texture(Texture<T> const& p_other)
{
  *this = p_other;
}

enum DataType
{
  eFloat,
  eVec3,
  eUCharTexture,
  eFloatTexture
};

class Material
{
public:

  Material();

  ~Material();

  typedef Texture<unsigned char> ByteTexture;
  typedef Texture<float> FloatTexture;
  typedef std::unordered_map<std::string, std::string> StringMap;

  void SetName(std::string const& p_name);

  void SetData(std::string const& p_name, float p_data);

  void SetData(std::string const& p_name, glm::vec3 const& p_data);

  //Create a deep copy of the texture from p_data
  void SetData(std::string const& p_name, ByteTexture const& p_data);

  //Create a deep copy of the texture
  void SetData(std::string const& p_name, FloatTexture const& p_data);

  //Create a ByteTexture and return a reference to it
  ByteTexture& AddByteTexData(std::string const& p_name);

  //Create a FloatTexture and return a reference to it
  FloatTexture& AddFloatTexData(std::string const& p_name);

  void SetUniformData(std::string const& p_uniform, std::string const& p_attributeKey);

  void SetShaderName(std::string const& p_name);

  std::string GetName() const;

  bool GetData(std::string const& p_name, float& p_data) const;

  bool GetData(std::string const& p_name, glm::vec3& p_data) const;

  bool GetData(std::string const& p_name, glm::vec4& p_data) const;

  ByteTexture const& GetByteTexture(std::string const& p_name) const;

  FloatTexture const& GetFloatTexture(std::string const& p_name) const;

  bool GetUniformData(std::string const& p_uniform, std::string& p_attributeKey) const;

  StringMap const& GetUniforms() const;

  std::string GetShaderName() const;

  bool HasFloatData(std::string const& p_name) const;

  bool HasVec3Data(std::string const& p_name) const;

  bool HasUCharTexData(std::string const& p_name) const;

  bool HasFloatTexData(std::string const& p_name) const;

private:

  std::string m_name;

  typedef std::map<std::string, ByteTexture>  ByteTextureMap;
  ByteTextureMap m_ucharTextures;

  typedef std::unordered_map<std::string, FloatTexture>  FloatTextureMap;
  FloatTextureMap m_floatTextures;

  typedef std::unordered_map<std::string, float>  FloatMap;
  FloatMap m_floats;

  typedef std::unordered_map<std::string, glm::vec3>  Vec3Map;
  Vec3Map m_vec3s;

  StringMap m_uniforms;

  std::string m_shaderName;
};

}
#endif
