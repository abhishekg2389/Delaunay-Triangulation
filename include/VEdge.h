/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#ifndef VEDGE_H
#define VEDGE_H
#pragma once

#include <ogdf/basic/geometry.h>

using namespace ogdf;

class VEdge {
private:
  const DPoint *r1;  // for region 1
  const DPoint *r2;  // for region 2
  const DPoint *p1;  // for starting / ending pt
  const DPoint *p2;  // for starting / ending pt

public:
  VEdge(const DPoint *r1, const DPoint *r2);
  
  bool isDefined() const;

  void setPoint(const DPoint *d);

  const DPoint *getr1() const;
  const DPoint *getr2() const;
  const DPoint *getp1() const;
  const DPoint *getp2() const;
};

#endif
