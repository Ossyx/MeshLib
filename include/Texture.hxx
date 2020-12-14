#ifndef TEXTURE_HXX
#define TEXTURE_HXX

#include <string>
#include <cstring>
#include <filesystem>
#include <memory>

#define cimg_use_png
#define cimg_use_jpeg
#include <CImg.h>

namespace cimg = cimg_library;

#include "Common.hxx"

namespace rx
{
  
template <typename T>
struct Texture
{
  Texture();

  ~Texture();

  Texture(Texture<T> const& p_other);

  Texture<T>& operator=(Texture<T> const& p_other);

  //B&W Textures
  void Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_data);

  //RG Texture
  void Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_dataR, T* p_dataB);
  
  //RGB Textures
  void Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_dataR, T* p_dataG, T* p_dataB);

  //RGBA Textures
  void Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_dataR, T* p_dataG, T* p_dataB, T* p_dataA);

  std::string m_filepath;
  unsigned int m_width;
  unsigned int m_height;  
  unsigned int m_channelCount;
  T* m_data;
};

class TextureFactory
{
public:
  template <typename T>
  static std::shared_ptr<Texture<T>> Create(std::filesystem::path const& pPath, bool pYinvert = false);
};

  //Template implementation of texture data structure
template <typename T>
Texture<T>::Texture():
  m_filepath(""),
  m_width(0),
  m_height(0),
  m_channelCount(0),
  m_data(NULL)
{
}

template <typename T>
Texture<T>::~Texture()
{
  if (m_data != NULL)
  {
    delete [] m_data;
  }
}

template <typename T>
Texture<T>& Texture<T>::operator=(Texture<T> const& p_other)
{
  //Operator do a shallow copy
  this->m_filepath = p_other.m_filepath;
  this->m_width = p_other.m_width;
  this->m_height = p_other.m_height;
  this->m_channelCount = p_other.m_channelCount;

  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = p_other.m_width*p_other.m_height*p_other.m_channelCount;
  m_data = new T[size];
  std::memcpy(this->m_data, p_other.m_data, size*sizeof(T));
  return *this;
}

template <typename T>
void Texture<T>::Initialize(std::string const& p_filepath,
    unsigned int p_width, unsigned int p_height,
    T* p_data)
{
  this->m_filepath = p_filepath;
  this->m_width = p_width;
  this->m_height = p_height;
  this->m_channelCount = 1;
  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = m_width*m_height*m_channelCount;
  m_data = new T[size];

  for (int i = 0; i < m_width*m_height; ++i)
  {
    m_data[i] = p_data[i];
  }
}

template <typename T>
void Texture<T>::Initialize(std::string const& p_filepath,
  unsigned int p_width, unsigned int p_height,
  T* p_dataR, T* p_dataG)
{
  this->m_filepath = p_filepath;
  this->m_width = p_width;
  this->m_height = p_height;
  this->m_channelCount = 2;
  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = m_width*m_height*m_channelCount;
  m_data = new T[size];

  //Interleave RGB data
  for (int i = 0; i < m_width*m_height; ++i)
  {
    m_data[i*m_channelCount] = p_dataR[i];
    m_data[i*m_channelCount + 1] = p_dataG[i];
  }
}


template <typename T>
void Texture<T>::Initialize(std::string const& p_filepath,
  unsigned int p_width, unsigned int p_height,
  T* p_dataR, T* p_dataG, T* p_dataB)
{
  this->m_filepath = p_filepath;
  this->m_width = p_width;
  this->m_height = p_height;
  this->m_channelCount = 3;
  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = m_width*m_height*m_channelCount;
  m_data = new T[size];

  //Interleave RGB data
  for (int i = 0; i < m_width*m_height; ++i)
  {
    m_data[i*m_channelCount] = p_dataR[i];
    m_data[i*m_channelCount + 1] = p_dataG[i];
    m_data[i*m_channelCount + 2] = p_dataB[i];
  }
}

template <typename T>
void Texture<T>::Initialize(std::string const& p_filepath,
  unsigned int p_width, unsigned int p_height,
  T* p_dataR, T* p_dataG, T* p_dataB, T* p_dataA)
{
  this->m_filepath = p_filepath;
  this->m_width = p_width;
  this->m_height = p_height;
  this->m_channelCount = 4;
  if (m_data != NULL)
  {
    delete [] m_data;
  }
  unsigned int size = m_width*m_height*m_channelCount;
  m_data = new T[size];

  //Interleave RGBA data
  for (int i = 0; i < m_width*m_height; ++i)
  {
    m_data[i*m_channelCount] = p_dataR[i];
    m_data[i*m_channelCount + 1] = p_dataG[i];
    m_data[i*m_channelCount + 2] = p_dataB[i];
    m_data[i*m_channelCount + 3] = p_dataA[i];
  }
}

template <typename T>
Texture<T>::Texture(Texture<T> const& p_other)
{
  *this = p_other;
}

template <typename T>
std::shared_ptr<Texture<T>> TextureFactory::Create(std::filesystem::path const& pPath, bool pYinvert)
{
  cimg::CImg<T> image(pPath.c_str());  
  if ( pYinvert )
  {
    image.mirror('y');
  }
  
  auto tex = std::make_shared<Texture<T>>();
  rxLogInfo("Loading "<<pPath<<" with spectrum " << image.spectrum());
//   cimg::CImgDisplay main_disp(image,"Click a point");
//   while (!main_disp.is_closed()) {
//     main_disp.wait();
//   }
  unsigned int pixelCount = image.width() * image.height();
  if (image.spectrum() == 1)//B&W
  {
    tex->Initialize(pPath, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data());
  }
  else if (image.spectrum() == 2)//RG
  {
    tex->Initialize(pPath, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data(), image.data(pixelCount));
  }
  else if (image.spectrum() == 3)//RGB
  {
    tex->Initialize(pPath, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data(), image.data(pixelCount), image.data(2*pixelCount));
  }
  else if(image.spectrum() == 4)//RGBA
  {
    tex->Initialize(pPath, (unsigned int)image.width(),
     (unsigned int)image.height(),
     image.data(), image.data(pixelCount),
     image.data(2*pixelCount), image.data(3*pixelCount));
  }
  
  return tex;
}

}

#endif
