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

#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/thread/mutex.hpp"

using namespace glm;

namespace raytracer {

KDTreeDataStruct::KDTreeDataStruct(Settings* settings){
  root = new KDNode();
  root->setSide(KDTreeDataStruct::ROOT);
  triangles = NULL;
  root_triangles = NULL;
  KDTreeDataStruct::settings = settings;
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
  if(_min<0)
    _min=0;
  stack<float> min_stack;
  stack<float> max_stack;

  node_stack.push(root);
  min_stack.push(_min);
  max_stack.push(_max);

  // Speed optimisation
  float ray_dir[3];
  for(size_t axis=0;axis<3;axis++){
    ray_dir[axis] = 1 / ray->getDirection()[axis];
  }
  IntersectionData return_value = IAccDataStruct::IntersectionData::miss();
  float best_t = INFINITY;
  while(!node_stack.empty()){
    KDNode* node = node_stack.top();
    float min = min_stack.top();
    float max = max_stack.top();

    node_stack.pop();
    min_stack.pop();
    max_stack.pop();

    int axis = -1;

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
    float closest_t = numeric_limits<float>::infinity();

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

      if(u >= 0.0f && v >= 0.0f && u + v <= 1.0f) {  // Intersection!
        if(t > 0 && t < closest_t) {
          closest_tri = cur_triangle;
          closest_pos = o + t * d;
          closest_t = t;
        }
      }
    }
    if(closest_t != numeric_limits<float>::infinity( ) && closest_t<best_t) {
      best_t = closest_t;
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

//      return_value = IntersectionData(closest_tri->getMaterial(), closest_pos, glm::normalize(inter_normal), vec2(inter_tex),
//            v1,v2);
      return_value = IntersectionData(closest_tri, closest_tri->getMaterial(), closest_pos, glm::normalize(inter_normal), vec2(inter_tex));
    }
  }
  return return_value;
}

IAccDataStruct::IntersectionData
KDTreeDataStruct::findClosestIntersection(Ray ray) {
  float min,max;
  if(aabb.intersect(ray,min,max)){
    return findClosestIntersectionStack(&ray,min,max);
  }
  return IntersectionData::miss();
}

void KDTreeDataStruct::build(){
  switch(settings->tree){
  case 2:
  default:
    KDTreeDataStruct::buildMedianTree(root,0);
    break;
  case 3:
    buildSAHTree();
    break;
  }

  if(settings->wireframe==1){
    constructWireframe();
  }
}

float KDTreeDataStruct::SAHCost(float P_l, float P_r, int N_l, int N_r)
{
  return ((N_l == 0 || N_r == 0) ? 0.8f : 1.0f)
      * (KD_TRAVERSE + KD_INTERSECT * (P_l * N_l + P_r * N_r));
}

/// Main method to evaluate the costs.
/// It returns the value of the heuristic for the given plane.,
/// if the tringles lying exactly in the plane are put on the left side,
/// otherwise the are on the right side.
std::pair<float, KDTreeDataStruct::Side>
  KDTreeDataStruct::SAH(AABB V,int axis, float split, int N_l, int N_r, int N_p)
{
  float SA_V = V.getSurfaceArea();

  std::pair<AABB,AABB> V_split = V.split(split,axis);
  float P_l = V_split.first.getSurfaceArea()/SA_V;
  float P_r = V_split.second.getSurfaceArea()/SA_V;

  float C_l = SAHCost(P_l, P_r, N_l + N_p, N_r);
  float C_r = SAHCost(P_l, P_r, N_l, N_p + N_r);
  return (C_l < C_r ?
      std::pair<float, KDTreeDataStruct::Side>(C_l,LEFT):
      std::pair<float, KDTreeDataStruct::Side>(C_r,RIGHT)
  );
}

