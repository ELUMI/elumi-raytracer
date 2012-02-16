/*
 * Texture.h
 *
 *  Created on: Jan 26, 2012
 *      Author: DmZ
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

namespace raytracer {

class Texture {
public:
  Texture();
  Texture(unsigned int width, unsigned int height, unsigned char* data);
	virtual ~Texture();

private:
  int width, height;
  unsigned char* data;
};

}



#endif /* TEXTURE_H_ */
