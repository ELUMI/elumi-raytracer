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
    triangle_test->setData(node->getTriangles(),node->getSize(),aabb);
    IntersectionData temp = triangle_test->findClosestIntersection(*ray);
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


void KDTreeDataStruct::constructTreeStack(){
  int depth = 0;
  stack< Triangle** > triangle_stack;
  stack<int> count_stack;
  stack< KDNode* > node_stack;
  triangle_stack.push(triangles);
  count_stack.push(triangle_count);
  root = new KDNode();
  node_stack.push(root);

  while(!node_stack.empty()){
    Triangle** triangles = triangle_stack.top();
    triangle_stack.pop();
    size_t size = count_stack.top();
    count_stack.pop();

    if(size<=6 || depth>20){
      KDNode* node = node_stack.top();
      node_stack.pop();
      node->setTriangles(triangles);
      node->setSize(size);
      node->setLeaf(true);
      depth--;
    }
    else{
      int axis = depth%3;
      quickSort(triangles,0,size-1,axis); // Add a function that sorts for the axis

      //Pick median triangle and just select one of the points with the axis
      float median = triangles[(int)(size/2)]->getMax(axis);
      // find triangles that are on both sides
      size_t left_intersect=size/2+1;
      size_t left_extra=0;
      while(1){
        if(left_intersect<size&&triangles[left_intersect]->getMin(axis)<median){
          left_extra++;
          left_intersect++;
        }
        else break;
      }

      Triangle** left_vert = new Triangle*[size/2+left_extra];
      for(size_t i=0;i<size/2+left_extra;i++){
        left_vert[i] = new Triangle();
        left_vert[i]->set(triangles[i]);
      }
      Triangle** right_vert= new Triangle*[size/2+size%2];
      for(size_t i=0;i<size/2+size%2;i++){
        right_vert[i] = new Triangle();
        right_vert[i]->set(triangles[size/2+i]);
      }
      delete[] triangles;
      KDNode* node = node_stack.top();
      node_stack.pop();
      node->setSplit(median);
      node->setSize(size);

      KDNode* left = new KDNode();
      KDNode* right = new KDNode();

      triangle_stack.push(right_vert);
      triangle_stack.push(left_vert);
      node_stack.push(right);
      node_stack.push(left);
      count_stack.push(size/2+size%2);
      count_stack.push(size/2+left_extra);

      node->setLeft(left);
      node->setRight(right);
      depth++;
    }
  }

}

/**
 * Recursive loop for creating the splitting planes
 */
KDTreeDataStruct::KDNode* KDTreeDataStruct::constructTree(vector<Triangle*> triangles,int size,int depth){
  //TODO: recursive loop and create splitting planes etc.


  if(triangles.size()<=0) // Can add depth max or anything other
    return NULL;
  else if(triangles.size()<=12){
    KDNode* node = new KDNode();
//    node->setTriangles(triangles);
    node->setSize(size);
    node->setLeaf(true);
    return node;
  }
  else{
    int axis = depth%3;
    //quickSort(triangles,0,size-1,axis); // Add a function that sorts for the axis


    //Pick median triangle and just select one of the points with the axis
    float median = triangles[(int)(size/2)]->getMax(axis);
    // find triangles that are on both sides
    size_t left_intersect=size/2+1;
    size_t left_extra=0;
    while(1){
      if(left_intersect<size&&triangles[left_intersect]->getMin(axis)<median){
        left_extra++;
        left_intersect++;
      }
      else break;

    }
//    Triangle** left_vert = new Triangle*[size/2+left_extra];
//    memcpy(left_vert,triangles,sizeof(Triangle**));
//    for(int i=0;i<size/2+left_extra;i++){
//      left_vert[i] = triangles[i];
//    }
//    copy(triangles,triangles+size/2+left_extra,left_vert);
//    Triangle** right_vert= new Triangle*[size/2+size%2];
    //    copy(triangles+size/2,triangles+size/2+size%2,right_vert);
//
//    for(int i=size/2;i<size;i++){
//      right_vert[i] = triangles[i];
//    }
//    memcpy(right_vert,triangles,sizeof(Triangle**));

    vector<Triangle*> left_vert(size/2+left_extra);
    left_vert.assign(size/2+left_extra,*triangles.data());
    vector<Triangle*> right_vert(size/2+size%2);
    right_vert.assign(size/2+size%2,*triangles.data()+size/2);

    KDNode* node = new KDNode();
    node->setSplit(median);
//    node->setTriangles(triangles);
    node->setSize(size);


    node->setLeft(KDTreeDataStruct::constructTree(left_vert,size/2+left_extra,depth+1));
    node->setRight(KDTreeDataStruct::constructTree(right_vert,size/2+size%2,depth+1));
    if(node->getLeft()==NULL&&node->getRight()==NULL)
      node->setLeaf(true);

    return node;
  }
}
void KDTreeDataStruct::setData(Triangle** triangles,size_t size,AABB* aabb){
  KDTreeDataStruct::triangles = new Triangle*[size];
  for(size_t t=0;t<size;t++){
    KDTreeDataStruct::triangles[t] = new Triangle();
    KDTreeDataStruct::triangles[t]->set(triangles[t]);
  }
  KDTreeDataStruct::triangle_count = size;
  KDTreeDataStruct::aabb=aabb;
  build();
}
}