KDTreeDataStruct::SAHValues KDTreeDataStruct::findPlane(int* triangles_pos,size_t size, AABB volume){
  SAHValues plane;
  plane.axis = -1;
  plane.split = INFINITY;
  plane.cost = INFINITY;
  int N_l, N_p, N_r;

  std::vector<Event> eventList(1);

  for(size_t k = 0; k < 3; k++)
  {
    eventList.clear();

    for(size_t i = 0; i < size; i++)
    {
      KDTriangle* tri = triangles[triangles_pos[i]];
      float triMin = std::max(tri->getMin()[k], volume.getPos()[k]);
      float triMax = std::min(tri->getMax()[k], volume.getPos()[k]+volume.getSize()[k]);
      if(triMin == triMax)
      {
        eventList.push_back(Event(triMin, 1));
      }
      else
      {
        //Starting event.
        eventList.push_back(Event(triMin, 2));
        //End event.
        eventList.push_back(Event(triMax, 0));
      }
    }

    //Sorting the list, based on the costs.
    sort(eventList.begin(), eventList.end(),Event());

    N_l = 0;
    N_p = 0;
    N_r = size;

    int P_plus, P_minus, P_planar;
    float split;

    size_t i = 0;
    while(i < eventList.size())
    {
      P_plus = 0; P_minus = 0; P_planar = 0;
      //Current cost value.
      split = eventList[i].plane;

      while(i < eventList.size() && eventList[i].plane == split
          && eventList[i].type == 0) // ending (-)
      {
        ++P_minus; ++i;
      }
      while(i < eventList.size() && eventList[i].plane == split
          && eventList[i].type == 1) // planar (|)
      {
        ++P_planar; ++i;
      }
      while(i < eventList.size() && eventList[i].plane == split
          && eventList[i].type == 2) // starting (+)
      {
        ++P_plus; ++i;
      }

      //Found new plane, evaluate SAH for old plane.
      N_p = P_planar;
      N_r -= (P_planar + P_minus);

      std::pair<float, Side> helpCost;
      float _EPS = 0.000001f;
      if(split <= volume.getPos()[k] + _EPS || split + _EPS >= volume.getPos()[k]+volume.getSize()[k])
      {
        helpCost.first = INFINITY;
      }
      else
      {
        helpCost = SAH(volume,k, split , N_l, N_r, N_p);
      }

      //Updating the counts.
      N_l += (P_plus + P_planar);
      N_p = 0;

      ///If the current cost is better than the cost of the plane, the
      //plane is updated.
      if (helpCost.first < plane.cost)
      {
        plane.cost = helpCost.first;
        plane.axis = k;
        plane.split = split;
        plane.side = helpCost.second;
      }

    }
  }

  return plane;
}


