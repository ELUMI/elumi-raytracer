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
  Heightmap::texture = new Texture(width,height,data);

  MathHelper mh = MathHelper();

  max_elevation = 0;

  for(unsigned int x = 0; x < width; x++) {
    for(unsigned int y = 0; y < height; y++) {
      float m = mh.lengthOfVector(texture->getColorAt(vec2(x,y)));
      max_elevation < m ? max_elevation = m : max_elevation;
    }
  }

  cout << "max elevation " << max_elevation << endl;

  bb = OBB(vec3(0,0,0),u,v,vec3(0,elevation,0));
}

Heightmap::~Heightmap() {}

IAccDataStruct::IntersectionData Heightmap::getTriangle(Ray ray) {
  //Musgrave algorithm
  //Naive. Drop y axis

  OBB::IntersectionData id = bb.rayIntersection(ray);

  if(!id.intersected)
    return IAccDataStruct::IntersectionData::miss();

  vec3 ip = id.position - position;

  ip = vec3(ip.x,ip.z,ip.y);

  vec3 direction = normalize(ray.getDirection());
  direction = vec3(direction.x,direction.z,direction.y);

  vec3 diff = direction;

  float x_max = length(u);
  float y_max = length(v);

  float cX = x_max*2/width;
  float cY = y_max*2/height;

  float tDeltaX = direction.x*cX;
  float tDeltaY = direction.y*cY;

  float xy = sqrt(tDeltaX*tDeltaX+tDeltaY*tDeltaY);
  float tDeltaZ = xy*direction.z;

  float nearZ = ip.z;
  float farZ = ip.z + tDeltaZ;

  vec2 pos = vec2(ip.x,ip.y);

  vec2 cell = vec2(floor((width/2)*(pos.x+1)),floor((height/2)*(pos.y+1)));

  int signX = diff.x >= 0 ? 1 : -1;
  int signY = diff.y >= 0 ? 1 : -1;

  float e = 0.00001;

  do {

    vec2 cell = vec2(floor((width/2)*(pos.x+1)),floor((height/2)*(pos.y+1)));

    float x1 = cell.x/width;
    float x2 = (cell.x+signX)/width;
    float y1 = (cell.y/height);
    float y2 = (cell.y+signY)/height;

    vec2 c0,c1,c2,c3;
    c0 = vec2(x1,y1);
    c1 = vec2(x2,y1);
    c2 = vec2(x1,y2);
    c3 = vec2(x2,y2);

    float z0 = length(texture->getColorAt(c0));
    float z1 = length(texture->getColorAt(c1));
    float z2 = length(texture->getColorAt(c2));
    float z3 = length(texture->getColorAt(c3));

    z0 = z0*elevation-elevation;
    z1 = z1*elevation-elevation;
    z2 = z2*elevation-elevation;
    z3 = z3*elevation-elevation;

    c0 = vec2(x1*2-1,y1*2-1);
    c1 = vec2(x2*2-1,y1*2-1);
    c2 = vec2(x1*2-1,y2*2-1);
    c3 = vec2(x2*2-1,y2*2-1);

    vec3 v0,v1,v2,v3;

    //Vertices
    v0 = vec3(c0.x,z0,c0.y);
    v1 = vec3(c1.x,z1,c1.y);
    v2 = vec3(c2.x,z2,c2.y);
    v3 = vec3(c3.x,z3,c3.y);

    IAccDataStruct::IntersectionData id = createTrianglesAndIntersect(ray,v0,v1,v2,v3);
    if(!id.missed())
      return id;

    pos.x += tDeltaX;
    pos.y += tDeltaY;
    nearZ = farZ;
    farZ += tDeltaZ;

  } while (nearZ >= -1.0f && farZ <= 1.0f && cell.x >= 0 && cell.x < width && cell.y > 0 && cell.y < height);

  return IAccDataStruct::IntersectionData::miss();

//  do {
//
//    float x1 = cell.x/width;
//    float x2 = (cell.x+signX)/width;
//    float y1 = (cell.y/height);
//    float y2 = (cell.y+signY)/height;
//
//    vec2 c0,c1,c2,c3;
//    c0 = vec2(x1,y1);
//    c1 = vec2(x2,y1);
//    c2 = vec2(x1,y2);
//    c3 = vec2(x2,y2);
//
//    float z0 = length(texture->getColorAt(c0));
//    float z1 = length(texture->getColorAt(c1));
//    float z2 = length(texture->getColorAt(c2));
//    float z3 = length(texture->getColorAt(c3));
//
//    z0 = z0*elevation-elevation;
//    z1 = z1*elevation-elevation;
//    z2 = z2*elevation-elevation;
//    z3 = z3*elevation-elevation;
//
//    if(glm::min(nearZ,farZ) <= glm::max(glm::max(z0,z1),glm::max(z2,z3))) {
//      //return IAccDataStruct::IntersectionData(NULL, 0, vec3(pos.x,pos.y,z0), vec3(0,1,0), vec2(),vec3(),vec3());
//
//      c0 = vec2(x1*2-1,y1*2-1);
//      c1 = vec2(x2*2-1,y1*2-1);
//      c2 = vec2(x1*2-1,y2*2-1);
//      c3 = vec2(x2*2-1,y2*2-1);
//
//      vec3 v0,v1,v2,v3;
//
//      //Vertices
//      v0 = vec3(c0.x,z0,c0.y);
//      v1 = vec3(c1.x,z1,c1.y);
//      v2 = vec3(c2.x,z2,c2.y);
//      v3 = vec3(c3.x,z3,c3.y);
//
//      return IAccDataStruct::IntersectionData(NULL, 0, vec3(pos.x,pos.y,z0), vec3(0,1,0), vec2(),vec3(),vec3());
//
//      return createTrianglesAndIntersect(ray,v0,v1,v2,v3);
//
//    }
//
//    cell.x += signX;
//    cell.y += signY;
//    pos.x += tDeltaX;
//    pos.y += tDeltaY;
//    nearZ = farZ;
//    farZ += tDeltaZ;
//
//  } while (nearZ >= -1.0f && farZ <= 1.0f && cell.x >= 0 && cell.x < width && cell.y > 0 && cell.y < height);
//
  while(pos.x >= -x_max - e && pos.x <= x_max + e &&
      pos.y >= - y_max - e && pos.y <= y_max + e) {

    float x1 = (pos.x+1)/2;
    float x2 = (pos.x+tDeltaX+1)/2;
    float y1 = (pos.y+1)/2;
    float y2 = (pos.y+tDeltaY+1)/2;

    vec2 c0,c1,c2,c3;
    c0 = vec2(x1,y1);
    c1 = vec2(x2,y1);
    c2 = vec2(x1,y2);
    c3 = vec2(x2,y2);

    float z0 = length(texture->getColorAt(c0));
    float z1 = length(texture->getColorAt(c1));
    float z2 = length(texture->getColorAt(c2));
    float z3 = length(texture->getColorAt(c3));

    z0 = z0*elevation-elevation;
    z1 = z1*elevation-elevation;
    z2 = z2*elevation-elevation;
    z3 = z3*elevation-elevation;

    if(glm::min(nearZ,farZ) <= glm::max(glm::max(z0,z1),glm::max(z2,z3))) {
      //Intersection
      vec3 v0,v1,v2,v3;

      float startX = (floor(pos.x*width/2))*2/width;
      float startY = (floor(pos.y*height/2))*2/height;

      c0 = vec2(startX,startY);
      c1 = vec2(startX+tDeltaX,startY);
      c2 = vec2(startX,startY+tDeltaY);
      c3 = vec2(startX+tDeltaX,startY+tDeltaY);

      //Vertices
      v0 = vec3(c0.x,1,c0.y);
      v1 = vec3(c1.x,1,c1.y);
      v2 = vec3(c2.x,1,c2.y);
      v3 = vec3(c3.x,1,c3.y);

      //return IAccDataStruct::IntersectionData(NULL, 0, vec3(pos.x,pos.y,z0), vec3(0,1,0), vec2(),vec3(),vec3());

      return createTrianglesAndIntersect(ray,v0,v1,v2,v3);

      //return IAccDataStruct::IntersectionData(0, vec3(pos.x,pos.y,0), vec3(0,1,0), vec2(),vec3(),vec3());
    }

    pos.x += tDeltaX;
    pos.y += tDeltaY;
    nearZ = farZ;
    farZ += tDeltaZ;
  }

  return IAccDataStruct::IntersectionData::miss();

}

