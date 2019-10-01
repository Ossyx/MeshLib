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
    
    ModelLoader loader;
    loader.LoadOBJModel(directory, file, "testModel");
  }
}
