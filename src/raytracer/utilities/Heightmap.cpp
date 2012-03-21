/*
 * Heightmap.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: david
 */

#include "Heightmap.h"

using namespace glm;

namespace raytracer {

Heightmap::Heightmap(size_t width, size_t height, float real_width,
    float real_height, vec3 position, unsigned char* data) {
  Heightmap::data = data;
  Heightmap::width = width;
  Heightmap::height = height;
  Heightmap::real_width = real_width;
  Heightmap::real_height = real_height;
  Heightmap::position = position;
  max_elevation = 0;
  for(unsigned int i = 0; i < width*height; i++) {
    max_elevation < data[i] ? max_elevation = data[i] : max_elevation;
  }
}

Heightmap::~Heightmap() {}

Triangle Heightmap::getTriangle(Ray ray, vec3 intersection_point) {
  //Musgrave algorithm
  //Naive. Drop y axis
  vec2 ip = vec2(intersection_point.x,intersection_point.z) - vec2(position.x,position.z);

  vec3 direction = ray.getDirection() - position;

  vec3 diff = normalize(vec3(direction.x,direction.z,direction.y));

  //Starting cell is...
  vec2 current_cell = vec2(floor(ip.x/real_width),floor(ip.y/real_height));


  if(diff.y > diff.x) {
    int x = current_cell.x;

    float d = diff.x / diff.y;

    for(int y = current_cell.y; x < width && x >= 0 &&
        y < height && y >= 0;) {

      float near_z = current_cell.y*d;

      int new_x = x+ceil(d);

      float far_z = new_x*d;

      if(glm::min(near_z,far_z) <= glm::min(
          glm::min(data[width*y+x],data[width*y+x+1]),
              glm::min(data[width*(y+1)+x],data[width*(y+1)+x+1]))) {
        //Intersection
        return Triangle();
      }

      x = new_x;
      y=ceil(diff.y);

      current_cell.x = x;
      current_cell.y = y;
    }
  } else {
  }

  return Triangle();
}

}
