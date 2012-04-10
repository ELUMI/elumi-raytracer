/*
 * ISuperSamplePattern.h
 *
 *  Created on: Apr 3, 2012
 *      Author: idk
 */

#ifndef ISUPERSAMPLEPATTERN_H_
#define ISUPERSAMPLEPATTERN_H_

#include <glm/glm.hpp>
using namespace glm;

namespace raytracer {

class ISuperSamplePattern {
public:
  ISuperSamplePattern(int size);
  virtual ~ISuperSamplePattern();

  const vec2* getOffsets() const;
  const vec2* getNewOffsets();
  virtual void newPixel() = 0;

  const int getSize() const;

protected:
  int size;
  vec2* offsets;
};

int gridify_size(int i);
ISuperSamplePattern* getSuperSamplePatter(int i);

} /* namespace raytracer */
#endif /* ISUPERSAMPLEPATTERN_H_ */
