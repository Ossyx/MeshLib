#include "image_processing.hxx"

void GenerateNormalMap(std::string const& p_bumpMapInput, std::string const& p_normalMapOutput)
{
  cimg_library::CImg<unsigned char> bumpMap(p_bumpMapInput.c_str());

  cimg_library::CImgList<float> gradients = bumpMap.get_gradient("xy", 1);
  cimg_library::CImg<float>& xgradient = gradients.front();
  cimg_library::CImg<float>& ygradient = gradients.back();

  //Output is an rgb image
  cimg_library::CImg<unsigned char> normalMap(xgradient.width(), xgradient.height(), 1, 3, 0);

  //[-1.0, 1.0] --> [0, 255]
  auto MapFloatToChar = [](float p_float)
  {
    return static_cast<unsigned char>((p_float + 1.0f) * 255 / 2.0f);
  };

  for (int i=0; i < normalMap.width(); ++i)
  {
    for (unsigned j=0; j < normalMap.height(); ++j)
    {
      float& xgrad = xgradient(i, j);
      float& ygrad = ygradient(i, j);

      unsigned char& x = normalMap(i, j, 0);
      unsigned char& y = normalMap(i, j, 1);
      unsigned char& z = normalMap(i, j, 2);

      glm::vec3 normal(0.0f, 0.0f, 1.0f);

      auto rotYMat = glm::rotate(xgrad / 255.0f * 10.0f * -1.0f, glm::vec3(0.0, 1.0, 0.0));
      auto rotXMat = glm::rotate(ygrad / 255.0f * 10.0f * -1.0f, glm::vec3(1.0, 0.0, 0.0));

      auto rotMap = rotYMat * rotXMat;
      normal = glm::vec3(rotMap * glm::vec4(normal, 1.0f));

      x = MapFloatToChar(normal.x);
      y = MapFloatToChar(normal.y);
      z = MapFloatToChar(normal.z);
    }
  }

  normalMap.save(p_normalMapOutput.c_str());
//   cimg_library::CImgDisplay main_disp(normalMap,"Click a point");
//   while (!main_disp.is_closed())
//   {
//     main_disp.wait();
//   }
}
