/**
* Copyright (C) 2019, Ossyx
*/
#include "ModelLoader.hxx"
#include "Material.hxx"

#include <fstream>
#include <algorithm>

#include <CImg.h>
#include <assimp/material.h>
#include <glm/mat4x4.hpp> // glm::mat4

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace cimg = cimg_library;

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
  ModelMap::iterator it = m_modelMap.begin();
  for (; it != m_modelMap.end(); ++it)
  {
    MeshLibLog("Unloading model "<<it->first<<"...");
    delete it->second;
  }
}

void ModelLoader::LoadOBJModel(
  std::string const& p_directory,
  std::string const& p_file,
  std::string const& p_name)
{
  unsigned int flag = aiProcess_Triangulate;
  flag |= aiProcess_GenSmoothNormals;
  flag |= aiProcess_JoinIdenticalVertices;
  flag |= aiProcess_CalcTangentSpace;

  m_directory = p_directory;
  std::string filepath = m_directory;
  filepath += "/";
  filepath += p_file;
  const aiScene* scene = aiImportFile(filepath.c_str(), flag);

  if (scene == false)
  {
    MeshLibLog("Error : "<< aiGetErrorString());
    return;
  }

  MeshLibLog("Loading model "<<p_name<<" from file "<< filepath);
  Model* loadedModel = new Model();
  loadedModel->SetName(p_name);

  std::map<unsigned int, std::string> materialMapping;
  if (scene->HasMaterials())
  {
    for (unsigned int materialIdx = 0; materialIdx < scene->mNumMaterials; ++materialIdx)
    {
      aiString name;
      scene->mMaterials[materialIdx]->Get(AI_MATKEY_NAME, name);
      materialMapping[materialIdx] = std::string(name.C_Str());
    }
  }

  LoadFromJsonMaterial(*loadedModel, p_directory, filepath);

  for (int meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx)
  {
    LoadFromAiMesh(*loadedModel, scene->mMeshes[meshIdx]);
    loadedModel->AddMeshMaterialLink(meshIdx,
      materialMapping[scene->mMeshes[meshIdx]->mMaterialIndex]);
  }

  m_modelMap[p_name] = loadedModel;

  //Computing the bounding box
  aiReleaseImport(scene);
}

