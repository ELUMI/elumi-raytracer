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
  vector<AABB*>& getAABBList(){return KDTreeDataStruct::splitting_list;}

private:
  enum Side{
    LEFT=0,RIGHT=1,ROOT=2
  };
  class KDNode{
  public:
    KDNode():left(NULL),right(NULL),leaf(false){};
    void setLeft(KDNode* left){KDNode::left=left;}
    void setRight(KDNode* right){KDNode::right=right;}
    void setLeaf(bool leaf){KDNode::leaf=leaf;}
    void setAxis(int axis){KDNode::axis=axis;}
    void setSplit(float split){KDNode::split=split;}
    void setSize(size_t size){KDNode::size=size;}
    void setTriangles(int* triangles){KDNode::triangles=triangles;}
    void setSide(Side side){KDNode::side=static_cast<Side>(side);}
    KDNode* getLeft(){return KDNode::left;}
    KDNode* getRight(){return KDNode::right;}
    size_t getSize(){return KDNode::size;}
    int* getTriangles(){return KDNode::triangles;}
    bool isLeaf(){return KDNode::leaf;}

    int getAxis(){return KDNode::axis;}
    float getSplit(){return KDNode::split;}
    Side getSide(){ return static_cast<Side>(KDNode::side);}
  private:
    KDNode* left;
    KDNode* right;
    int* triangles;
    size_t size;
    int axis;
    float split;
    unsigned char side;
    bool leaf;
  };
  class SortableTriangle : Triangle {
  public:
    SortableTriangle(Triangle* triangle){

    }
  private:
    float min,max,baryCenter;

  };
//  KDNode* constructTree(vector<Triangle*> triangles,int size,int depth);
  void constructTreeStack();
  void constructWireframe();
  IAccDataStruct::IntersectionData findClosestIntersectionR(KDNode* node,Ray* ray,float min,float max,int depth);

  IAccDataStruct::IntersectionData findClosestIntersectionStack(Ray* ray,float min,float max);
  void quickSort(int* triangles,int top,int bottom,int axis);
  int qsPartition(int* triangles,int top,int bottom,int axis);


  KDNode* root;
  AABB* aabb;
  vector<AABB*> splitting_list;
  Triangle** triangles;
  int* root_triangles;
  size_t triangle_count;

};

}

#endif /* KDTREEDATASTRUCT_HPP_ */
