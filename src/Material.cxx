/**
* Copyright (C) 2019, Ossyx
*/
#include "Material.hxx"
namespace rx
{

Material::Material():
m_name("")
{
}

Material::~Material()
{
}

std::string Material::GetName() const
{
  return m_name;
}

void Material::SetName(std::string const& p_name)
{
  m_name = p_name;
}

void Material::SetData(std::string const& p_name, float p_data)
{
  m_floats[p_name] = p_data;
}

void Material::SetData(std::string const& p_name, glm::vec3 const& p_data)
{
  m_vec3s[p_name] = p_data;
}

void Material::SetData(std::string const& p_name, Material::ByteTexture const& p_data)
{
  m_ucharTextures[p_name] = p_data;
}

void Material::SetData(std::string const& p_name, Material::FloatTexture const& p_data)
{
  m_floatTextures[p_name] = p_data;
}

void Material::SetData(std::string const& p_name, Material::UShortTexture const& p_data)
{
  m_ushortTextures[p_name] = p_data;
}

void Material::SetUniformData(std::string const& p_uniform, std::string const& p_attributeKey)
{
  m_uniforms[p_uniform] = p_attributeKey;
}


void Material::SetShaderName(std::string const& p_name)
{
  m_shaderName = p_name;
}

bool Material::GetData(std::string const& p_name, float& p_data) const
{
  FloatMap::const_iterator it = m_floats.find(p_name);
  if (it != m_floats.cend())
  {
    p_data = it->second;
    return true;
  }
  return false;
}

bool Material::GetData(std::string const& p_name, glm::vec3& p_data) const
{
  Vec3Map::const_iterator it = m_vec3s.find(p_name);
  if (it != m_vec3s.cend())
  {
    p_data = it->second;
    return true;
  }
  return false;
}

Material::ByteTexture const& Material::GetByteTexture(std::string const& p_name) const
{
  ByteTextureMap::const_iterator it = m_ucharTextures.find(p_name);
  assert(it != m_ucharTextures.cend());
  return it->second;
}

Material::FloatTexture const& Material::GetFloatTexture(std::string const& p_name) const
{
  FloatTextureMap::const_iterator it = m_floatTextures.find(p_name);
  assert(it != m_floatTextures.cend());
  return it->second;
}

Material::UShortTexture const& Material::GetUShortTexture(std::string const& p_name) const
{
  UShortTextureMap::const_iterator it = m_ushortTextures.find(p_name);
  assert(it != m_ushortTextures.cend());
  return it->second;
}

bool Material::HasFloatData(std::string const& p_name) const
{
  return m_floats.find(p_name) != m_floats.end();
}
bool Material::HasVec3Data(std::string const& p_name) const
{
  return m_vec3s.find(p_name) != m_vec3s.end();
}
bool Material::HasUCharTexData(std::string const& p_name) const
{
  ByteTextureMap::const_iterator it = m_ucharTextures.find(p_name);
  return it != m_ucharTextures.cend();
}
bool Material::HasFloatTexData(std::string const& p_name) const
{
  return m_floatTextures.find(p_name) != m_floatTextures.end();
}

bool Material::HasUShortTexData(std::string const& p_name) const
{
  return m_ushortTextures.find(p_name) != m_ushortTextures.end();
}

bool Material::GetUniformData(std::string const& p_uniform, std::string& p_attributeKey) const
{
  StringMap::const_iterator it = m_uniforms.find(p_uniform);
  if (it != m_uniforms.cend())
  {
    p_attributeKey = it->second;
    return true;
  }
  return false;
}

Material::StringMap const& Material::GetUniforms() const
{
  return m_uniforms;
}

std::string Material::GetShaderName() const
{
  return m_shaderName;
}

Material::ByteTexture& Material::AddByteTexData(std::string const& p_name)
{
  ByteTextureMap::const_iterator it = m_ucharTextures.find(p_name);
  assert(it == m_ucharTextures.cend());
  return m_ucharTextures[p_name];
}

Material::FloatTexture& Material::AddFloatTexData(std::string const& p_name)
{
  FloatTextureMap::const_iterator it = m_floatTextures.find(p_name);
  assert(it == m_floatTextures.cend());
  return m_floatTextures[p_name];
}

Material::UShortTexture& Material::AddUShortTexData(std::string const& p_name)
{
  UShortTextureMap::const_iterator it = m_ushortTextures.find(p_name);
  assert(it == m_ushortTextures.cend());
  return m_ushortTextures[p_name];
}

}
