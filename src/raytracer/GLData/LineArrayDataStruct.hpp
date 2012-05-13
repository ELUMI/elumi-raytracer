#ifndef LineArrayDataStruct_H_
#define LineArrayDataStruct_H_

#include "../IDraw.h"
#include "../common.hpp"

namespace raytracer {

class LineArrayDataStruct: public IDraw {
public:
  LineArrayDataStruct(){};
  LineArrayDataStruct(std::vector<AABB> aabb);
  LineArrayDataStruct(std::vector<vec3>& lines);
  virtual ~LineArrayDataStruct();
  void draw();
private:
  GLuint vertexArrayObject;
  GLuint linesBuffer;
  size_t size;
};
}

#endif
