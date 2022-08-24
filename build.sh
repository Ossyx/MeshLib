rm -rf build/*
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/project/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_INSTALL_PREFIX=/c/lib/
cmake --build .
cmake --install . --config Debug
