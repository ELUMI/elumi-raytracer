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
IAccDataStruct::IntersectionData
KDTreeDataStruct::findClosestIntersectionR(KDNode* node,Ray* ray,float min,float max, int depth){
  if(node->isLeaf()){
    ArrayDataStruct* triangle_test =new ArrayDataStruct();
    Triangle** temp_tri = new Triangle*[node->getSize()];
    for(size_t i=0;i<node->getSize();i++){
      temp_tri[i] = new Triangle();
      temp_tri[i] = KDTreeDataStruct::triangles[(depth-1)%3][i];
    }
    triangle_test->setData(temp_tri,node->getSize(),aabb);
    IntersectionData temp = triangle_test->findClosestIntersection(*ray);
    delete[] temp_tri;
    delete triangle_test;
    return temp;
  }
  int axis = depth%3;
  float split = (node->getSplit()-ray->getPosition()[axis])/ray->getDirection()[axis];
  KDNode* near = (ray->getPosition()[axis]<split?node->getLeft():node->getRight());
  KDNode* far = (near==node->getLeft()?node->getRight():node->getLeft());

  if(split>max){
    return findClosestIntersectionR(near,ray,min,max,depth+1);
  }
  else if(split<min){
    if(split>0){
      return findClosestIntersectionR(far,ray,min,max,depth+1);
    }
    else if(split<0){
      return findClosestIntersectionR(near,ray,min,max,depth+1);
    }
    else if(ray->getDirection()[axis]<0){
      return findClosestIntersectionR(far,ray,min,max,depth+1);
    }
    else{
      return findClosestIntersectionR(near,ray,min,max,depth+1);
    }
  }
  else{
    if(split>0){
      IntersectionData temp =  findClosestIntersectionR(near,ray, min, split,depth+1);
      return (temp.material!=IntersectionData::NOT_FOUND?temp:findClosestIntersectionR(near,ray, split, max,depth+1));
    }
    else{
      return findClosestIntersectionR(near,ray,split, max,depth+1);//case G
    }
  }

}

IAccDataStruct::IntersectionData
KDTreeDataStruct::findClosestIntersection(Ray ray) {
  float min,max;
  if(aabb->intersect(ray,min,max)){
    //run intersection;
      return findClosestIntersectionR(root,&ray,min,max,0);
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
  float t=triangles[top]->getMax(axis);
  int i = top - 1;
  int j = bottom + 1;
  Triangle* temp;
  do
  {
    do
    {
      j--;
    }while (t > triangles[j]->getMax(axis));

    do
    {
      i++;
    } while (t <triangles[i]->getMax(axis));

    if (i < j)
    {
      temp = triangles[i];
      triangles[i] = triangles[j];
      triangles[j] = temp;
    }
    //delete &t;
  }while (i < j);
  return j;
}

/**
 * Doing a stack implementation instead of a recursive approach because the call_stack may end up loosing some data
 * or giving corrupt data. This way we will never leave the method and therefor don't risk damaging the call_stack
 */
void KDTreeDataStruct::constructTreeStack(){
  int depth = 0;

  stack<int> start_node;
  stack<int> end_node;

  start_node.push(0);
  end_node.push(triangle_count);

  stack< KDNode* > node_stack;
  root = new KDNode();
  node_stack.push(root);

  while(!node_stack.empty()){
    int axis = depth%3;
    Triangle** triangles = KDTreeDataStruct::triangles[axis];

    size_t start = start_node.top();
    size_t end = end_node.top();
    size_t size = end-start;
    start_node.pop();
    end_node.pop();

    KDNode* node = node_stack.top();
    node_stack.pop();

    // TODO: Should make size and depth check values so they can be set from a easier location. TODO: test different values for size and depth
    if(size<=6 || depth>20){
      node->setStart(start);
      node->setEnd(end);
      node->setLeaf(true);
      depth--;
    }
    else{

      //Pick median triangle and just select one of the points with the axis
      float median = triangles[(int)(size/2)]->getMax(axis);

      // Sort with max vertice of each triangle, so we will always get all triangles on right node correct
      // but some of the triangles on the right plane may also be on the left plane so need to check for those
      // This approach will make the tree a little unbalanced but it's pretty fast
      size_t left_intersect=size/2+1;
      size_t left_extra=0;
      while(left_intersect<size){
        if(triangles[left_intersect]->getMin(axis)<median){
          left_extra++;
          left_intersect++;
        }
        else break;
      }

      // Set the split values and position in the array in which contains the node triangles
      node->setSplit(median);
      node->setStart(start);
      node->setEnd(end);

      KDNode* left = new KDNode();
      KDNode* right = new KDNode();

      // Start position for the right and left node
      // Traverse the left node first, so add the right start point first
      start_node.push(start+size/2);
      start_node.push(start);

      // End position for the right end left node
      end_node.push(end);
      end_node.push(start+size/2+left_extra);

      // Push both children to the stack, make sure to put left on bottom to traverse fist
      node_stack.push(right);
      node_stack.push(left);

      node->setLeft(left);
      node->setRight(right);
      depth++;
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
      KDTreeDataStruct::triangles[n][t] = new Triangle();
      KDTreeDataStruct::triangles[n][t]= triangles[t];
    }
    quickSort(KDTreeDataStruct::triangles[n],0,size-1,n);
  }
  KDTreeDataStruct::triangle_count = size;
  KDTreeDataStruct::aabb=aabb;
  build();
}
}
