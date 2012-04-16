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
#include "../Settings.h"

namespace raytracer {

class KDTreeDataStruct : public IAccDataStruct {
public:
  KDTreeDataStruct(Settings* settings);
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
  struct SAHValues
  {
    //Axis of the split.
    int axis;
    //Split position.
    float split,cost;
    //Decision variable. It decides whether the triangles are placed to
    //the left or right branch.
    Side side;
  };
  struct TravElem
  {
    size_t nodeIndex;
    float t_near;
    float t_far;
  };

  struct Event
  {
    float plane;
    int type;

    Event()
    {
      plane = INFINITY;
      type = -1;
    }

    Event(float val, int typ)
    {
      plane = val;
      type = typ;
    }
    inline bool operator()(Event e1, Event e2)
    {
      if (e1.plane < e2.plane)
      {
        return true;
      }
      else if (e1.plane == e2.plane)
      {
        return e1.type < e2.type;
      }
      else{
        return false;
      }
    }
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
  class KDTriangle {
  public:
    KDTriangle(Triangle* tri):triangle(tri)
    {
      vector<vec3*> vertices = triangle->getVertices();
      barycenter[0]= (vertices[0]->x + vertices[1]->x + vertices[2]->x)/3;
      barycenter[1]=(vertices[0]->y + vertices[1]->y + vertices[2]->y)/3;
      barycenter[2]=(vertices[0]->z + vertices[1]->z + vertices[2]->z)/3;

      min[0]=(*vertices[0])[0]; min[1] = (*vertices[0])[1]; min[2] = (*vertices[0])[2];

      max[0] = (*vertices[0])[0]; max[1] = (*vertices[0])[1]; max[2] = (*vertices[0])[2];
      for(size_t j=1;j<3;j++){
        for(size_t i=0;i<3;i++){
          float _value = (*vertices[j])[i];
          if(_value<min[i])
            min[i] = _value;
          if(_value>max[i])
            max[i] = _value;
        }
      }
    }
    const vec3& getMax(){return max;}
    const vec3& getMin(){return min;}
    const vec3& getBarycenter(){return barycenter;}
    Triangle* getTriangle(){return triangle;}
  private:
    vec3 min,max,barycenter;
    Triangle* triangle;

  };

  void buildMedianTree(KDNode* node,int depth);
  void buildMedianNode(KDNode* node,int depth);
  void constructWireframe();
  IAccDataStruct::IntersectionData findClosestIntersectionR(KDNode* node,Ray* ray,float min,float max,int depth);

  IAccDataStruct::IntersectionData findClosestIntersectionStack(Ray* ray,float min,float max);
  void quickSort(int* triangles,int top,int bottom,int axis);
  int qsPartition(int* triangles,int top,int bottom,int axis);

  float findMedianSplitting(int* triangles_pos,size_t size,int axis);

  float SAHCost(float P_l, float P_r, int N_l, int N_r);
  std::pair<float, Side> SAH(AABB V,int axis, float split, int N_l, int N_r, int N_p);
  /// This method allow finding the "best" splitting plane.
  SAHValues findPlane(int* triangles,size_t size, AABB volume);

  void buildSAHTree();

  static const float KD_TRAVERSE = 1.0f;
  static const float KD_INTERSECT = 10.0f;
  KDNode* root;
  AABB* aabb;
  vector<AABB*> splitting_list;
  KDTriangle** triangles;
  int* root_triangles;
  size_t triangle_count;
  Settings* settings;
  size_t min_size;

};

}

#endif /* KDTREEDATASTRUCT_HPP_ */