void KDTreeDataStruct::buildSAHTree(){
  stack<int> depth_node;
  stack< int* > triangle_stack;
  stack<size_t> size_stack;
  stack< KDNode* > node_stack;
  stack<AABB> V_stack; //AABB

  root->setTriangles(root_triangles);

  depth_node.push(0);
  triangle_stack.push(root_triangles);
  size_stack.push(triangle_count);
  node_stack.push(root);
  V_stack.push(aabb);


  while(!node_stack.empty()){

    int depth = depth_node.top();
    int* triangles_node = triangle_stack.top();
    size_t size = size_stack.top();
    KDNode* node = node_stack.top();
    AABB V = V_stack.top();

    depth_node.pop();
    size_stack.pop();
    triangle_stack.pop();
    V_stack.pop();
    node_stack.pop();

    SAHValues plane = findPlane(triangles_node,size,V);

    // If it is cheaper to not split end
    if(KD_INTERSECT*size<=plane.cost||depth>20){ // Can add depth test also
      node->setSize(size);
      node->setTriangles(triangles_node);
      node->setAxis(plane.axis); //
      node->setLeaf(true);
    }
    else{
      KDNode* left = new KDNode(),*right = new KDNode();

      float split = plane.split;
      vector<int> left_vec,right_vec;
      for(size_t t=0;t<size;t++){
        float min = triangles[triangles_node[t]]->getMin()[plane.axis];
        float max = triangles[triangles_node[t]]->getMax()[plane.axis];

        if(split==min&&split==max){
          if(plane.side == LEFT){
            left_vec.push_back(triangles_node[t]);
          }
          else{
            right_vec.push_back(triangles_node[t]);
          }
        }
        else{
          if(min<split){
            left_vec.push_back(triangles_node[t]);
          }
          if(max > split){
            right_vec.push_back(triangles_node[t]);
          }
        }
      }
      delete[] triangles_node; // Only need triangles in leaf's


      left->setSide(KDTreeDataStruct::LEFT);
      right->setSide(KDTreeDataStruct::RIGHT);

      // Push both children to the stack, make sure to put left on bottom to traverse fist
      node_stack.push(right);
      node_stack.push(left);

      int* triangles_left = new int[left_vec.size()];
      int* triangles_right = new int[right_vec.size()];

      copy( left_vec.begin(), left_vec.begin()+left_vec.size(), triangles_left);
      copy( right_vec.begin(), right_vec.begin()+right_vec.size(), triangles_right);

      triangle_stack.push(triangles_right);
      triangle_stack.push(triangles_left);

      size_stack.push(right_vec.size());
      size_stack.push(left_vec.size());

      std::pair<AABB,AABB> V_side = V.split(plane.split,plane.axis);

      V_stack.push(V_side.second);
      V_stack.push(V_side.first);

      depth_node.push(depth+1);
      depth_node.push(depth+1);

      node->setAxis(plane.axis);
      node->setSplit(plane.split);
      node->setLeft(left);
      node->setRight(right);
    }
  }
}

float KDTreeDataStruct::findMedianSplitting(int* triangles_pos,size_t size,int axis){

  float first_median = triangles[triangles_pos[size/2-1+size%2]]->getBarycenter()[axis];
  float second_median = triangles[triangles_pos[size/2]]->getBarycenter()[axis];
  return (first_median+second_median)/2;
}


/**
 * Doing a stack implementation instead of a recursive approach because the call_stack may end up loosing some data
 * or giving corrupt data. This way we will never leave the method and therefor don't risk damaging the call_stack
 */
