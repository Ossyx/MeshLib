cmake_minimum_required(VERSION 2.6)
project(MeshLib)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

#Assimp is a dependency
find_package(Assimp REQUIRED)
#CImg is a dependency
find_package(CImg REQUIRED)
#JSon is a dependency
find_package(JsonCpp REQUIRED)

set(GLM_INCLUDE "/home/bertrand/Work/Lib/glm/install/include")
include_directories(${CMAKE_SOURCE_DIR}/include ${GLM_INCLUDE} ${CImg_INCLUDE_DIR}
${JsonCpp_INCLUDE_DIR})

add_library(MeshLib SHARED
  ${CMAKE_SOURCE_DIR}/src/Mesh.cxx
  ${CMAKE_SOURCE_DIR}/src/Material.cxx
  ${CMAKE_SOURCE_DIR}/src/Model.cxx
  ${CMAKE_SOURCE_DIR}/src/ModelLoader.cxx)

#target_include_directories(MeshLib PUBLIC ${ASSIMP_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/glm)

target_include_directories(MeshLib PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(MeshLib ${ASSIMP_LIBRARIES} ${CImg_LIBRARIES} ${JsonCpp_LIBRARY})

add_executable(TestLoading ${CMAKE_SOURCE_DIR}/tests/loading.cxx)
target_link_libraries(TestLoading MeshLib)

add_subdirectory(modules)

file(GLOB_RECURSE srcs
  ${CMAKE_SOURCE_DIR}/src/*.cxx
  ${CMAKE_SOURCE_DIR}/include/*.hxx)
add_custom_target(VeraStyle ALL
  vera++
  --warning
  ${srcs})

