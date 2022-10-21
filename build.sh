rm -rf build/*
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/project/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . -v
cmake --install . --config Debug