void ModelLoader::LoadFromAiMesh(Model& p_model, aiMesh* p_aiMesh)
{
  MeshLibLog("-- Loading mesh for model "<<p_model.GetName()<<" with "
    <<p_aiMesh->mNumVertices<<" vertices "
    <<"and "<<p_aiMesh->mNumFaces<<" faces.");

  float* vertexPtr = new float[p_aiMesh->mNumVertices*3];
  unsigned int* triangleIdxPtr = new unsigned int[p_aiMesh->mNumFaces*3];

  float* normalPtr = NULL;
  float* uvCoordsPtr = NULL;
  float* tangentPtr = NULL;
  float* bitangentPtr = NULL;

  if (p_aiMesh->HasNormals() == true)
  {
    MeshLibLog("  Mesh has normals.");
    normalPtr = new float[p_aiMesh->mNumVertices*3];
  }
  if (p_aiMesh->HasTextureCoords(0) == true)
  {
    MeshLibLog("  Mesh has texture coordinates.");
    uvCoordsPtr = new float[p_aiMesh->mNumVertices*2];

    if (p_aiMesh->HasTangentsAndBitangents())
    {
      MeshLibLog("  Mesh has tangents and bitangents.");
      tangentPtr = new float[p_aiMesh->mNumVertices*3];
      bitangentPtr = new float[p_aiMesh->mNumVertices*3];
    }
  }

  //Load vertex, normals and textures coords
  unsigned int idxVN = 0;
  unsigned int idxUV = 0;
  for (int numVert = 0; numVert < p_aiMesh->mNumVertices; numVert++)
  {
    aiVector3D const& assimpVert = p_aiMesh->mVertices[numVert];
    vertexPtr[idxVN + 0] = assimpVert.x;
    vertexPtr[idxVN + 1]  = assimpVert.y;
    vertexPtr[idxVN + 2]  = assimpVert.z;

    if (normalPtr != NULL)
    {
      aiVector3D const& assimpNorm = p_aiMesh->mNormals[numVert];
      normalPtr[idxVN + 0] = assimpNorm.x;
      normalPtr[idxVN + 1] = assimpNorm.y;
      normalPtr[idxVN + 2] = assimpNorm.z;
    }
    if (uvCoordsPtr != NULL)
    {
      aiVector3D const& assimpUVW = p_aiMesh->mTextureCoords[0][numVert];
      uvCoordsPtr[idxUV + 0] = assimpUVW.x;
      uvCoordsPtr[idxUV + 1] = assimpUVW.y;
    }
    if (tangentPtr != NULL)
    {
      aiVector3D const& assimpTan = p_aiMesh->mTangents[numVert];
      tangentPtr[idxVN + 0] = assimpTan.x;
      tangentPtr[idxVN + 1] = assimpTan.y;
      tangentPtr[idxVN + 2] = assimpTan.z;
    }
    if (bitangentPtr != NULL)
    {
      aiVector3D const& assimpBitan = p_aiMesh->mBitangents[numVert];
      bitangentPtr[idxVN + 0] = assimpBitan.x;
      bitangentPtr[idxVN + 1] = assimpBitan.y;
      bitangentPtr[idxVN + 2] = assimpBitan.z;
    }
    idxVN += 3;
    idxUV += 2;
  }

  //Load triangles index
  unsigned int idx = 0;
  for (int numTri = 0; numTri < p_aiMesh->mNumFaces; ++numTri)
  {
    if (p_aiMesh->mFaces->mNumIndices < 3)
    {
      MeshLibLog("Error : Faces with  less than 3 indices not supported.");
      return;
    }
    if (p_aiMesh->mFaces->mNumIndices > 3)
    {
      MeshLibLog("Error : Faces with more than 3 indices not supported.");
      return;
    }
    aiFace const& assimpFace = p_aiMesh->mFaces[numTri];
    triangleIdxPtr[idx + 0] = assimpFace.mIndices[0];
    triangleIdxPtr[idx + 1] = assimpFace.mIndices[1];
    triangleIdxPtr[idx + 2] = assimpFace.mIndices[2];
    idx += 3;
  }

  //Load

  Mesh* m = new Mesh(vertexPtr, normalPtr, triangleIdxPtr, uvCoordsPtr, tangentPtr,
    bitangentPtr,p_aiMesh->mNumVertices, p_aiMesh->mNumVertices, p_aiMesh->mNumVertices,
    p_aiMesh->mNumVertices, p_aiMesh->mNumFaces, p_aiMesh->mNumVertices);

  p_model.AddMesh(m);
  return;
}

void ModelLoader::LoadFromJsonMaterial(Model& p_model, std::string const& p_directory,
  std::string const& p_objFile)
{
  std::string jsonMatFile = p_objFile;
  size_t index;
  index = jsonMatFile.find(".obj");
  assert(index != std::string::npos);
  jsonMatFile.replace(index, 4, ".json");

  std::ifstream input;
  input.open(jsonMatFile.c_str());
  if (input.is_open())
  {
    MeshLibLog("Reading material file "<<jsonMatFile);
  }
  else
  {
    MeshLibLog("Cannot find json material file "<<jsonMatFile);
    return;
  }

  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(input, root );
  if (parsingSuccessful == false)
  {
      // report to the user the failure and their locations in the document.
    MeshLibLog("Fail to parse material file : "<<reader.getFormattedErrorMessages());
    return;
  }

  std::vector<std::string> const& materialKeys = root.getMemberNames();
  for (int i = 0; i < materialKeys.size(); ++i)
  {
    MeshLibLog("Reading material " << materialKeys[i]);
    Material* mat = new Material();
    mat->SetName(materialKeys[i]);
    Json::Value matAttributes = root[materialKeys[i]]["Attributes"];
    Json::Value matShader = root[materialKeys[i]]["Shader"];

    mat->SetShaderName(matShader["name"].asString());
    for (int attIdx = 0; attIdx < matAttributes.size(); ++attIdx)
    {
      LoadJsonAttribute(*mat, matAttributes[attIdx]);
    }
    p_model.AddMaterial(mat, materialKeys[i]);
  }
}

void ModelLoader::LoadJsonAttribute(Material& p_material, Json::Value const& p_value)
{
  if (p_value.isMember("type"))
  {
    //set float as the default value to return
    std::string attType = p_value["type"].asString();
    if (attType == "float")
    {
      AttributeAsFloat(p_material, p_value);
    }
    else if (attType == "vec3")
    {
      AttributeAsVec3(p_material, p_value);
    }
    else if (attType == "UCharTex")
    {
      AttributeAsByteTexture(p_material, p_value);
    }
    else
    {
      MeshLibLog("Unsupported attribute type : "<<attType);
      assert(false);
    }
  }

  if (p_value.isMember("uniform"))
  {
    p_material.SetUniformData(p_value["uniform"].asString(), p_value["name"].asString());
  }
}

