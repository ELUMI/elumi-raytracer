/*
 * Settings.h
 *
 *  Created on: Jan 20, 2012
 *      Author: david
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>

namespace raytracer {

	struct Settings {
	    int width;
	    int height;
	    uint8_t* backgroundColor;

	};

}

#endif /* SETTINGS_H_ */