void KDTreeDataStruct::buildMedianTree(KDNode* node,int depth){
  if(node==NULL||node->isLeaf())
    return;

  stack<int> depth_node;
  stack<size_t> size_node;
  stack<int*> triangle_pos_node;
  stack< KDNode* > node_stack;

  depth_node.push(depth);

  quickSort(node->getTriangles(),0,node->getSize()-1,0);
  triangle_pos_node.push(node->getTriangles());
  size_node.push(node->getSize());
  node_stack.push(node);

  while(!node_stack.empty()){
    int depth = depth_node.top();
    int axis = depth%3;
    int* triangles_pos = triangle_pos_node.top();
    size_t size = size_node.top();
    KDNode* node = node_stack.top();

    size_node.pop();
    depth_node.pop();
    triangle_pos_node.pop();
    node_stack.pop();

    float split = findMedianSplitting(triangles_pos,size,axis);

    // If it is cheaper to not split end
    if(size<=min_size){ // Can add depth test also
      node->setSize(size);
      node->setTriangles(triangles_pos);
      node->setAxis(axis);
      node->setLeaf(true);
    }
    else{
      vector<int> right_tri,left_tri;

      for(size_t i=0;i<size;i++){
        float min_triangle = KDTreeDataStruct::triangles[triangles_pos[i]]->getMin()[axis];
        float max_triangle = KDTreeDataStruct::triangles[triangles_pos[i]]->getMax()[axis];
        if(min_triangle<=split){
          left_tri.push_back(i);
        }
        if(max_triangle>=split){
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
      node->setSplit(split);
      node->setAxis(axis);

      KDNode* left = new KDNode();
      KDNode* right = new KDNode();

      left->setSide(KDTreeDataStruct::LEFT);
      right->setSide(KDTreeDataStruct::RIGHT);

      size_node.push(right_tri.size());
      size_node.push(left_tri.size());
      if(right_tri.size()==0)
        cout << "0 right";
      else if(left_tri.size()==0)
        cout << "0 right";

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

void KDTreeDataStruct::buildMedianNode(KDNode* node,int depth){
  int axis = depth%3;
  int* triangles_pos = node->getTriangles();
  size_t size = node->getSize();

  if(size<=min_size){
    node->setSize(size);
    node->setTriangles(triangles_pos);
    node->setAxis(axis);
    node->setLeaf(true);
  }
  else{

    //Pick median triangle and just select the barycenter, or mean of the two in the middle.
    float first_median = triangles[triangles_pos[size/2-1+size%2]]->getBarycenter()[axis];
    float second_median = triangles[triangles_pos[size/2]]->getBarycenter()[axis];
    float median = (first_median+second_median)/2;

    vector<int> right_tri,left_tri;

    for(size_t i=0;i<size;i++){
      float min_triangle = KDTreeDataStruct::triangles[triangles_pos[i]]->getMin()[axis];
      float max_triangle = KDTreeDataStruct::triangles[triangles_pos[i]]->getMax()[axis];
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

    left->setSize(left_tri.size());
    right->setSize(right_tri.size());

    left->setTriangles(left_triangles);
    right->setTriangles(right_triangles);

    node->setLeft(left);
    node->setRight(right);
  }
}


void KDTreeDataStruct::constructWireframe(){
  stack<AABB> aabb_stack;
  stack<KDNode*> node_stack;

  node_stack.push(root);
  aabb_stack.push(aabb);
  splitting_list.push_back(aabb);

  while(!node_stack.empty()){
    KDNode* node = node_stack.top();
    AABB s_aabb = aabb_stack.top();

    node_stack.pop();
    aabb_stack.pop();
    Side side = node->getSide();

    if(!node->isLeaf()){
      vec3 start,right_end,left_end;

      int axis = node->getAxis();
      vec3 pos = s_aabb.getPos();
      vec3 size = s_aabb.getSize();
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
        left_end = vec3(size.x,size.y,size.z-right_end.z);
      }

//      if(side == LEFT){
//        delete s_aabb;
//      }

      AABB left_aabb = AABB(pos,left_end,false,axis); // False -> Don't create any lines
      AABB right_aabb = AABB(start,right_end,true,axis); // True-> Create only four lines

      splitting_list.push_back(right_aabb);

      aabb_stack.push(right_aabb);
      aabb_stack.push(left_aabb);

      node_stack.push(node->getRight());
      node_stack.push(node->getLeft());
    }
  }
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
  float t=KDTreeDataStruct::triangles[triangles[top]]->getMax()[axis];
  int i = top - 1;
  int j = bottom + 1;
  int temp;
  do
  {
    do
    {
      j--;
    }while (t < KDTreeDataStruct::triangles[triangles[j]]->getMax()[axis]);

    do
    {
      i++;
    } while (t > KDTreeDataStruct::triangles[triangles[i]]->getMax()[axis]);

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

  KDTreeDataStruct::triangles = new KDTriangle*[size];
  KDTreeDataStruct::root_triangles = new int[size];
  for(size_t t=0;t<size;t++){
    KDTreeDataStruct::triangles[t] = new KDTriangle(triangles[t]);
    KDTreeDataStruct::root_triangles[t] = t;
  }
  KDTreeDataStruct::triangle_count = size;
  KDTreeDataStruct::aabb=aabb;
  KDTreeDataStruct::root->setTriangles(root_triangles);
  KDTreeDataStruct::root->setSize(size);
  min_size = (int)log10(triangle_count)*8;
  build();
}

}