void ModelLoader::AttributeAsFloat(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  float value = p_value["value"].asFloat();
  p_material.SetData(name, value);
  MeshLibLog("Loading "<<name <<" ("<<value<<")");
}

void ModelLoader::AttributeAsVec3(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  glm::vec3 value(p_value["value"][0].asFloat(),
    p_value["value"][1].asFloat(),
    p_value["value"][2].asFloat());

  p_material.SetData(name, value);
  MeshLibLog("Loading "<<name <<" ("<<value.r<<","<<value.g<<","<<value.b<<")");
}

void ModelLoader::AttributeAsByteTexture(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  std::string value = p_value["value"].asString();;
  MeshLibLog("Loading "<< name <<" texture "<< value);

  Texture<unsigned char> tex;
  LoadTextureFromFile(m_directory, value, tex);
  p_material.SetData(name, tex);

  //Special case of bump map, generating normalmap
  if (name == "map_bump")
  {
    MeshLibLog("Generating a normal map from texture "<< value);
    Texture<unsigned char> texNormalMap;
    LoadAndComputeNormalMap(m_directory, value, texNormalMap);
    p_material.SetData("normalMap", texNormalMap);
    p_material.SetUniformData("normalMap", "normalMap");
  }
}

void ModelLoader::LoadFromAiMaterial(Model& p_model, aiMaterial* p_aiMaterial)
{
  aiString name;
  p_aiMaterial->Get(AI_MATKEY_NAME, name);
  MeshLibLog("-- Loading material "<<name.C_Str());
  Material* mat = new Material();
  LoadColors(p_aiMaterial, *mat);
  LoadTextures(p_aiMaterial, *mat);
  p_model.AddMaterial(mat, name.C_Str());
}

void ModelLoader::LoadColors(aiMaterial* p_aiMaterial, Material& p_mat)
{
  auto LoadColorType = [p_aiMaterial, this](const char* p_Key, unsigned int p_type,
    unsigned int p_idx, std::string const& p_parameterName, Material& p_oMat)
  {
    aiColor3D color (0.f,0.f,0.f);
    if (p_aiMaterial->Get(p_Key, p_type, p_idx, color) == AI_SUCCESS)
    {
      MeshLibLog("Loading "<<p_parameterName <<"("<<color.r<<","<<color.g<<","<<color.b<<")");
      p_oMat.SetData(p_parameterName, glm::vec3(color.b, color.g, color.r));
    }
  };

  LoadColorType(AI_MATKEY_COLOR_AMBIENT, "Ambient", p_mat);
  LoadColorType(AI_MATKEY_COLOR_DIFFUSE, "Diffuse", p_mat);
  LoadColorType(AI_MATKEY_COLOR_SPECULAR, "Specular", p_mat);
  LoadColorType(AI_MATKEY_COLOR_EMISSIVE, "Emissive", p_mat);
  LoadColorType(AI_MATKEY_COLOR_TRANSPARENT, "Transparent", p_mat);
  LoadColorType(AI_MATKEY_COLOR_REFLECTIVE, "Reflective", p_mat);
}

void ModelLoader::LoadTextures(aiMaterial* p_aiMaterial, Material& p_mat)
{
  auto LoadTextureType = [p_aiMaterial, this](aiTextureType p_type,
    std::string const& p_parameterName, Material& p_oMat )
  {
    aiString texturePath;
    if (p_aiMaterial->GetTextureCount(p_type) > 0
    && p_aiMaterial->GetTexture(p_type, 0, &texturePath) == AI_SUCCESS)
    {
      //TODO Do not assume textures will only be rgb
      MeshLibLog("Loading "<< p_parameterName <<" texture "<<texturePath.C_Str());
      Texture<unsigned char> tex;
      LoadTextureFromFile(m_directory, texturePath.C_Str(), tex);
      p_oMat.SetData(p_parameterName, tex);
    }
  };

  LoadTextureType(aiTextureType_AMBIENT, "Ambient", p_mat);
  LoadTextureType(aiTextureType_DIFFUSE, "Diffuse", p_mat);
  LoadTextureType(aiTextureType_SPECULAR, "Specular", p_mat);
  LoadTextureType(aiTextureType_EMISSIVE, "Emissive", p_mat);
  LoadTextureType(aiTextureType_HEIGHT, "Height", p_mat);
  LoadTextureType(aiTextureType_NORMALS, "Normals", p_mat);
  LoadTextureType(aiTextureType_SHININESS, "Shininess", p_mat);
  LoadTextureType(aiTextureType_OPACITY, "Opacity", p_mat);
  LoadTextureType(aiTextureType_DISPLACEMENT, "Displacement", p_mat);
  LoadTextureType(aiTextureType_LIGHTMAP, "Lightmap", p_mat);
  LoadTextureType(aiTextureType_REFLECTION, "Reflection", p_mat);
}

