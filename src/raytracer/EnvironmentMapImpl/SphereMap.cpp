/*
 * DiffuseMap.cpp
 *
 *  Created on: 18 maj 2012
 *      Author: irri
 */

#include "SphereMap.h"
#include <iostream>

namespace raytracer {

SphereMap::SphereMap(float*** hdrdata, unsigned int width) {
  this->data = hdrdata;
  this->width = width;

  c1 = 0.429043f;
  c2 = 0.511664f;
  c3 = 0.743125f;
  c4 = 0.886227f;
  c5 = 0.247708f;

  L00  = vec3(0);
  L1_1 = vec3(0);
  L10  = vec3(0);
  L11  = vec3(0);
  L2_2 = vec3(0);
  L2_1 = vec3(0);
  L20  = vec3(0);
  L21  = vec3(0);
  L22  = vec3(0);

  const float y1 = 0.282095;
  const float y2 = 0.488603;
  const float y3 = 1.092548;
  const float y4 = 0.315392;
  const float y5 = 0.546274;

  const float radius = width/2.0f;
  const float delta  = (M_PI)/radius * 1.0f/radius;

  for(unsigned int i=0; i<width; i++)
    for(unsigned int j=0; j<width; j++) {

      //convert coord to range [-1,1], unit circle
      float u = (radius-j)/radius;
      float v = (i-radius)/radius;
      float r = sqrt(u*u + v*v);
      if (r>1.0f) //check if inside circle
        continue;

      if(r==0.0f)
        continue;

      float theta = M_PI*r;
      float phi = atan2(v,u);

      float x = glm::sin(theta) * glm::cos(phi);
      float y = glm::sin(theta) * glm::sin(phi);
      float z = glm::cos(theta);

      vec3 color = vec3(data[i][j][0], data[i][j][1], data[i][j][2]);
      float sin  = glm::sin(theta)/theta * delta;

      L00  += color * y1 * sin;

      L11  += color * y2 * x * sin;
      L10  += color * y2 * z * sin;
      L1_1 += color * y2 * y * sin;

      L21  += color * y3 * x*z * sin;
      L2_1 += color * y3 * y*z * sin;
      L2_2 += color * y3 * x*y * sin;

      L20  += color * y4 * (3*z*z-1) * sin;

      L22  += color * y5 * (x*x-y*y) * sin;
    }

  /*std::cout << "L00 = (" << L00.x << "; " << L00.y << "; " << L00.z << ")\n"
            << "L11 = (" << L11.x << "; " << L11.y << "; " << L11.z << ")\n"
            << "L10 = (" << L10.x << "; " << L10.y << "; " << L10.z << ")\n"
            << "L1_1 = (" << L1_1.x << "; " << L1_1.y << "; " << L1_1.z << ")\n"
            << "L21 = (" << L21.x << "; " << L21.y << "; " << L21.z << ")\n"
            << "L2_1 = (" << L2_1.x << "; " << L2_1.y << "; " << L2_1.z << ")\n"
            << "L2_2 = (" << L2_2.x << "; " << L2_2.y << "; " << L2_2.z << ")\n"
            << "L20 = (" << L20.x << "; " << L20.y << "; " << L20.z << ")\n"
            << "L22 = (" << L22.x << "; " << L22.y << "; " << L22.z << ")\n";*/

}

SphereMap::~SphereMap() {
  for (unsigned int i = 0; i < width; ++i) {
    for (unsigned int j = 0; j < width; ++j) {
      delete [] data[i][j];
    }
    delete [] data[i];
  }
  delete [] data;

}



vec3 SphereMap::getSpecularColor(vec3 dir) {
  float m = glm::sqrt(dir.x*dir.x + dir.y*dir.y + (dir.z +1)*(dir.z +1));
  int x = (dir.x/(2.0f*m) + 0.5f) * width;
  int y = (dir.y/(2.0f*m) + 0.5f) * width;

  return vec3(data[y][x][0], data[y][x][1], data[y][x][2] );
}

vec3 SphereMap::getDiffuseColor(vec3 normal) {
  const float x = normal.x;
  const float y = normal.y;
  const float z = normal.z;
  const float x2 = x*x;
  const float y2 = y*y;
  const float z2 = z*z;

  vec3 irradiance =
      c1*L22*(x2-y2) + c3*L20*z2 + c4*L00 - c5*L20
      + 2*c1*(L2_2*x*y + L21*x*z + L2_1*y*z)
      + 2*c2*(L11*x+L1_1*y+L10*z);

  return irradiance;
}

}
