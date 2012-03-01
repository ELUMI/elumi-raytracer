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

  IAccDataStruct::IntersectionData findClosestIntersection(Ray ray);
  void setData(Triangle** triangles,size_t size,AABB* aabb);
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
    void setStart(size_t start){KDNode::start=start;}
    void setEnd(size_t end){KDNode::end=end;}
    size_t getStart(){return KDNode::start;}
    size_t getEnd(){return KDNode::end;}
    size_t getSize(){return KDNode::end-KDNode::start;}
    float getSplit(){return KDNode::split;}
    void setSplit(float split){KDNode::split=split;}
  private:
    KDNode* left;
    KDNode* right;
    size_t start;
    size_t end;
    float split;
    bool leaf;
  };
//  KDNode* constructTree(vector<Triangle*> triangles,int size,int depth);
  void constructTreeStack();
  IAccDataStruct::IntersectionData findClosestIntersectionR(KDNode* node,Ray* ray,float min,float max,int depth);
  void quickSort(Triangle** triangles,int top,int bottom,int axis);
  int qsPartition(Triangle** triangles,int top,int bottom,int axis);


  KDNode* root;
  AABB* aabb;
  Triangle*** triangles;
  vector<Triangle*> triangles_vec;
  size_t triangle_count;
  float lookUpTable[3];

};

}

#endif /* KDTREEDATASTRUCT_HPP_ */
