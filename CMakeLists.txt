cmake_minimum_required(VERSION 3.21)
project(MeshLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
if (NOT WIN32)
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
endif (NOT WIN32)

set(MeshLib_MAJOR_VERSION 0)
set(MeshLib_MINOR_VERSION 1)
set(MeshLib_PATCH_VERSION 0)
set(MeshLib_VERSION
  ${MeshLib_MAJOR_VERSION}.${MeshLib_MINOR_VERSION}.${MeshLib_PATCH_VERSION})


option(BUILD_MODULES "Build the modules" OFF)

#Assimp is a dependency
find_package(Assimp REQUIRED)
#CImg is a dependency
find_package(CImg REQUIRED)
#JSon is a dependency
find_package(JsonCpp CONFIG REQUIRED)
#glm is a dependency
find_package(glm REQUIRED)
#libPNG for CImg to have png full support
find_package(libpng REQUIRED)
#libJPEG for CImg to have jpeg full support
find_package(JPEG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(MeshLib SHARED
  ${CMAKE_SOURCE_DIR}/src/Mesh.cxx
  ${CMAKE_SOURCE_DIR}/src/GLSLTypeStore.cxx
  ${CMAKE_SOURCE_DIR}/src/Material.cxx
  ${CMAKE_SOURCE_DIR}/src/Model.cxx
  ${CMAKE_SOURCE_DIR}/src/ModelLoader.cxx
  ${CMAKE_SOURCE_DIR}/src/Primitives.cxx)

if (WIN32 AND VCPKG_TOOLCHAIN)
  target_include_directories(MeshLib PUBLIC ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
endif()
target_link_libraries(MeshLib PRIVATE png assimp::assimp jsoncpp_lib glm::glm JPEG::JPEG) 

add_executable(TestLoading ${CMAKE_SOURCE_DIR}/tests/loading.cxx)
target_link_libraries(TestLoading MeshLib)


if(BUILD_MODULES)
  add_subdirectory(modules)
endif()

if(NOT WIN32)
  file(GLOB_RECURSE srcs
    ${CMAKE_SOURCE_DIR}/src/*.cxx
    ${CMAKE_SOURCE_DIR}/include/*.hxx)
  add_custom_target(VeraStyle ALL
    vera++
    --warning
    ${srcs})
endif()

set_target_properties(MeshLib PROPERTIES
  PUBLIC_HEADER "include/Common.hxx;include/Material.hxx;include/Mesh.hxx;include/Model.hxx;include/ModelLoader.hxx;include/MeshLibEXPORT.hxx;include/GLSLTypeStore.hxx;include/Texture.hxx;include/Primitives.hxx")

# Add all targets to the build-tree export set
export(TARGETS MeshLib
FILE "${PROJECT_BINARY_DIR}/MeshLibTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE MeshLib)

configure_file(${CMAKE_SOURCE_DIR}/CMake/MeshLibConfig.cmake.in
"${PROJECT_BINARY_DIR}/MeshLibConfig.cmake" @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/CMake/MeshLibConfigVersion.cmake.in
"${PROJECT_BINARY_DIR}/MeshLibConfigVersion.cmake" @ONLY)


if (WIN32 AND VCPKG_TOOLCHAIN)
  include(CMake/installWindows.cmake)
endif()