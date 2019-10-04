#ifndef IMAGE_PROCESSING_HXX
#define IMAGE_PROCESSING_HXX

#include <string>

#include <CImg.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4


void GenerateNormalMap(std::string const& p_bumpMapInput, std::string const& p_normalMapOutput);

#endif
