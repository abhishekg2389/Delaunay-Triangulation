/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#ifndef VEVENT_H
#define VEVENT_H
#pragma once

#include <ogdf/basic/geometry.h>
#include <VArc.h>

using namespace ogdf;

class VEvent {
private:
  const DPoint *pt;
  bool site;
  double y;
  VArc *a;

public:
  VEvent();
  VEvent(const DPoint *pt, bool site, double y);
  VEvent(const DPoint *pt, bool site, VArc *a, double y);
  
  bool operator<(const VEvent &b) const;

  bool isVPoint() const;

  void setVArc(VArc *a);

  const DPoint *getPoint() const;
  double getY() const;
  VArc *getVArc() const;
};

#endif
