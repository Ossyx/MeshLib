#include <string>
#include <chrono>

#include "ModelLoader.hxx"

int main(int argc, char** argv)
{
  if(argc >= 2)
  {
    std::string directory(argv[1]);
    std::string file(argv[2]);
    rxLogInfo("Loading file "<< file <<" from directory "<< directory <<".");

    auto tbegin = std::chrono::steady_clock::now();

    rx::ModelLoader loader;
    loader.LoadOBJModel(directory, file, "testModel");

    auto tend = std::chrono::steady_clock::now();
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbegin).count();

    rxLogInfo("Total loading time is "<< millis / 1000.0f << "s");
  }
}
