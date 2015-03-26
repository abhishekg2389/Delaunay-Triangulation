/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#ifndef VARC_H
#define VARC_H
#pragma once

#include <ogdf/basic/geometry.h>

using namespace ogdf;

class VArc {
private:
  VArc *left;
  VArc *right;
  VArc *parent;
  const DPoint *p;
  bool lefty;

public:
  VArc();
  VArc(const DPoint *p);

  bool isLeaf() const;
  bool isLeft() const;

  void setLeft(VArc *a); 
  void setRight(VArc *a);
  void setParent(VArc *a);

  VArc *getLeft() const;
  VArc *getRight() const;
  VArc *getParent() const;
  const DPoint *getPoint() const;
};

#endif
