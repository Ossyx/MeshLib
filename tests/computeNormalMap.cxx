#include <string>

#include "ModelLoader.hxx"
 
int main(int argc, char** argv)
{  
  if(argc >= 2)
  {
    std::string directory(argv[1]);
    std::string file(argv[2]);
    MeshLibLog("Try loading from directory "<<directory
      <<" file "<<file<<" as test model.");
    
    std::string path = directory + "/" + file;
    std::replace(path.begin(), path.end(), '\\', '/');
    cimg_library::CImg<unsigned char> image(path.c_str());
    Texture<unsigned char> normalMap;
    ModelLoader::ComputeNormalMap(image, normalMap);
  }
}
