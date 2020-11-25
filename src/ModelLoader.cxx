/**
* Copyright (C) 2019, Ossyx
*/
#include "ModelLoader.hxx"
#include "Material.hxx"

#include <fstream>
#include <algorithm>
#include <vector>
#include <thread>

#include <assimp/material.h>
#include <glm/mat4x4.hpp> // glm::mat4

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace cimg = cimg_library;

namespace rx
{

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

ModelPtr ModelLoader::LoadOBJModel(
  std::filesystem::path const& p_file,
  std::string const& p_name)
{
  unsigned int flag = aiProcess_Triangulate;
  flag |= aiProcess_GenSmoothNormals;
  flag |= aiProcess_JoinIdenticalVertices;
  flag |= aiProcess_CalcTangentSpace;
  
  const aiScene* scene = aiImportFile(p_file.c_str(), flag);

  if (scene == NULL)
  {
    rxLogError("Error : "<< aiGetErrorString());
    return nullptr;
  }

  rxLogInfo("Loading model "<<p_name<<" from file "<< p_file);
  ModelPtr loadedModel = std::make_shared<Model>();
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

  std::filesystem::path jsonFilePath = p_file;
  jsonFilePath.replace_extension(".json");  
  
  if( std::filesystem::exists(jsonFilePath) )
  {
    rxLogInfo("Loading materials from file "<< jsonFilePath);
    auto materials = LoadMaterialCollection(jsonFilePath);
    
    for(MaterialPtr mat : materials)
    {
      loadedModel->AddMaterial(mat, mat->GetName());
    }
  }
  else
  {
    rxLogWarning("No json material file available");
  }
  
  for (int meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx)
  {
    rxLogInfo("-- Loading mesh for model "<<loadedModel->GetName()<<" with "
      <<scene->mMeshes[meshIdx]->mNumVertices<<" vertices "
      <<"and "<<scene->mMeshes[meshIdx]->mNumFaces<<" faces.");
    MeshPtr mesh = LoadFromAiMesh(scene->mMeshes[meshIdx]);
    loadedModel->AddMesh(mesh);
    loadedModel->AddMeshMaterialLink(meshIdx,
      materialMapping[scene->mMeshes[meshIdx]->mMaterialIndex]);
  }

  aiReleaseImport(scene);
  return loadedModel;
}

MeshPtr ModelLoader::LoadFromAiMesh(aiMesh* p_aiMesh)
{
  float* vertexPtr = new float[p_aiMesh->mNumVertices*3];
  unsigned int* triangleIdxPtr = new unsigned int[p_aiMesh->mNumFaces*3];

  float* normalPtr = NULL;
  float* uvCoordsPtr = NULL;
  float* tangentPtr = NULL;
  float* bitangentPtr = NULL;

  if (p_aiMesh->HasNormals() == true)
  {
    rxLogInfo("  Mesh has normals.");
    normalPtr = new float[p_aiMesh->mNumVertices*3];
  }
  if (p_aiMesh->HasTextureCoords(0) == true)
  {
    rxLogInfo("  Mesh has texture coordinates.");
    uvCoordsPtr = new float[p_aiMesh->mNumVertices*2];

    if (p_aiMesh->HasTangentsAndBitangents())
    {
      rxLogInfo("  Mesh has tangents and bitangents.");
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
      rxLogError("Error : Faces with  less than 3 indices not supported.");
      return nullptr;
    }
    if (p_aiMesh->mFaces->mNumIndices > 3)
    {
      rxLogError("Error : Faces with more than 3 indices not supported.");
      return nullptr;
    }
    aiFace const& assimpFace = p_aiMesh->mFaces[numTri];
    triangleIdxPtr[idx + 0] = assimpFace.mIndices[0];
    triangleIdxPtr[idx + 1] = assimpFace.mIndices[1];
    triangleIdxPtr[idx + 2] = assimpFace.mIndices[2];
    idx += 3;
  }
  
  auto m = std::make_shared<Mesh>(vertexPtr, normalPtr, triangleIdxPtr, uvCoordsPtr, tangentPtr,
    bitangentPtr,p_aiMesh->mNumVertices, p_aiMesh->mNumVertices, p_aiMesh->mNumVertices,
    p_aiMesh->mNumVertices, p_aiMesh->mNumFaces, p_aiMesh->mNumVertices);

  return m;
}

std::vector<MaterialPtr> 
ModelLoader::LoadMaterialCollection(std::filesystem::path const& p_jsonFile)
{
  std::ifstream inputStreamJsonMap;
  inputStreamJsonMap.open(p_jsonFile.c_str());
  if (inputStreamJsonMap.is_open() == false)
  {
    rxLogError("Cannot open json material file " << p_jsonFile);
    assert(false);
  }

  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(inputStreamJsonMap, root );
  if (parsingSuccessful == false)
  {
      // report to the user the failure and their locations in the document.
    rxLogError("Fail to parse material file : "<<reader.getFormattedErrorMessages());
    assert(false);
  }
  
  std::vector<MaterialPtr> result;

  //Each thread needs material key, root data and p_model and a mutex for p_model
  std::mutex res_mutex;
  std::vector<std::thread> matThreads;
  std::vector<std::string> const& materialKeys = root.getMemberNames();
  for (int i = 0; i < materialKeys.size(); ++i)
  {
    std::string materialKey = materialKeys[i];
    auto func = [&res_mutex, materialKey, &root, &result, &p_jsonFile]
      {
        MaterialPtr mat = LoadMaterial(root[materialKey], materialKey, p_jsonFile.parent_path());
        res_mutex.lock();
        result.push_back(mat);
        res_mutex.unlock();
      };
    matThreads.push_back(std::thread(func));
  }

  for (int i = 0; i < matThreads.size(); ++i)
  {
    matThreads[i].join();
  }
  
  return result;
}

MaterialPtr ModelLoader::LoadMaterial(Json::Value& pDescription, std::string const& pKey, std::filesystem::path const& pDirectory)
{
  MaterialPtr mat = std::make_shared<Material>();
  mat->SetName(pKey);
  mat->SetDirectory(pDirectory);
  Json::Value matAttributes = pDescription["Attributes"];
  Json::Value matShader = pDescription["Shader"];

  mat->SetShaderName(matShader["name"].asString());
  for (int attIdx = 0; attIdx < matAttributes.size(); ++attIdx)
  {
    LoadJsonAttribute(*mat, matAttributes[attIdx]);
  }
  return mat;
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
    else if (attType == "UShortTex")
    {
      AttributeAsUShortTexture(p_material, p_value);
    }
    else if (attType == "FloatTex")
    {
      AttributeAsFloatTexture(p_material, p_value);
    }
    else
    {
      rxLogError("Unsupported attribute type : "<< attType);
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
  rxLogInfo("Loading "<<name <<" ("<<value<<")");
}

void ModelLoader::AttributeAsVec3(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  glm::vec3 value(p_value["value"][0].asFloat(),
    p_value["value"][1].asFloat(),
    p_value["value"][2].asFloat());

  p_material.SetData(name, value);
  rxLogInfo("Loading "<<name <<" ("<<value.r<<","<<value.g<<","<<value.b<<")");
}

void ModelLoader::AttributeAsByteTexture(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  std::filesystem::path value = p_value["value"].asString();
  rxLogInfo("Loading "<< name <<" texture "<< value);

  Texture<unsigned char>& tex = p_material.AddByteTexData(name);
  LoadTextureFromFile<unsigned char>(p_material.GetDirectory() / value, tex);
}

void ModelLoader::AttributeAsUShortTexture(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  std::filesystem::path value = p_value["value"].asString();
  rxLogInfo("Loading "<< name <<" texture "<< value);

  Texture<unsigned short>& tex = p_material.AddUShortTexData(name);
  LoadTextureFromFile<unsigned short>(p_material.GetDirectory() / value, tex);
}

void ModelLoader::AttributeAsFloatTexture(Material& p_material, Json::Value const& p_value)
{
  std::string name = p_value["name"].asString();
  std::filesystem::path value = p_value["value"].asString();
  bool yinvert = false;
  if( p_value.isMember("yinvert") )
  {
    yinvert = p_value["yinvert"].asBool();
  }
  rxLogInfo("Loading "<< name <<" texture "<< value);

  Texture<float>& tex = p_material.AddFloatTexData(name);
  LoadTextureFromFile<float>(p_material.GetDirectory() / value, tex, yinvert);
}

template <typename T>
void ModelLoader::LoadTextureFromFile(std::filesystem::path const& p_path,
  Texture<T>& p_texture, bool pYinvert)
{
  cimg::CImg<T> image(p_path.c_str());  
  if ( pYinvert )
  {
    image.mirror('y');
  }
  rxLogInfo("Loading "<<p_path<<" with spectrum " << image.spectrum());
//   cimg::CImgDisplay main_disp(image,"Click a point");
//   while (!main_disp.is_closed()) {
//     main_disp.wait();
//   }
  unsigned int pixelCount = image.width() * image.height();
  if (image.spectrum() == 1)//B&W
  {
    p_texture.Initialize(p_path, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data());
  }
  else if (image.spectrum() == 2)//RG
  {
    p_texture.Initialize(p_path, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data(), image.data(pixelCount));
  }
  else if (image.spectrum() == 3)//RGB
  {
    p_texture.Initialize(p_path, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data(), image.data(pixelCount), image.data(2*pixelCount));
  }
  else if(image.spectrum() == 4)//RGBA
  {
    p_texture.Initialize(p_path, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data(), image.data(pixelCount),
     image.data(2*pixelCount), image.data(3*pixelCount));
  }
}

}
