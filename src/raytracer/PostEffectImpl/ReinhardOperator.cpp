/*
 * ReinhardOperator.cpp
 *
 *  Created on: 22 feb 2012
 *      Author: julian
 */

#include "ReinhardOperator.h"
#include <glm/glm.hpp>
#include <iostream>
#include <assert.h>


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

void ReinhardOperator::run(float* color_buffer, int pixels, int channels) {

  cout << "ReinhardOperator: Currently only supports local operator!" << endl;

  mat3 RGB2XYZ = mat3(0.5141364, 0.3238786,  0.16036376,
                      0.265068,  0.67023428, 0.06409157,
                      0.0241188, 0.1228178,  0.84442666);

  RGB2XYZ = transpose(RGB2XYZ);

  int length = pixels * 4;

  //vec3 buffer[pixels];

  for( int i = 0; i < length; i += channels) {
    vec3 v = vec3(color_buffer[i],
                  color_buffer[i+1],
                  color_buffer[i+2]);

    // Convert from RGB to XYZ
    v = RGB2XYZ * v;

    // Convert from XYZ to Yxy
    float nom = v.x + v.y + v.z;  // nominator
    if(nom > 0.f) {
      float Y = v.y;        // Y
      float x = v.x / nom;  // x
      float y = v.y / nom;  // y
      v.x = Y;
      v.y = x;
      v.z = y;
    } else {
      v.x = 0;
      v.y = 0;
      v.z = 0;
    }

    color_buffer[i]   = v.x;
    color_buffer[i+1] = v.y;
    color_buffer[i+2] = v.z;
  }

  float log_avg = logAverage(color_buffer, pixels, channels);
  float scale_factor = key / log_avg;
  for(int i = 0; i < length; i += channels) {       // Eq. 2
    color_buffer[i] *= scale_factor;
  }


  float lmax2;
  if(white < 1e20) {
    lmax2 = white * white;
  } else {
    lmax2 = getMaxValue(color_buffer, pixels, channels);
    lmax2 *= lmax2;
  }

//  cout << "ReinhardOperator: lmax2: " << lmax2 << endl;
  for(int i = 0; i < length; i += channels) {         // Eq. 4 (if white is big enough, eq 3)
    //buffer[i].x = buffer[i].x / (1.0f + buffer[i].x);

    color_buffer[i] = (color_buffer[i] * ( 1.0f + color_buffer[i] / lmax2)) /
                  (1.0f + color_buffer[i]);
  }

  mat3 XYZ2RGB = mat3(2.5651,   -1.1665,   -0.3986,
                     -1.0217,    1.9777,    0.0439,
                      0.0753,   -0.2543,    1.1892);

  XYZ2RGB = transpose(XYZ2RGB);

  for(int i = 0; i < length; i += 4) {
    vec3 v = vec3(color_buffer[i],
                  color_buffer[i+1],
                  color_buffer[i+2]);

    // Convert from Yxy to XYZ
    if(v.x > 0.f && v.y > 0.f && v.z > 0.f) {
      float Y = v.x, x = v.y, y = v.z;
      float X = (x * Y) / y;
      float Z = (X / x) - X - Y;
      v.x = X;
      v.y = Y;
      v.z = Z;
    } else {
      v.x = 0.f;
      v.y = 0.f;
      v.z = 0.f;
    }

    // Convert from XYZ to RGB
    v = XYZ2RGB * v;

    color_buffer[i]    = v.x;
    color_buffer[i+1]  = v.y;
    color_buffer[i+2]  = v.z;
  }
}

float ReinhardOperator::getMaxValue(float* buf, int pixels, int channels) {
  float max = 0;
  for(int i = 0; i < pixels * channels; i += channels) {
    if(channels == 4 && buf[i+3] == 0)
      continue;
    if(buf[i] > max)
      max = buf[i];
  }
  if(max == 0)    // Prevent division by zero.
    max = 1e20;

  return max;
}

float ReinhardOperator::logAverage(float* buf, int pixels, int channels) { // Eq. 1
  double sum = 0;
  for(int i = 0; i < pixels * channels; i += channels) {
    if(channels == 4 && buf[i+3] == 0)
      continue;
    sum += log(0.00001 + (double)buf[i]);
  }
  double res = sum / (double)pixels;  // OBS! Wrong in paper! division by pixels
  return exp(res);                    // should be done before! exp-function
}

}
