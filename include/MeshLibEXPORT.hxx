#ifndef MESHLIBEXPORT_HXX
#define MESHLIBEXPORT_HXX

#if defined (WIN32)
  #define MeshLib_EXPORT __declspec(dllexport)
#else
  #define MeshLib_EXPORT
#endif

#endif
