/*
 * KDTreeDataStruct.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: DmZ
 */



#include <vector>
#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stack>

#include "../IAccDataStruct.h"
#include "KDTreeDataStruct.hpp"
#include "ArrayDataStruct.h"
#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"

using namespace glm;

namespace raytracer {

KDTreeDataStruct::KDTreeDataStruct(){
  root = new KDNode();
}

KDTreeDataStruct::KDTreeDataStruct(std::vector<Triangle*> triangles){
}
KDTreeDataStruct::~KDTreeDataStruct() {
  //delete(triangles);
}

IAccDataStruct::IntersectionData KDTreeDataStruct::findClosestIntersectionStack(Ray* ray,float _min,float _max){
  stack<KDNode*> node_stack;
  stack<float> min_stack;
  stack<float> max_stack;

  node_stack.push(root);
  min_stack.push(_min);
  max_stack.push(_max);

  IntersectionData hit = IntersectionData(IntersectionData::NOT_FOUND, vec3(), vec3(), vec2());
  while(!node_stack.empty()){
    KDNode* node = node_stack.top();
    float min = min_stack.top();
    float max = max_stack.top();

    node_stack.pop();
    min_stack.pop();
    max_stack.pop();

    int axis = 0;
    while(!node->isLeaf()){
      axis = node->getAxis();
      float t = (node->getSplit()-ray->getPosition()[axis]) / ray->getDirection()[axis];
      float dir_t = ray->getDirection()[axis]*t;
      //vec3 p = ray->getPosition()+ray->getDirection()*t;
      float split = t;

      if(t<0||split>=max){
        node = (0 < dir_t ? // ray.pos[axis] < ray.pos[axis] + ray.dir*t
            node->getLeft() : node->getRight());
      }
      else if(split<=min){
        node = (0 > dir_t?
            node->getLeft() : node->getRight());
      }
      else{
        KDNode* first = (0 < dir_t ?
            node->getLeft() : node->getRight());
        KDNode* second =(0 >= dir_t ?
            node->getLeft() : node->getRight());
        node_stack.push(second);
        min_stack.push(split);
        max_stack.push(max);
        node = first;
        max = split;
      }
    }
    vec3 o = ray->getPosition();
    vec3 d = ray->getDirection();

    Triangle* closest_tri = NULL;
    vec3 closest_pos;
    float closest_dist;
    float closest_t = numeric_limits<float>::infinity( );
    int parent_axis = axis;
    for(size_t i=0;i<node->getSize();i++){
      Triangle* cur_triangle = triangles[parent_axis][node->getStart()+i];
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
          closest_dist = dist;
          closest_t = t;
        }
      }
    }
    if(closest_t != numeric_limits<float>::infinity( )) {

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

//      if(closest_dist<max)
        return IntersectionData(closest_tri->getMaterial(), closest_pos, glm::normalize(inter_normal), vec2(inter_tex));
    }
  }
  return hit;
}

IAccDataStruct::IntersectionData
KDTreeDataStruct::findClosestIntersection(Ray ray) {
  float min,max;
  if(aabb->intersect(ray,min,max)){
    return findClosestIntersectionStack(&ray,min,max);
  }
  return (IntersectionData(IntersectionData::NOT_FOUND, vec3(), vec3(), vec2()));
}

void KDTreeDataStruct::build(){
  time_t start,end;
  time (&start);
  constructTreeStack();
  //root = constructTree(triangles_vec,triangle_count,0);
  time (&end);
  double dif = difftime (end,start);
  cout << "Tree compete : " << dif;
  cout << endl;
}

void KDTreeDataStruct::quickSort(Triangle** triangles,int top,int bottom,int axis){

  int middle = 0;
  if(top<bottom){
    middle = qsPartition(triangles,top,bottom,axis);
    quickSort(triangles,top,middle,axis);
    quickSort(triangles,middle+1,bottom,axis);
  }
}
int KDTreeDataStruct::qsPartition(Triangle** triangles,int top,int bottom,int axis){
  float t=triangles[top]->getBarycenter(axis);
  int i = top - 1;
  int j = bottom + 1;
  Triangle* temp;
  do
  {
    do
    {
      j--;
    }while (t < triangles[j]->getBarycenter(axis));

    do
    {
      i++;
    } while (t > triangles[i]->getBarycenter(axis));

    if (i < j)
    {
      temp = triangles[j];
      triangles[j] = triangles[i];
      triangles[i] = temp;
    }
  }while (i < j);
  return j;
}

