
#include "../IDraw.h"
#include "../common.hpp"

namespace raytracer {

class LineArrayDataStruct: public IDraw {
public:
  LineArrayDataStruct(){};
  LineArrayDataStruct(std::vector<Line> lines);
  virtual ~LineArrayDataStruct();
  void draw();
private:
  GLuint vertexArrayObject;
  GLuint linesBuffer;
  size_t size;
};
}
