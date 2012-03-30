/*
 * GridPM.h
 *
 *  Created on: Mar 20, 2012
 *      Author: ulvinge
 */

#ifndef GRIDPM_H_
#define GRIDPM_H_

#include "../IPhotonMap.h"

namespace raytracer {

class GridPM: public IPhotonMap {
public:
  GridPM();
  virtual ~GridPM();
};

}

#endif /* GRIDPM_H_ */
