/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#include <VEvent.h>

VEvent::VEvent()
  : pt(NULL)
  , a(NULL)
{}

VEvent::VEvent(const DPoint *pt, bool site, double y)
  : pt(pt)
  , site(site)
  , a(NULL)
  , y(y)
{}

VEvent::VEvent(const DPoint *pt, bool site, VArc *a, double y)
  : pt(pt)
  , site(site)
  , a(a)
  , y(y)
{}

bool VEvent::operator<(const VEvent &b) const {
  return b.y < y;
}

bool VEvent::isVPoint() const {
  return !site;
}

void VEvent::setVArc(VArc *a) {
  this->a = a;
}

const DPoint *VEvent::getPoint() const {
  return pt;
}

double VEvent::getY() const {
  return y;
}

VArc *VEvent::getVArc() const {
  return a;
}
