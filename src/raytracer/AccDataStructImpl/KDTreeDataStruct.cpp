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

KDTreeDataStruct::KDTreeDataStruct(Settings* settings){
  KDTreeDataStruct::settings = settings;
  triangles = 0;
  root_triangles = 0;
}

KDTreeDataStruct::KDTreeDataStruct(std::vector<Triangle*> triangles){
}
KDTreeDataStruct::~KDTreeDataStruct() {
  if(KDTreeDataStruct::triangles) {
    for(size_t t=0; t<triangle_count; ++t){
      delete triangles[t];
    }
    delete [] KDTreeDataStruct::triangles;
  }
  //if(root_triangles)
  //  delete [] root_triangles;
}

IAccDataStruct::IntersectionData KDTreeDataStruct::findClosestIntersectionStack(Ray* ray,float _min,float _max){
  stack<KDNode*> node_stack;
  stack<float> min_stack;
  stack<float> max_stack;

  node_stack.push(&root);
  min_stack.push(_min);
  max_stack.push(_max);

  // Speed optimisation
  float ray_dir[3];
  for(size_t axis=0;axis<3;axis++){
    ray_dir[axis] = 1 / ray->getDirection()[axis];
  }
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
      float dir = (node->getSplit()-ray->getPosition()[axis]); // pos[axis] differance between ray and split
      float split_t = dir * ray_dir[axis]; // t steps to split intersection

      if(split_t<0||split_t>max){ // If ray don't intersect split or only intersect one side
        node = (dir > 0?
            node->getLeft() : node->getRight());
      }
      else if(split_t<min){ // Distance from ray to split < than distance from ray to min
        node = (dir <= 0?
            node->getLeft() : node->getRight());
      }
      else{
        KDNode* first = (dir > 0 ?
            node->getLeft() : node->getRight());
        KDNode* second =(first == node->getRight() ?
            node->getLeft() : node->getRight());
        node_stack.push(second);
        min_stack.push(split_t);
        max_stack.push(max);
        node = first;
        max = split_t;
      }
    }
    vec3 o = ray->getPosition();
    vec3 d = ray->getDirection();

    Triangle* closest_tri = NULL;
    vec3 closest_pos;
    float closest_t = numeric_limits<float>::infinity( );

    int* node_triangles_pos = node->getTriangles();
    for(size_t i=0;i<node->getSize();i++){
      Triangle* cur_triangle = triangles[node_triangles_pos[i]]->getTriangle();
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

      if(u >= 0 && v >= 0 && u + v <= 1) {  // Intersection!
        if(t > 0 && t < closest_t) {
          closest_tri = cur_triangle;
          closest_pos = o + t * d;
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

        vec3 v1 = v1v0;
        vec3 v2 = v2v1;

        if(v1.length() > v2.length()) {
          v1 = v2v1;
          v1 = v1v0;
        }
        if(v2.length() > v2v0.length()) {
          v2 = v2v0;
        }

      return IntersectionData(closest_tri, closest_tri->getMaterial(), closest_pos, glm::normalize(inter_normal), vec2(inter_tex),
            v1,v2);
    }
  }
  return IntersectionData::miss();
}

IAccDataStruct::IntersectionData
KDTreeDataStruct::findClosestIntersection(Ray ray) {
  float min,max;
  if(aabb.intersect(ray,min,max)){ //TODO: Bugg när kameran ligger på linjen för första splitting planet
    return findClosestIntersectionStack(&ray,min,max);
  }
  return IntersectionData::miss();
}

void KDTreeDataStruct::build(){
  // Launch threads
//  int nr_threads = boost::thread::hardware_concurrency();
//  boost::thread threads[nr_threads];
//  for (int i = 0; i < nr_threads; ++i) {
//    threads[i] = boost::thread(
//        boost::bind(&KDTreeDataStruct::constructTreeStack, this, RIGHT));
//  }
//
//  // Wait for threads to complete
//  for (int i = 0; i < nr_threads; ++i) {
//    threads[i].join();
//  }


  constructTreeStack(LEFT);

  if(settings->wireframe==1){
    constructWireframe();
  }
}

/**
 * Doing a stack implementation instead of a recursive approach because the call_stack may end up loosing some data
 * or giving corrupt data. This way we will never leave the method and therefor don't risk damaging the call_stack
 */
