#ifndef GLSLTYPESTORE_HXX
#define GLSLTYPESTORE_HXX

#include <unordered_map>
#include <string>
#include <set>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "Common.hxx"

namespace rx
{

class GLSLTypeStore
{
public:
  
  using MemberSet = std::set<std::string>;
  GLSLTypeStore();
  virtual ~GLSLTypeStore();
  
  template <typename T>
  bool Has(std::string const& pKey) const;
   
  template <typename T>
  T Get(std::string const& pKey) const;
  
  template <typename T>
  void Set(std::string const& pKey, T pValue);
  
  MemberSet const& GetMembers() const;
  
protected:
  
  std::unordered_map<std::string, int> mInts;
  std::unordered_map<std::string, float> mFloats;
  std::unordered_map<std::string, glm::vec2> mVec2s;
  std::unordered_map<std::string, glm::vec3> mVec3s;
  std::unordered_map<std::string, glm::vec4> mVec4s;
  std::unordered_map<std::string, glm::mat4> mMat4s;
  MemberSet mMembers;
};

template <typename T>
bool GLSLTypeStore::Has(std::string const& pKey) const
{
  return mMembers.find(pKey) != mMembers.end();
}

}
#endif
