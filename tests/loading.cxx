#include <string>
#include <chrono>

#include "ModelLoader.hxx"

int main(int argc, char** argv)
{
  if(argc >= 2)
  {
    std::string file(argv[1]);
    rxLogInfo("Loading file "<< file );

    auto tbegin = std::chrono::steady_clock::now();

    auto testModelPtr = rx::ModelLoader::LoadOBJModel(file, "testModel");

    auto tend = std::chrono::steady_clock::now();
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbegin).count();

    rxLogInfo("Total loading time is "<< millis / 1000.0f << "s");
  }
}
