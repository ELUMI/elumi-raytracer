/*
 * PriorityList.h
 *
 *  Created on: Mar 26, 2012
 *      Author: irri
 */

#ifndef PRIORITYLIST_H_
#define PRIORITYLIST_H_

#include <list>

template <class T>
class PriorityList {
public:
  PriorityList(size_t max_size);
  virtual ~PriorityList();

  void add(T data);
  T get(T data);

private:
  size_t max_size;
  std::list<T> plist;
};

PriorityList::PriorityList(max_size) {
  this->max_size = max_size;
}

PriorityList::~PriorityList() {

}

void PriorityList::add(T data) {
  plist.push_front(data);
  if(plist.size() > max_size) {
    plist.pop_back();
  }

}

T PriorityList::get(T data) {
  for (std::list<T>::iterator it = plist.begin(); it != plist.end(); it++) {
    if(it == data) {
      if(it==plist.begin())
        return data;
      plist.pop_front(data);
      plist.erase(it);
    }
  }
}


#endif /* PRIORITYLIST_H_ */
