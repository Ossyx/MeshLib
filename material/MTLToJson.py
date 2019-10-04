import os
import argparse
import json
import image_processing

parser = argparse.ArgumentParser()
parser.add_argument("inputMtl")
parser.add_argument("outputJson")
args = parser.parse_args()

mtl_filepath = args.inputMtl
output_file_path = args.outputJson

class Material:
  def __init__(self, name, attributeUniformMap):
    self.name = name
    self.floatAttributes = dict()
    self.vec3Attributes = dict()
    self.textureAttributes = dict()
    self.attributeUniformMap = attributeUniformMap;

  def __repr__(self):
    return str(self)
  def __str__(self):
    stringRep = self.name
    stringRep += '\n'
    stringRep += str(self.floatAttributes)
    stringRep += '\n'
    stringRep += str(self.vec3Attributes)
    stringRep += '\n'
    stringRep += str(self.textureAttributes)
    return stringRep

  def ToDict(self):

    materialObject = dict()
    materialObject["Shader"] = dict()

    if "map_Kd" in self.textureAttributes :
      if "map_bump" in self.textureAttributes :
        materialObject["Shader"]["name"] = "NormalMapped"
      else:
        materialObject["Shader"]["name"] = "Textured"
    else:
      materialObject["Shader"]["name"] = "Colored"

    if "map_bump" in self.textureAttributes :
      #Generate the matching normal map name
      bumpfilename, file_extension = os.path.splitext(self.textureAttributes["map_bump"])
      normalFileName = bumpfilename + "_generated_normals" + file_extension
      self.textureAttributes["map_normal"] = normalFileName

      #Get bump and normal map real path
      directory = os.path.dirname(os.path.abspath(mtl_filepath))
      properBumpFilePath = directory + "/" + bumpfilename.replace("\\", "/") + file_extension
      properNormalFilePath = directory + "/" + normalFileName.replace("\\", "/")
      image_processing.GenerateNormalMap(properBumpFilePath,  properNormalFilePath)


    materialObject["Attributes"] = list()

    for att in self.floatAttributes:
      if att in self.attributeUniformMap:
        cAttribute = dict()
        cAttribute["name"] = att
        cAttribute["type"] = "float"
        cAttribute["value"] = self.floatAttributes[att]
        cAttribute["uniform"] = self.attributeUniformMap[att]
        materialObject["Attributes"].append(cAttribute)

    for att in self.vec3Attributes:
      if att in self.attributeUniformMap:
        cAttribute = dict()
        cAttribute["name"] = att
        cAttribute["type"] = "vec3"
        cAttribute["value"] = self.vec3Attributes[att]
        cAttribute["uniform"] = self.attributeUniformMap[att]
        materialObject["Attributes"].append(cAttribute)

    for att in self.textureAttributes:
      if att in self.attributeUniformMap:
        cAttribute = dict()
        cAttribute["name"] = att
        cAttribute["type"] = "UCharTex"
        cAttribute["value"] = self.textureAttributes[att]
        cAttribute["uniform"] = self.attributeUniformMap[att]
        materialObject["Attributes"].append(cAttribute)

    return materialObject
def ParseFloat(p_floatStr):
  return float(p_floatStr)

def ParserVec3(p_floatStr):
  valueTable = p_floatStr.split(' ')
  return [float(valueTable[0]), float(valueTable[1]), float(valueTable[2])]

currentMaterial = None
materialList = []

UniformMap = dict()
UniformMap["Ka"] = "ambient_color"
UniformMap["Kd"] = "diffuse_color"
UniformMap["Ks"] = "specular_color"
UniformMap["map_Kd"] = "texture1"
UniformMap["map_Ka"] = "texture2"
UniformMap["map_Ks"] = "texture3"
UniformMap["map_bump"] = "texture4"
UniformMap["map_d"] = "texture5"
UniformMap["map_normal"] = "texture6"

FloatToParse = ["Ns", "Ni", "d", "Tr", "illum"]
Vec3ToParse = ["Tf", "Ka", "Kd", "Ks", "Ke"]
TextureToPars = ["map_Ka", "map_Kd", "map_d", "map_bump"]


with open(mtl_filepath) as fp:
   line = fp.readline()
   cnt = 1
   while line:
     #Remove last char of line
     line = line[:-1]
     if line.startswith("newmtl"):
       currentMtlName = line[7:]
       print currentMtlName
       newMTL = Material(currentMtlName, UniformMap)
       materialList.append(newMTL)
       currentMaterial = newMTL

     elif currentMaterial != None and len(line) > 0 :
       #First char is a tab
       line = line[1:]
       #print line.split(' ', 1)
       attributeName = line.split(' ', 1)[0]
       #print attributeName
       attributeValue = line.split(' ', 1)[1]
       #print attributeValue
       if attributeName in FloatToParse:
         currentMaterial.floatAttributes[attributeName] = ParseFloat(attributeValue)
       if attributeName in Vec3ToParse:
         currentMaterial.vec3Attributes[attributeName] = ParserVec3(attributeValue)
       if attributeName in TextureToPars:
         currentMaterial.textureAttributes[attributeName] = attributeValue

     line = fp.readline()


#Serialize all material
jsonRep = dict()
for mat in materialList:
  jsonRep[mat.name] = mat.ToDict()

with open(output_file_path, "w") as write_file:
    json.dump(jsonRep, write_file, indent=4)
