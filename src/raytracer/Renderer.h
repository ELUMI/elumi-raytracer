/*
 * Renderer.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef RENDERER_H_
#define RENDERER_H_

namespace raytracer {

class Renderer {
public:
	Renderer();
	virtual ~Renderer();
	void render();
};

}

#endif /* RENDERER_H_ */
