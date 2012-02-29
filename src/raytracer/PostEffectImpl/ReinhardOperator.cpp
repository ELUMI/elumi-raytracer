/*
 * ReinhardOperator.cpp
 *
 *  Created on: 22 feb 2012
 *      Author: julian
 */

#include "ReinhardOperator.h"
#include <glm/glm.hpp>
#include <iostream>


namespace raytracer {

using namespace glm;
using namespace std;

ReinhardOperator::ReinhardOperator(float _key, float _white, int _range,
                                   int _low, int _high) {
  key = _key;
  white = _white;
  range = _range;
  low = _low;
  high = _high;
}

float* ReinhardOperator::run(float* color_buffer, int length) {

  cout << "ReinhardOperator: Currently only supports local operator!" << endl;

  mat3 RGB2XYZ = mat3(0.5141364, 0.3238786,  0.16036376,
                      0.265068,  0.67023428, 0.06409157,
                      0.0241188, 0.1228178,  0.84442666);

  //RGB2XYZ = transpose(RGB2XYZ);

  int pixels = length / 4;

  // Convert to XYZ OBS! There is a difference between capital and no capital letters in this color space.
  vec3 buffer[pixels];
  cout << "ReinhardOperator: Assuming buffer format RGBA." << endl;
  for( int i = 0; i < length; i += 4) {
    vec3 v = vec3(color_buffer[i],
                  color_buffer[i+1],
                  color_buffer[i+2]);
    v = RGB2XYZ * v;    // TODO konvertera till Yxy (jmf. kŠllkod)
    buffer[i/4] = v;
  }

  float log_avg = logAverage(buffer, pixels);
  float scale_factor = 1.0f / log_avg;
  for(int i = 0; i < pixels; i++) {       // Eq. 2
    buffer[i] *= scale_factor * key;
  }


  float lmax2;
  if(white < 1e20) {
    lmax2 = white * white;
  } else {
    lmax2 = getMaxValue(buffer, pixels);
    lmax2 *= lmax2;
  }
  for(int i = 0; i < pixels; i++) {         // Eq. 4 (if white is big enough, eq 3)
    buffer[i].x = (buffer[i].x * ( 1.0f + buffer[i].x / lmax2) /
                  (1.0f + buffer[i].x));
  }

  mat3 XYZ2RGB = mat3(2.5651,   -1.1665,   -0.3986,
                     -1.0217,    1.9777,    0.0439,
                      0.0753,   -0.2543,    1.1892);

  //XYZ2RGB = transpose(XYZ2RGB);

  //float* res = new float[length];

  for(int i = 0; i < length; i += 4) {
    vec3 v = XYZ2RGB * buffer[i/4];
    color_buffer[i]    = v.x;
    color_buffer[i+1]  = v.y;
    color_buffer[i+2]  = v.z;
  }

  return color_buffer;
}

float ReinhardOperator::getMaxValue(vec3* buf, int pixels) {
  float max = 0;
  for(int i = 0; i < pixels; i++) {
    if(buf[i].x > max)
      max = buf[i].x;
  }
  if(max == 0)    // Prevent division by zero.
    max = 1e20;

  return max;
}

float ReinhardOperator::logAverage(vec3* buf, int pixels) { // Eq. 1
  double sum = 0;
  for(int i = 0; i < pixels; i++) {
    //cout << "sum: " << sum << ", buf[i].x: " << buf[i].x << ", logf(): " << logf(0.00001 + buf[i].x) << endl;
    sum += log(0.00001 + (double)buf[i].x);
  }
  double res = sum / (double)pixels;  // OBS! Wrong in paper! division by pixels
  return exp(res);                    // should be done before! exp-function
}

}