/**
 * Doing a stack implementation instead of a recursive approach because the call_stack may end up loosing some data
 * or giving corrupt data. This way we will never leave the method and therefor don't risk damaging the call_stack
 */
void KDTreeDataStruct::constructTreeStack(){

  stack<int> start_node;
  stack<int> end_node;
  stack<int> depth_node;

  start_node.push(0);
  end_node.push(triangle_count-1);
  depth_node.push(0);

  stack< KDNode* > node_stack;
  root = new KDNode();
  node_stack.push(root);

  while(!node_stack.empty()){
    int depth = depth_node.top();
    int axis = depth%3;
    Triangle** triangles = KDTreeDataStruct::triangles[axis];

    int start = start_node.top();
    int end = end_node.top();
    int size = end-start+1; // + 1 since start begins at 0
    start_node.pop();
    end_node.pop();
    depth_node.pop();

    KDNode* node = node_stack.top();
    node_stack.pop();

    // TODO: Should make size and depth check values so they can be set from a easier location. TODO: test different values for size and depth
    if(size<=4 || depth>20){
      node->setStart(start);
      node->setEnd(end);
      axis-=1;
      if(axis==-1)
        axis=2;
      node->setAxis(axis);
      node->setLeaf(true);
    }
    else{

      //Pick median triangle and just select the barycenter, or mean of the two in the middle.
      float median = (triangles[start+size/2-1+size%2]->getBarycenter(axis)+triangles[start+size/2]->getBarycenter(axis))/2;
      // Count the triangles on both sides
      int left_intersect=start+size/2+size%2;
      size_t left_extra=0;
      while(left_intersect<=end){
        float min_triangle = triangles[left_intersect]->getMin(axis);
        if(min_triangle<=median){
          left_extra++;
          left_intersect++;
        }
        else break;
      }
      int right_intersect=start+size/2-1;
      size_t right_extra=0;
      while(right_intersect>=start){
        float max_triangle = triangles[right_intersect]->getMax(axis);
        if(max_triangle>=median){
          right_extra++;
          right_intersect--;
        }
        else break;
      }

      // Set the split values and position in the array in which contains the node triangles
      node->setSplit(median);
      node->setStart(start);
      node->setAxis(axis);
      node->setEnd(end);

      KDNode* left = new KDNode();
      KDNode* right = new KDNode();

      // Start position for the right and left node
      // Traverse the left node first, so add the right start point first
      start_node.push(start+size/2-right_extra);
      start_node.push(start);

      // End position for the right end left node
      end_node.push(end);
      end_node.push(start+size/2+left_extra-(1-size%2));

      // Push both children to the stack, make sure to put left on bottom to traverse fist
      node_stack.push(right);
      node_stack.push(left);

      depth_node.push(depth+1);
      depth_node.push(depth+1);

      node->setLeft(left);
      node->setRight(right);
    }
  }

}

/**
 * Creates three triangle lists that are sorted after x,y,z axises. This is mush faster than
 * sorting every time we create a node. The three triangle lists are also the only triangle lists
 * that need to be created because each node has an axis, start and end position.
 */
void KDTreeDataStruct::setData(Triangle** triangles,size_t size,AABB* aabb){
  KDTreeDataStruct::triangles = new Triangle**[3];
  for(size_t n=0;n<3;n++){
    KDTreeDataStruct::triangles[n] = new Triangle*[size];
    for(size_t t=0;t<size;t++){
      KDTreeDataStruct::triangles[n][t]= triangles[t];
    }
    quickSort(KDTreeDataStruct::triangles[n],0,size-1,n);
//    for(size_t t=0;t<size;t++){
//      cout << KDTreeDataStruct::triangles[n][t]->getBarycenter(n) << endl;
//    }
  }
  KDTreeDataStruct::triangle_count = size;
  KDTreeDataStruct::aabb=aabb;
  build();
}
}
