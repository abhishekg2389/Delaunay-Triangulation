/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#include <VEdge.h>

VEdge::VEdge(const DPoint *r1, const DPoint *r2)
  : r1(r1)
  , r2(r2)
  , p1(NULL)
  , p2(NULL)
{}

bool VEdge::isDefined() const {
  return p1 != NULL && p2 != NULL;
}

void VEdge::setPoint(const DPoint *d) {
  if(p1 == NULL) {
    p1 = d;
  }
  else {
    p2 = d;
  }
}

const DPoint *VEdge::getr1() const {
  return r1;
}

const DPoint *VEdge::getr2() const {
  return r2;
}

const DPoint *VEdge::getp1() const {
  return p1;
}

const DPoint *VEdge::getp2() const {
  return p2;
}
