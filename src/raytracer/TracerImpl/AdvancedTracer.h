/*
 * AdvancedTracer.h
 *
 *  Created on: Mar 26, 2012
 *      Author: ulvinge
 */

#ifndef ADVANCEDTRACER_H_
#define ADVANCEDTRACER_H_

#include "PhotonMapper.h"

namespace raytracer {

class AdvancedTracer: public PhotonMapper {
public:
  AdvancedTracer(Scene* scene);
  virtual ~AdvancedTracer();
private:
};

}

#endif /* ADVANCEDTRACER_H_ */
