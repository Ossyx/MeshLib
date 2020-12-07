cmake_minimum_required(VERSION 2.6)
project(MeshLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

#Assimp is a dependency
find_package(Assimp REQUIRED)
#CImg is a dependency
find_package(CImg REQUIRED)
#JSon is a dependency
find_package(JsonCpp REQUIRED)
#glm is a dependency
find_package(glm REQUIRED)
#libPNG for CImg to have png full support
find_package(PNG REQUIRED)
#libJPEG for CImg to have jpeg full support
find_package(JPEG REQUIRED)

message(GLM : ${GLM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include ${GLM_INCLUDE_DIRS} ${CImg_INCLUDE_DIR}
${JsonCpp_INCLUDE_DIR})

file(GLOB_RECURSE headers
  ${CMAKE_SOURCE_DIR}/include/*.hxx)
install(FILES ${headers} DESTINATION include)

add_library(MeshLib SHARED
  ${CMAKE_SOURCE_DIR}/src/Mesh.cxx
  ${CMAKE_SOURCE_DIR}/src/GLSLTypeStore.cxx
  ${CMAKE_SOURCE_DIR}/src/Material.cxx
  ${CMAKE_SOURCE_DIR}/src/Model.cxx
  ${CMAKE_SOURCE_DIR}/src/ModelLoader.cxx
  ${CMAKE_SOURCE_DIR}/src/Primitives.cxx)

target_include_directories(MeshLib PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(MeshLib ${ASSIMP_LIBRARIES} ${CImg_LIBRARIES} ${JsonCpp_LIBRARY} ${PNG_LIBRARY_RELEASE} ${JPEG_LIBRARY_RELEASE})

install(TARGETS MeshLib DESTINATION lib)

add_executable(TestLoading ${CMAKE_SOURCE_DIR}/tests/loading.cxx)
target_link_libraries(TestLoading MeshLib)

install(TARGETS TestLoading DESTINATION bin)

add_subdirectory(modules)

file(GLOB_RECURSE srcs
  ${CMAKE_SOURCE_DIR}/src/*.cxx
  ${CMAKE_SOURCE_DIR}/include/*.hxx)
add_custom_target(VeraStyle ALL
  vera++
  --warning
  ${srcs})

