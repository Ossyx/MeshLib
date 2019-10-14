/**
* Copyright (C) 2019, Ossyx
*/
#include <iostream>

//#define MeshLibLog(log) std::cout<<x<<std::endl;

#define rxLogInfo(log) std::cout<< log <<std::endl;
#define rxLogDebug(log) rxLog(log, __FILE__, __LINE__, "Debug")
#define rxLogWarning(log) rxLog(log, __FILE__, __LINE__, "Warning")
#define rxLogError(log) rxLog(log, __FILE__, __LINE__, "Error")
#define rxLog(p_log, p_file, p_line, p_type) \
  std::cout <<p_file << ":("<< p_line<<")"<<" "\
  << p_type << " : " << p_log << std::endl;
