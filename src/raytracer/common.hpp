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
  Line(vec3 start,vec3 size){Line::start=start;Line::end=start+size;}
  vec3& getStart(){return start;}
  vec3& getEnd(){return end;}
private:
  vec3 start, end;
};
class AABB{
  public:
    AABB(){}
    AABB(const float& x,const float& y,const float& z,const float& w,const float& h,const float&  d){
      AABB::pos = vec3(x,y,z);
      AABB::size = vec3(w,h,d);
      createAABBLines();
    }
    AABB(const vec3 pos,const vec3& size){
      AABB::pos = pos;
      AABB::size = size;
      createAABBLines();
    }
    AABB(const vec3 pos,const vec3& size,bool lines, int axis){
          AABB::pos = pos;
          AABB::size = size;
          if(lines){
            createSplittingLines(axis);
          }
    }
    bool intersect(Ray ray,float& min,float& max);
    const vec3& getPos(){return AABB::pos;}
    const vec3& getSize(){return AABB::size;}
    void setStartAxis(float start,int axis){AABB::size[axis]=start-AABB::pos[axis];AABB::pos[axis] = start;}
    void setEndAxis(float end,int axis){AABB::size[axis] = AABB::size[axis]-(end-pos[axis]);}
    float getSurfaceArea(){return 2*(size.x*size.y+size.x*size.z+size.y*size.z);}
    const vector<Line>& getLines(){
      return AABB::lines;
    }
    std::pair<AABB, AABB> split(float split,int axis);
    void extend(AABB aabb){
      vec3 ma = glm::max(pos+size, aabb.pos+aabb.size);
      pos = glm::min(pos,aabb.pos);
      size = ma-pos;
    }
  private:
    vector<Line> lines;
    vec3  pos,size;

    void createAABBLines();
    void createSplittingLines(int axis);
};
}

#endif /* COMMON_HPP_ */
