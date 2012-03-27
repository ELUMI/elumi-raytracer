/*
 * Heightmap.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: david
 */

#include "Heightmap.h"
#include "../utilities/MathHelper.h"

using namespace glm;
using namespace std;

namespace raytracer {

Heightmap::Heightmap(size_t width, size_t height, vec3 position, vec3 u, vec3 v,float elevation, unsigned char* data) {
  Heightmap::data = data;
  Heightmap::width = width;
  Heightmap::height = height;
  Heightmap::u = u;
  Heightmap::v = v;
  Heightmap::position = position;
  max_elevation = 0;
  Heightmap::elevation = elevation;
  for(unsigned int i = 0; i < width*height; i++) {
    max_elevation < data[i] ? max_elevation = data[i] : max_elevation;
  }

  bb = OBB(vec3(0,0,0),u,v,vec3(0,elevation,0));
  Heightmap::texture = new Texture(width,height,data);
}

Heightmap::~Heightmap() {}

IAccDataStruct::IntersectionData Heightmap::getTriangle(Ray ray) {
  //Musgrave algorithm
  //Naive. Drop y axis

  OBB::IntersectionData id = bb.rayIntersection(ray);

  if(!id.intersected)
    return IAccDataStruct::IntersectionData(IAccDataStruct::IntersectionData::NOT_FOUND, vec3(), vec3(), vec2(),vec3(),vec3());
  //else return IAccDataStruct::IntersectionData(0, id.position, vec3(0,1,0), vec2(),vec3(),vec3());

  vec3 ip = id.position - position;

  ip = vec3(ip.x,ip.z,ip.y);

  MathHelper mh = MathHelper();

  vec3 direction = normalize(ray.getDirection());
  direction = vec3(direction.x,direction.z,direction.y);

  vec3 diff = normalize(vec3(direction.x,direction.y,direction.z));

  float x_max = mh.lengthOfVector(u);
  float y_max = mh.lengthOfVector(v);

  float  tDeltaX = (x_max*2) / width;
  float tDeltaY = (y_max*2) / height;

  //cout << "deltax: " << tDeltaX << " deltay: " << tDeltaY << endl;

  //z as function of x
  float zasx = direction.z / direction.x;

  float nearZ = ip.z;
  float farZ = zasx * tDeltaX;

  vec2 pos = vec2(ip.x,ip.y);
  vec2 cell;
  int step_x = diff.x >= 0 ? 1 : -1;
  int step_y = diff.y >= 0 ? 1 : -1;

  while(pos.x > -x_max && pos.x < x_max &&
      pos.y > - y_max && pos.y < y_max) {

    cell = vec2(floor((pos.x-x_max)*width),floor((pos.y-y_max)*height));

    float z0 = mh.lengthOfVector(texture->getColorAt(cell.x,cell.y));
    float z1 = mh.lengthOfVector(texture->getColorAt(cell.x+step_x,cell.y));
    float z2 = mh.lengthOfVector(texture->getColorAt(cell.x,cell.y+step_y));
    float z3 = mh.lengthOfVector(texture->getColorAt(cell.x+step_x,cell.y+step_y));

    z0 = z0*elevation/max_elevation;
    z1 = z1*elevation/max_elevation;
    z2 = z2*elevation/max_elevation;
    z3 = z3*elevation/max_elevation;


     //cout << "cell: " << cell.x << " " << cell.y << endl;

     if(glm::min(nearZ,farZ) <= glm::min(glm::min(z0,z1),glm::min(z2,z3))) {
       //Intersection

       vec3 v0,v1,v2;

       float dx = (x_max*2)/width;
       float dy = (y_max*2)/height;

       v0 = vec3(-1,-1,0);
       v1 = vec3(1,-1,0);
       v2 = vec3(1,1,0);
       //v0 = vec3(cell.x*dx-x_max,cell.y*dy-y_max,0);
       //v1 = vec3((cell.x+step_x)*dx-x_max,cell.y*dy-y_max,0);
       //v2 = vec3(cell.x*dx-x_max,(cell.y+step_y)*dy-y_max,0);

       vector<vec3*> vertices;

       vertices.push_back(&v0);
       vertices.push_back(&v1);
       vertices.push_back(&v2);

       vec3 normal = vec3(cross(v1-v0,v2-v0));

       vector<vec3*> normals;

       normals.push_back(&normal);
       normals.push_back(&normal);
       normals.push_back(&normal);

       vector<vec3*> tex_coords;

       Triangle** triangles;
       triangles[0] = new Triangle(vertices,normals,tex_coords,0);

       return triangleIntersection(ray,triangles);


       return IAccDataStruct::IntersectionData(0, vec3(pos.x,pos.y,0), vec3(0,1,0), vec2(),vec3(),vec3());
       //cout << "test: " << pos.x/x_max << endl;
     }

     pos.x += tDeltaX;
     pos.y += tDeltaY;
     nearZ = farZ;
     farZ *= tDeltaX;
  }

  return IAccDataStruct::IntersectionData(IAccDataStruct::IntersectionData::NOT_FOUND, vec3(), vec3(), vec2(),vec3(),vec3());

}

IAccDataStruct::IntersectionData Heightmap::triangleIntersection(Ray ray, Triangle** triangles) {
  vec3 o = ray.getPosition();
  vec3 d = ray.getDirection();

  Triangle* closest_tri = NULL;
  //float closest_dist = -1;
  vec3 closest_pos;

  float closest_t = numeric_limits<float>::infinity( );

  for(int i = 0; i < 1; i++) {
    Triangle* cur_triangle = triangles[i];
    const vector<vec3*> vertices = cur_triangle->getVertices();
    vec3 v0 = *(vertices[0]);
    vec3 v1 = *(vertices[1]);
    vec3 v2 = *(vertices[2]);

    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    vec3 s = o - v0;

    vec3 dxe2 = cross(d, e2);
    vec3 sxe1 = cross(s, e1);
    vec3 res = ( 1.0f /  dot(dxe2, e1) ) *
        vec3( dot(sxe1, e2), dot(dxe2, s), dot(sxe1, d) );

    float t = res.x;
    float u = res.y;
    float v = res.z;

    float dist = glm::distance(o, closest_pos);

    if(u >= 0 && v >= 0 && u + v <= 1) {  // Intersection!
      if(t > 0 && t < closest_t) {
        closest_tri = cur_triangle;
        closest_pos = o + t * d;
        //closest_dist = dist;
        closest_t = t;
      }
    }
  }

  if(closest_t == numeric_limits<float>::infinity( )) {
    return IAccDataStruct::IntersectionData(IAccDataStruct::IntersectionData::NOT_FOUND, vec3(), vec3(), vec2(),vec3(),vec3());
  }

  vec3 v1v0 = *(closest_tri->getVertices()[1]) - *(closest_tri->getVertices()[0]);
  vec3 v2v1 = *(closest_tri->getVertices()[2]) - *(closest_tri->getVertices()[1]);
  vec3 v2v0 = *(closest_tri->getVertices()[2]) - *(closest_tri->getVertices()[0]);
  vec3 pv0 = closest_pos - *(closest_tri->getVertices()[0]);
  vec3 pv1 = closest_pos - *(closest_tri->getVertices()[1]);

  float a = length(cross(v1v0, v2v0));
  float a0 = length(cross(v2v1, pv1))/a;
  float a1 = length(cross(v2v0, pv0))/a;
  float a2 = length(cross(v1v0, pv0))/a;

  vec3 inter_normal = a0 * *(closest_tri->getNormals()[0]) +
      a1 * *(closest_tri->getNormals()[1]) +
      a2 * *(closest_tri->getNormals()[2]);

  vec3 inter_tex =    a0 * *(closest_tri->getTextures()[0]) +
      a1 * *(closest_tri->getTextures()[1]) +
      a2 * *(closest_tri->getTextures()[2]);

  vec3 v1 = v1v0;
  vec3 v2 = v2v1;

  if(v1.length() > v2.length()) {
    v1 = v2v1;
    v1 = v1v0;
  }
  if(v2.length() > v2v0.length()) {
    v2 = v2v0;
  }

  return IAccDataStruct::IntersectionData(closest_tri->getMaterial(), closest_pos, glm::normalize(inter_normal), vec2(inter_tex),
      v1,v2);
}

}
