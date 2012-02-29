/*
 * KDTreeDataStruct.hpp
 *
 *  Created on: Feb 22, 2012
 *      Author: DmZ
 */

#ifndef KDTREEDATASTRUCT_HPP_
#define KDTREEDATASTRUCT_HPP_

#include <vector>
#include <math.h>

#include "../utilities/Triangle.h"
#include "../utilities/Ray.h"
#include "../IAccDataStruct.h"
#include "../common.hpp"

namespace raytracer {

class KDTreeDataStruct : public IAccDataStruct {
public:
  KDTreeDataStruct();
  KDTreeDataStruct(std::vector<Triangle*> triangles);
  virtual ~KDTreeDataStruct();

  IAccDataStruct::IntersectionData* findClosestIntersection(Ray ray);
  void setData(std::vector<Triangle*> triangles,AABB* aabb);
  void build();


private:
  class KDNode{
  public:
    KDNode():left(NULL),right(NULL),leaf(false){};
    void setLeft(KDNode* left){KDNode::left=left;}
    void setRight(KDNode* right){KDNode::right=right;}
    KDNode* getLeft(){return KDNode::left;}
    KDNode* getRight(){return KDNode::right;}
    bool isLeaf(){return KDNode::leaf;}
    void setLeaf(bool leaf){KDNode::leaf=leaf;}
    void setTriangles(Triangle* triangles){KDNode::triangles = triangles;}
    Triangle* getTriangles(){return triangles;}
    void setSize(size_t size){KDNode::size=size;}
    size_t getSize(){return KDNode::size;}
    float getSplit(){return KDNode::split;}
    void setSplit(float split){KDNode::split=split;}
  private:
    KDNode* left;
    KDNode* right;
    Triangle* triangles;
    size_t size;
    float split;
    bool leaf;
  };
  KDNode* constructTree(Triangle* triangles,int size,int depth);
  IAccDataStruct::IntersectionData* findClosestIntersectionR(KDNode* node,Ray* ray,float min,float max,int depth);
  void quickSort(Triangle* triangles,int top,int bottom,int axis);
  int qsPartition(Triangle* triangles,int top,int bottom,int axis);


  KDNode* root;
  AABB* aabb;
  Triangle* triangles;
  size_t triangle_count;
  float lookUpTable[3];

};

}

#endif /* KDTREEDATASTRUCT_HPP_ */
