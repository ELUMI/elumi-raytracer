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

	Color shade()
};

}

#endif /* ISHADER_H_ */
