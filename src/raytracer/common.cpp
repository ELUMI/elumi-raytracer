


#include "common.hpp"
#include <iostream>

namespace raytracer{

bool AABB::intersect(Ray ray,float& min,float& max){
  // Instead of calculating it several times do it once
  vec3 ray_dir = ray.getDirection();
  vec3 ray_pos = ray.getPosition();
  vec3 direction_opt = vec3(1.0f/ray_dir.x,1.0f/ray_dir.y,1.0f/ray_dir.z);

  // Calculate the distance from the ray to the different planes
  float distance_x_min = (pos.x-ray_pos.x)*direction_opt.x;
  float distance_x_max = ((pos.x+size.x)-ray_pos.x)*direction_opt.x;
  float distance_y_min = (pos.y-ray_pos.y)*direction_opt.y;
  float distance_y_max = ((pos.y+size.y)-ray_pos.y)*direction_opt.y;
  float distance_z_min = (pos.z-ray_pos.z)*direction_opt.z;
  float distance_z_max = ((pos.z+size.z)-ray_pos.z)*direction_opt.z;

  // Find the minimal distance (closest plane)
  min = std::max(std::max(
      std::min(distance_x_min,distance_x_max),
      std::min(distance_y_min,distance_y_max)),
      std::min(distance_z_min,distance_z_max));

  // Find the maximal distance (plane farest away)
  max = std::min(std::min(
      std::max(distance_x_min,distance_x_max),
      std::max(distance_y_min,distance_y_max)),
      std::max(distance_z_min,distance_z_max));

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
    lines.push_back(Line(pos,vec3(pos.x+size.x,pos.y,pos.z)));
    lines.push_back(Line(pos,vec3(pos.x,pos.y+size.y,pos.z)));
    lines.push_back(Line(pos,vec3(pos.x,pos.y,pos.z+size.z)));

    lines.push_back(Line(vec3(pos.x+size.x,pos.y,pos.z),vec3(pos.x+size.x,pos.y+size.y,pos.z)));
    lines.push_back(Line(vec3(pos.x+size.x,pos.y,pos.z),vec3(pos.x+size.x,pos.y,pos.z+size.z)));

    lines.push_back(Line(vec3(pos.x,pos.y+size.y,pos.z),vec3(pos.x+size.x,pos.y+size.y,pos.z)));
    lines.push_back(Line(vec3(pos.x,pos.y+size.y,pos.z),vec3(pos.x,pos.y+size.y,pos.z+size.z)));
//
    lines.push_back(Line(vec3(pos.x,pos.y,pos.z+size.z),vec3(pos.x+size.x,pos.y,pos.z+size.z)));
    lines.push_back(Line(vec3(pos.x,pos.y,pos.z+size.z),vec3(pos.x,pos.y+size.y,pos.z+size.z)));
//
//
    lines.push_back(Line(pos+size,pos+size-vec3(size.x,0,0)));
    lines.push_back(Line(pos+size,pos+size-vec3(0,size.y,0)));
    lines.push_back(Line(pos+size,pos+size-vec3(0,0,size.z)));

//    for(int i=0;i<lines.size();i++){
//      cout << lines[i].getStart().x<< ","<<lines[i].getStart().y << ","<<lines[i].getStart().z << " | "
//      <<lines[i].getEnd().x<< ","<<lines[i].getEnd().y << ","<<lines[i].getEnd().z << endl;
//    }
}
}
