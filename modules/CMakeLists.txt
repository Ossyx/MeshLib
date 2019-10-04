#Python is required
find_package(PythonLibs REQUIRED)

set(MODULE_BINARY_DIR "${CMAKE_BINARY_DIR}/modules")
file(MAKE_DIRECTORY ${MODULE_BINARY_DIR})

add_custom_command(
    OUTPUT  ${MODULE_BINARY_DIR}/image_processing_wrap.cxx
    COMMAND "swig" "-outdir" "${MODULE_BINARY_DIR}" "-o" "${MODULE_BINARY_DIR}/image_processing_wrap.cxx" "-c++" "-python" "${CMAKE_CURRENT_SOURCE_DIR}/image_processing.i"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GLM_INCLUDE} ${CImg_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
add_library(image_processing SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/image_processing.cxx
  ${MODULE_BINARY_DIR}/image_processing_wrap.cxx)
target_link_libraries(image_processing ${CImg_LIBRARIES} ${PYTHON_LIBRARIES})

set_target_properties(image_processing PROPERTIES OUTPUT_NAME _image_processing)
SET_TARGET_PROPERTIES(image_processing PROPERTIES PREFIX "")

install(TARGETS image_processing DESTINATION modules)
install(FILES ${MODULE_BINARY_DIR}/image_processing.py DESTINATION modules)

#add_dependencies(image_processing swig_module_image_processing)