void ModelLoader::LoadTextureFromFile(std::string const& p_directory, std::string const& p_fileName,
  Texture<unsigned char>& p_texture)
{
  std::string path = m_directory + "/" + p_fileName;
  std::replace( path.begin(), path.end(), '\\', '/');
  cimg::CImg<unsigned char> image(path.c_str());
  image.mirror('y');
  unsigned int pixelCount = image.width() * image.height();
  assert(image.spectrum() == 1 || image.spectrum() == 3);
  if (image.spectrum() == 1)
  {
    p_texture.Initialize(path, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data());
  }
  else if (image.spectrum() == 3)
  {
    p_texture.Initialize(path, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data(), image.data(pixelCount), image.data(2*pixelCount));
  }
}

void ModelLoader::LoadAndComputeNormalMap(std::string const& p_directory,
  std::string const& p_fileName, Texture<unsigned char>& p_texture)
{
  std::string path = m_directory + "/" + p_fileName;
  std::replace( path.begin(), path.end(), '\\', '/');
  cimg::CImg<unsigned char> image(path.c_str());
  image.mirror('y');
  assert(image.spectrum() == 1);
  if (image.spectrum() == 1)
  {
    ComputeNormalMap(image, p_texture);
  }
}

Model* ModelLoader::FindModel(std::string const& p_name)
{
  ModelMap::iterator it = m_modelMap.find(p_name);
  if (it != m_modelMap.end())
  {
    return it->second;
  }
  else
  {
    return NULL;
  }
}

void ModelLoader::ComputeNormalMap(cimg::CImg<unsigned char>const& p_bumpMap,
  Texture<unsigned char>& p_normalMap)
{
  cimg::CImgList<float> gradients = p_bumpMap.get_gradient("xy", 1);
  cimg::CImg<float>& xgradient = gradients.front();
  cimg::CImg<float>& ygradient = gradients.back();

  //Output is an rgb image
  cimg::CImg<unsigned char> normalMap(xgradient.width(), xgradient.height(), 1, 3, 0);

  //[-1.0, 1.0] --> [0, 255]
  auto MapFloatToChar = [](float p_float)
  {
    return static_cast<unsigned char>((p_float + 1.0f) * 255 / 2.0f);
  };

  for (int i=0; i < normalMap.width(); ++i)
  {
    for (unsigned j=0; j < normalMap.height(); ++j)
    {
      float& xgrad = xgradient(i, j);
      float& ygrad = ygradient(i, j);

      unsigned char& x = normalMap(i, j, 0);
      unsigned char& y = normalMap(i, j, 1);
      unsigned char& z = normalMap(i, j, 2);

      glm::vec3 normal(0.0f, 0.0f, 1.0f);

      auto rotYMat = glm::rotate(xgrad / 255.0f * 15.0f, glm::vec3(0.0, 1.0, 0.0));
      auto rotXMat = glm::rotate(ygrad / 255.0f * 15.0f, glm::vec3(1.0, 0.0, 0.0));

      auto rotMap = rotYMat * rotXMat;
      normal = glm::vec3(rotMap * glm::vec4(normal, 1.0f));

      x = MapFloatToChar(normal.x);
      y = MapFloatToChar(normal.y);
      z = MapFloatToChar(normal.z);
    }
  }

//   cimg::CImgDisplay main_disp(normalMap,"Click a point");
//   while (!main_disp.is_closed())
//   {
//     main_disp.wait();
//   }

  unsigned int pixelCount = normalMap.width() * normalMap.height();
  p_normalMap.Initialize("GeneratedNormalMap",
    static_cast<unsigned int>(normalMap.width()),
    static_cast<unsigned int>(normalMap.height()),
    normalMap.data(), normalMap.data(pixelCount),
    normalMap.data(2*pixelCount));
}
