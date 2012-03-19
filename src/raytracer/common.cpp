


#include "common.hpp"
#include <iostream>

namespace raytracer{

bool AABB::intersect(Ray ray,float& min,float& max){
  // Instead of calculating it several times do it once
  vec3 ray_dir = ray.getDirection();
  vec3 ray_pos = ray.getPosition();
  vec3 direction_opt = vec3(1.0f/ray_dir.x,1.0f/ray_dir.y,1.0f/ray_dir.z);

  // Calculate the distance from the ray to the different planes
  vec3 distance_min = vec3((pos.x-ray_pos.x)*direction_opt.x,
      (pos.y-ray_pos.y)*direction_opt.y,(pos.z-ray_pos.z)*direction_opt.z);
  vec3 distance_max = vec3(((pos.x+size.x)-ray_pos.x)*direction_opt.x,
      ((pos.y+size.y)-ray_pos.y)*direction_opt.y,((pos.z+size.z)-ray_pos.z)*direction_opt.z);

  // Find the minimal distance (closest plane)
  min = std::max(std::max(
      std::min(distance_min.x,distance_max.x),
      std::min(distance_min.y,distance_max.y)),
      std::min(distance_min.z,distance_max.z));

  // Find the maximal distance (plane farest away)
  max = std::min(std::min(
      std::max(distance_min.x,distance_max.x),
      std::max(distance_min.y,distance_max.y)),
      std::max(distance_min.z,distance_max.z));

  // AABB is behind ray
  if(max<0){
    return false;
  }

  // No intersection
  if(max < min ){
    return false;
  }

  return true;
}
void AABB::createLines(){
    lines.push_back(Line(pos,pos+vec3(size.x,0,0)));
    lines.push_back(Line(pos,pos+vec3(0,size.y,0)));
    lines.push_back(Line(pos,pos+vec3(0,0,+size.z)));

    lines.push_back(Line(pos+vec3(size.x,0,0),pos+vec3(size.x,size.y,0)));
    lines.push_back(Line(pos+vec3(size.x,0,0),pos+vec3(size.x,0,size.z)));

    lines.push_back(Line(pos+vec3(0,size.y,0),pos+vec3(size.x,size.y,0)));
    lines.push_back(Line(pos+vec3(0,+size.y,0),pos+vec3(0,size.y,size.z)));

    lines.push_back(Line(vec3(pos.x,pos.y,pos.z+size.z),vec3(pos.x+size.x,pos.y,pos.z+size.z)));
    lines.push_back(Line(vec3(pos.x,pos.y,pos.z+size.z),vec3(pos.x,pos.y+size.y,pos.z+size.z)));

    lines.push_back(Line(pos+size,pos+size-vec3(size.x,0,0)));
    lines.push_back(Line(pos+size,pos+size-vec3(0,size.y,0)));
    lines.push_back(Line(pos+size,pos+size-vec3(0,0,size.z)));

}
}
