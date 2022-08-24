cmake_minimum_required(VERSION 3.2)
project(MeshLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
if (NOT WIN32)
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
endif (NOT WIN32)

#set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

set(MeshLib_MAJOR_VERSION 0)
set(MeshLib_MINOR_VERSION 1)
set(MeshLib_PATCH_VERSION 0)
set(MeshLib_VERSION
  ${MeshLib_MAJOR_VERSION}.${MeshLib_MINOR_VERSION}.${MeshLib_PATCH_VERSION})


# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/MeshLib)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
set(var INSTALL_${p}_DIR)
if(NOT IS_ABSOLUTE "${${var}}")
  set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
endif()
endforeach()

option(BUILD_MODULES "Build the modules" OFF)

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#set(BUILD_SHARED_LIBS TRUE)

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

include_directories(${CMAKE_SOURCE_DIR}/include ${glm_DIR} ${CImg_INCLUDE_DIR}
${JsonCpp_DIR})

#file(GLOB_RECURSE headers
#  ${CMAKE_SOURCE_DIR}/include/*.hxx)
#install(FILES ${headers} DESTINATION include)

add_library(MeshLib SHARED
  ${CMAKE_SOURCE_DIR}/src/Mesh.cxx
  ${CMAKE_SOURCE_DIR}/src/GLSLTypeStore.cxx
  ${CMAKE_SOURCE_DIR}/src/Material.cxx
  ${CMAKE_SOURCE_DIR}/src/Model.cxx
  ${CMAKE_SOURCE_DIR}/src/ModelLoader.cxx
  ${CMAKE_SOURCE_DIR}/src/Primitives.cxx)

target_include_directories(MeshLib PUBLIC ${Assimp_DIR})
target_include_directories(MeshLib PUBLIC ${CImg_DIR})
target_link_libraries(MeshLib PUBLIC CImg::CImg png assimp::assimp jsoncpp_lib glm::glm JPEG::JPEG) 

#install(TARGETS MeshLib DESTINATION lib)

add_executable(TestLoading ${CMAKE_SOURCE_DIR}/tests/loading.cxx)
target_link_libraries(TestLoading MeshLib)

#install(TARGETS TestLoading DESTINATION bin)

if(BUILD_MODULES)
  add_subdirectory(modules)
endif()

file(GLOB_RECURSE srcs
  ${CMAKE_SOURCE_DIR}/src/*.cxx
  ${CMAKE_SOURCE_DIR}/include/*.hxx)
#add_custom_target(VeraStyle ALL
#  vera++
#  --warning
#  ${srcs})

set_target_properties(MeshLib PROPERTIES
  PUBLIC_HEADER "include/Common.hxx;include/Material.hxx;include/Mesh.hxx;include/Model.hxx;include/ModelLoader.hxx;include/MeshLibEXPORT.hxx;include/GLSLTypeStore.hxx;include/Texture.hxx;include/Primitives.hxx")

#Install stuff
install(TARGETS MeshLib
  EXPORT MeshLibTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/MeshLib"
    COMPONENT dev)

#install(TARGETS MeshLib RUNTIME_DEPENDENCIES RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)

# Add all targets to the build-tree export set
export(TARGETS MeshLib
FILE "${PROJECT_BINARY_DIR}/FFTWaterTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE MeshLib)

# Create the FooBarConfig.cmake and FooBarConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
 "${INSTALL_INCLUDE_DIR}")

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${MeshLib_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(MeshLibConfig.cmake.in
"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/MeshLibConfig.cmake" @ONLY)

configure_file(MeshLibConfigVersion.cmake.in
"${PROJECT_BINARY_DIR}/MeshLibConfigVersion.cmake" @ONLY)


# Install the MeshLibConfig.cmake and MeshLibConfigVersion.cmake
install(FILES
"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/MeshLibConfig.cmake"
"${PROJECT_BINARY_DIR}/MeshLibConfigVersion.cmake"
DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT MeshLibTargets DESTINATION
"${INSTALL_CMAKE_DIR}" COMPONENT dev)