void KDTreeDataStruct::constructTreeStack(Side side){

  stack<int> depth_node;
  stack<size_t> size_node;
  stack<int*> triangle_pos_node;
  stack< KDNode* > node_stack;

  root = KDNode();
  root.setSide(KDTreeDataStruct::ROOT);

  depth_node.push(0);


  quickSort(root_triangles,0,triangle_count-1,0);
  triangle_pos_node.push(root_triangles);
  size_node.push(triangle_count);
  node_stack.push(&root);

  size_t min_size = (int)log10(triangle_count)*8;

  while(!node_stack.empty()){
    int depth = depth_node.top();
    int axis = depth%3;
    int* triangles_pos = triangle_pos_node.top();

    size_t size = size_node.top();

    size_node.pop();
    depth_node.pop();
    triangle_pos_node.pop();

    KDNode* node = node_stack.top();
    node_stack.pop();

    // TODO: Should make size and depth check values so they can be set from a easier location. TODO: test different values for size and depth
    if(size<=min_size|| depth>20){
      node->setSize(size);
      node->setTriangles(triangles_pos);
      node->setAxis(axis);
      node->setLeaf(true);
    } else {

      //Pick median triangle and just select the barycenter, or mean of the two in the middle.
      float first_median = triangles[triangles_pos[size/2-1+size%2]]->getBarycenter(axis);
      float second_median = triangles[triangles_pos[size/2]]->getBarycenter(axis);
      float median = (first_median+second_median)/2;

      vector<int> right_tri,left_tri;

      for(size_t i=0;i<size;i++){
        float min_triangle = KDTreeDataStruct::triangles[triangles_pos[i]]->getMin(axis);
        float max_triangle = KDTreeDataStruct::triangles[triangles_pos[i]]->getMax(axis);
        if(min_triangle<median){
          left_tri.push_back(i);
        }
        if(max_triangle>median){
          right_tri.push_back(i);
        }
      }

      int child_axis = (axis+1)%3;
      int* left_triangles = new int[left_tri.size()];
      for(size_t t=0;t<left_tri.size();t++){
        left_triangles[t]= triangles_pos[left_tri[t]];
      }
      quickSort(left_triangles,0,left_tri.size()-1,child_axis);
      int* right_triangles = new int[right_tri.size()];
      for(size_t t=0;t<right_tri.size();t++){
        right_triangles[t]= triangles_pos[right_tri[t]];
      }
      quickSort(right_triangles,0,right_tri.size()-1,child_axis);

      delete [] triangles_pos;

      // Set the split values and position in the array in which contains the node triangles
      node->setSplit(median);
      node->setAxis(axis);

      KDNode* left = new KDNode();
      KDNode* right = new KDNode();

      left->setSide(KDTreeDataStruct::LEFT);
      right->setSide(KDTreeDataStruct::RIGHT);

      size_node.push(right_tri.size());
      size_node.push(left_tri.size());

      triangle_pos_node.push(right_triangles);
      triangle_pos_node.push(left_triangles);

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
void KDTreeDataStruct::constructWireframe(){
  stack<AABB*> aabb_stack;
  stack<KDNode*> node_stack;

  node_stack.push(&root);
  aabb_stack.push(&aabb);

  while(!node_stack.empty()){
    KDNode* node = node_stack.top();
    AABB* s_aabb = aabb_stack.top();

    node_stack.pop();
    aabb_stack.pop();
    Side side = node->getSide();
    if(side==ROOT||side==RIGHT){ // Optimization, only need to save one of the AABB. (Need to construct both left/right to traverse down)
      splitting_list.push_back(s_aabb);
    }

    if(!node->isLeaf()){
      vec3 start,right_end,left_end;
      AABB* left_aabb;
      AABB* right_aabb;

      int axis = node->getAxis();
      vec3 pos = s_aabb->getPos();
      vec3 size = s_aabb->getSize();
      float split = node->getSplit();

      if(axis==0){
        start = vec3(split,pos[1],pos[2]);
        right_end = size - vec3(split-pos.x,0,0);
        left_end = vec3(split-pos.x,size.y,size.z);
      }
      else if(axis==1){
        start = vec3(pos[0],split,pos[2]);
        right_end = size - vec3(0,split-pos.y,0);
        left_end = vec3(size.x,split-pos.y,size.z);
      }
      else{
        start = vec3(pos[0],pos[1],split);
        right_end = size-vec3(0,0,split-pos.z);
        left_end = vec3(size.x,size.y,split-pos.z);
      }

      if(side==LEFT){
        delete s_aabb;
      }
      left_aabb = new AABB(pos,left_end,false,axis); // False -> Don't create any lines
      right_aabb = new AABB(start,right_end,true,axis); // True-> Create only four lines

      aabb_stack.push(right_aabb);
      aabb_stack.push(left_aabb);

      node_stack.push(node->getRight());
      node_stack.push(node->getLeft());
    }
  }
}

/**
 * Creates three triangle lists that are sorted after x,y,z axises. This is mush faster than
 * sorting every time we create a node. The three triangle lists are also the only triangle lists
 * that need to be created because each node has an axis, start and end position.
 */
void KDTreeDataStruct::setData(Triangle** triangles,size_t size,AABB aabb){
  if(KDTreeDataStruct::triangles) {
    for(size_t t=0; t<triangle_count; ++t){
      delete triangles[t];
    }
    delete [] KDTreeDataStruct::triangles;
  }
  if(KDTreeDataStruct::root_triangles)
    delete [] KDTreeDataStruct::root_triangles;

  KDTreeDataStruct::triangles = new SortableTriangle*[size];
  KDTreeDataStruct::root_triangles = new int[size];
  for(size_t t=0;t<size;t++){
    KDTreeDataStruct::triangles[t] = new SortableTriangle(triangles[t]);
    KDTreeDataStruct::root_triangles[t] = t;
  }
  KDTreeDataStruct::triangle_count = size;
  KDTreeDataStruct::aabb=aabb;
  build();
}
void KDTreeDataStruct::quickSort(int* triangles,int top,int bottom,int axis){

  int middle = 0;
  if(top<bottom){
    middle = qsPartition(triangles,top,bottom,axis);
    quickSort(triangles,top,middle,axis);
    quickSort(triangles,middle+1,bottom,axis);
  }
}
int KDTreeDataStruct::qsPartition(int* triangles,int top,int bottom,int axis){
  float t=KDTreeDataStruct::triangles[triangles[top]]->getBarycenter(axis);
  int i = top - 1;
  int j = bottom + 1;
  int temp;
  do
  {
    do
    {
      j--;
    }while (t < KDTreeDataStruct::triangles[triangles[j]]->getBarycenter(axis));

    do
    {
      i++;
    } while (t > KDTreeDataStruct::triangles[triangles[i]]->getBarycenter(axis));

    if (i < j)
    {
      temp = triangles[j];
      triangles[j] = triangles[i];
      triangles[i] = temp;
    }
  }while (i < j);
  return j;
}
}