IAccDataStruct::IntersectionData Heightmap::createTrianglesAndIntersect(Ray ray, vec3 v0, vec3 v1, vec3 v2, vec3 v3) {

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

  Triangle* triangle1 = new Triangle(vertices,normals,tex_coords,0);

  vector<vec3*> vertices2;

  vertices2.push_back(&v2);
  vertices2.push_back(&v3);
  vertices2.push_back(&v0);

  vec3 normal2 = vec3(cross(v2-v3,v2-v0));

  vector<vec3*> normals2;

  normals2.push_back(&normal2);
  normals2.push_back(&normal2);
  normals2.push_back(&normal2);

  vector<vec3*> tex_coords2;

  Triangle* triangle2 = new Triangle(vertices2,normals2,tex_coords2,0);

  return triangleIntersection(ray,triangle1,triangle2);
}

IAccDataStruct::IntersectionData Heightmap::triangleIntersection(Ray ray, Triangle* triangle1, Triangle* triangle2) {
  vec3 o = ray.getPosition();
  vec3 d = ray.getDirection();

  Triangle* closest_tri = NULL;
  //float closest_dist = -1;
  vec3 closest_pos;

  float closest_t = numeric_limits<float>::infinity( );

  for(int i = 0; i < 2; i++) {

    Triangle* cur_triangle;

    switch (i) {
      case 0:
        cur_triangle = triangle1;
        break;
      case 1:
        cur_triangle = triangle2;
        break;
      default:
        cur_triangle = triangle1;
        break;
    }

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
    return IAccDataStruct::IntersectionData::miss();
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

  vec3 v1 = v1v0;
  vec3 v2 = v2v1;

  if(v1.length() > v2.length()) {
    v1 = v2v1;
    v1 = v1v0;
  }
  if(v2.length() > v2v0.length()) {
    v2 = v2v0;
  }

  return IAccDataStruct::IntersectionData(NULL, 0, closest_pos, glm::normalize(inter_normal), vec2(0,0));
}

}
