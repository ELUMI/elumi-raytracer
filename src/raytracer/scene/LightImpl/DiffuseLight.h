#ifndef DIFFUSELIGHT_H_
#define DIFFUSELIGHT_H_

#include "../ILight.h"

namespace raytracer {

class DiffuseLight : public ILight {
public:
  DiffuseLight();
	virtual ~DiffuseLight();

};

}


#endif /* DIFFUSELIGHT_H_ */
