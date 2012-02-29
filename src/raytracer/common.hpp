/*
 * common.hpp
 *
 *  Created on: Feb 22, 2012
 *      Author: DmZ
 */

#include "utilities/Triangle.h"
#include "utilities/Ray.h"
#include <GL/glfw.h>

#ifndef COMMON_HPP_
#define COMMON_HPP_
using namespace glm;
namespace raytracer{

class Line{
public:
  Line(vec3 start,vec3 end){Line::start=start;Line::end=end;}
  vec3& getStart(){return start;}
  vec3& getEnd(){return end;}
private:
  vec3 start, end;
};
class AABB{
  public:
    AABB(const float& x,const float& y,const float& z,const float& w,const float& h,const float&  d){
      AABB::pos = vec3(x,y,z);
      AABB::size = vec3(w,h,d);
      createLines();
    }
    AABB(const vec3 pos,const vec3& size){
      AABB::pos = pos;
      AABB::size = size;
      createLines();
    }
    bool intersect(Ray ray,float& min,float& max);
    const vec3& getPos(){return AABB::pos;}
    const vec3& getSize(){return AABB::size;}
    const vector<Line>& getLines(){
      return AABB::lines;
    }
  private:
    vector<Line> lines;
    vec3  pos,size;
    float x,y,z,w,h,d;

    void createLines();
};
}

#endif /* COMMON_HPP_ */
