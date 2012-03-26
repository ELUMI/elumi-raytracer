/*
 * Heightmap.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: david
 */

#include "Heightmap.h"

using namespace glm;
using namespace std;

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

IAccDataStruct::IntersectionData Heightmap::getTriangle(Ray ray) {
  //Musgrave algorithm
  //Naive. Drop y axis

  vec3 intersection_point = vec3();

  bool intersection = false;

  //Ray-box intersection
  float t_min = -INFINITY;
  float t_max = INFINITY;
  vec3 p = position - ray.getPosition();

  vec3 u = vec3(1,0,0);
  vec3 v = vec3(0,1,0);
  vec3 w = vec3(0,0,1);

  vec3 current_vector = u;

  for(int i = 0; i < 3; i++) {

    switch (i) {
      case 0:
        current_vector = u;
        break;
      case 1:
        current_vector = v;
        break;
      case 2:
        current_vector = w;
        break;
      default:
        break;
    }

    float e = dot(current_vector,p);
    float f = dot(current_vector,ray.getDirection());

    if(true) {
      float t1 = (e + width/2)/f;
      float t2 = (e - width/2)/f;

      if(t1 > t2) {
        float tmp = t1;
        t1 = t2;
        t2 = tmp;
      }

      if(t1 > t_min)
        t_min = t1;
      if(t2 < t_max)
        t_max = t2;

      if(t_min > t_max)
        break;
      if(t_max < 0)
        break;
    }
  }

  if(t_min > 0) {
    intersection_point = p+ray.getDirection()*t_min;
    intersection = true;
  }
  else {
    intersection_point = p+ray.getDirection()*t_max;
    intersection = true;
  }

  //cout << "T min: " << t_min << " T max: " << t_max << endl;

  if(intersection)
    return IAccDataStruct::IntersectionData(0,intersection_point,vec3(0,1,0),vec2(),vec3(),vec3());
  else return IAccDataStruct::IntersectionData(-1,vec3(),vec3(),vec2(),vec3(),vec3());

//  vec3 ip = intersection_point - position;
//
//  ip = vec3(ip.x,ip.z,ip.y);
//
//  vec3 direction = normalize(ray.getDirection() - position);
//  direction = vec3(direction.x,direction.z,direction.y);
//
//  vec3 diff = normalize(vec3(direction.x,direction.z,direction.y));
//
//  //Starting cell is...
//  vec2 current_cell = vec2(floor(ip.x/real_width),floor(ip.y/real_height));
//
//
//  if(diff.y > diff.x) {
//    int x = current_cell.x;
//
//    float d = diff.x / diff.y;
//
//    int step_x = ceil(diff.x);
//    int step_y = ceil(diff.y);
//
//    for(int y = current_cell.y; x < width && x >= 0 &&
//        y < height && y >= 0;) {
//
//      vec3 vz = ip+(1/real_height)*y*diff;
//      vec3 vf = ip+(1/real_height)*(y+step_y)*diff;
//
//      float near_z, far_z;
//
//      near_z = vz.z;
//      far_z = vf.z;
//      if(glm::min(near_z,far_z) <= glm::min(
//          glm::min(data[width*y+x],data[width*y+x+step_x]),
//              glm::min(data[width*(y+step_y)+x],data[width*(y+step_y)+x+step_x]))) {
//        //Intersection
//        //Vertices
//        vec3 v0,v1,v2,v3;
//
//        v0 = vec3(current_cell.x*real_width,current_cell.y+real_height,data[width*y+x]);
//        v0 = v0 + position;
//
//        v1 = vec3(current_cell.x+step_x*
//            real_width,current_cell.y+real_height,data[width*y+(x+step_x)]);
//        v1 = v1 + position;
//
//        v2 = vec3(current_cell.x*real_width,current_cell.y+step_y
//            +real_height,data[width*(y+step_y)+x]);
//        v2 = v2 + position;
//
//        v3 = vec3(current_cell.x+step_x*real_width,current_cell.y+
//            step_y+real_height,data[width*(y+step_y)+x+step_x]);
//        v3 = v3 + position;
//
//        vector<vec3*> vertices;
//
//        vertices.push_back(&v0);
//        vertices.push_back(&v1);
//        vertices.push_back(&v2);
//
//        vec3 normal = vec3(cross(v1-v0,v2-v0));
//
//        vector<vec3*> normals;
//
//        normals.push_back(&normal);
//        normals.push_back(&normal);
//        normals.push_back(&normal);
//
//        vector<vec3*> tex_coords;
//
//        tex_coords.push_back(new vec3(0,0,0));
//
//        Triangle near_triangle = Triangle(vertices,normals,tex_coords,0);
//
//        return Triangle();
//      }
//
//      x = step_x;
//      y = step_y;
//
//      current_cell.x = x;
//      current_cell.y = y;
//    }
//  } else {
//  }

  //return Triangle();
}

}
