/*
 * IShader.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef ISHADER_H_
#define ISHADER_H_

namespace raytracer {

class IShader {
public:
	virtual ~IShader();

	//virtual Color shade() = 0;
};

}

#endif /* ISHADER_H_ */
