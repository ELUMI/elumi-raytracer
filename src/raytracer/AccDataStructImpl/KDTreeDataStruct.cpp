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
    Triangle** _triangles = node->getTriangles();
    ArrayDataStruct* triangle_test =new ArrayDataStruct();
    vector<Triangle*> _temp(node->getSize());
    _temp.assign(node->getSize(),*_triangles);
    //triangle_test->setData(_temp,aabb);
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
  root = constructTree(triangles,triangle_count,0);
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
 * Recursive loop for creating the splitting planes
 */
KDTreeDataStruct::KDNode* KDTreeDataStruct::constructTree(Triangle** triangles,int size,int depth){
  //TODO: recursive loop and create splitting planes etc.
  if(size<=0) // Can add depth max or anything other
    return NULL;
  else if(size<=12){
    KDNode* node = new KDNode();
    node->setTriangles(triangles);
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
    Triangle** left_vert = new Triangle*[size/2+left_extra];
    copy(triangles,triangles+size/2+left_extra,left_vert);
    Triangle** right_vert= new Triangle*[size/2+size%2];

    //memcpy(left_vert,triangles,sizeof(left_vert)); //memcpy(right_vert,triangles+size/2,sizeof(right_vert));
    cout << left_vert[1]->getMax(axis);
    KDNode* node = new KDNode();
    node->setSplit(median);
    node->setTriangles(triangles);
    node->setSize(size+1);


    node->setLeft(KDTreeDataStruct::constructTree(left_vert,size/2+left_extra,depth+1));
    node->setRight(KDTreeDataStruct::constructTree(right_vert,size/2+size%2,depth+1));
    if(node->getLeft()==NULL&&node->getRight()==NULL)
      node->setLeaf(true);

    return node;
  }
}
void KDTreeDataStruct::setData(std::vector<Triangle*> triangles,AABB* aabb){
//  delete(KDTreeDataStruct::triangles);
  //TODO:FIXME
  KDTreeDataStruct::triangles = new Triangle*[triangles.size()];
  copy(triangles.data(),triangles.data()+triangles.size(),KDTreeDataStruct::triangles);
  KDTreeDataStruct::triangle_count = triangles.size();
  KDTreeDataStruct::aabb=aabb;
}
}
