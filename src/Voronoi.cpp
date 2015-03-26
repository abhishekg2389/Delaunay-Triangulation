/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/geometry.h>
#include <Voronoi.h>
#include <VEdge.h>
#include <vector>
#include <cmath>
#include <utility>

#define EPS 1e-8

using namespace ogdf;

std::vector<const DPoint *> &Voronoi::getVPoints() {
  return VPoints;
}

std::vector<VEdge *> &Voronoi::getVEdges(std::vector<const DPoint *> &v) {
  root = NULL;

  for(int i=0;i<v.size();i++) {
    mp.insert(VEvent(v[i], true, v[i]->m_y));
  }

  while(!mp.empty()) {
    VEvent eve = *mp.begin();
    currentY = mp.begin()->getY();
    mp.erase(mp.begin());

    if(!eve.isVPoint()) {
      InsertVArc(eve.getPoint());
    }
    else {
      RemoveVArc(eve.getPoint(), currentY, eve.getVArc());
    }
  }
  
  return VEdges;
}

void Voronoi::InsertVArc(const DPoint *p) {
  if(root == NULL) {
    VArc* a = new VArc(p);
    root = a;
    deletedPoints.push_back(p);
    return;
  }

  VArc *temp = getVArc(p, p->m_y);
  
  if(temp->getPoint()->m_y == p->m_y) {
    VArc *new_node = new VArc(p);
    VArc *copy1 = new VArc(temp->getPoint());
    if(temp->getPoint()->m_x < p->m_x) {
      temp->setLeft(copy1);
      temp->setRight(new_node);
    }
    else {
      temp->setLeft(new_node);
      temp->setRight(copy1);
    }
    new_node->setParent(temp);
    copy1->setParent(temp);
    VPointchk(copy1, p);
    deletedPoints.push_back(p);

    return;
  }

  VArc *copy1 = new VArc(temp->getPoint());
  VArc *copy2 = new VArc(temp->getPoint());
  VArc *new_node = new VArc(p);
  VArc *fake = new VArc();
  
  if(temp->getPoint()->m_x < p->m_x) {
    temp->setLeft(copy1);
    temp->setRight(fake);
    fake->setLeft(new_node);
    fake->setRight(copy2);
  }
  else {
    temp->setLeft(fake);
    fake->setRight(new_node);
    fake->setLeft(copy2);
    temp->setRight(copy1);
  }
    
  copy1->setParent(temp);
  copy2->setParent(fake);
  new_node->setParent(fake);
  fake->setParent(temp);
  
  VPointchk(copy2, p);
  VPointchk(copy1, p);
  
  deletedPoints.push_back(p);
}

void Voronoi::RemoveVArc(const DPoint *p, double y, VArc *a) {
  if(!a->isLeaf() || a == NULL) {
    if(a==NULL) {
      cout<<"error";
    }
  return;
  }

  VArc *arcL;
  VArc *arcR;

  if(a->isLeft()) {
    arcL = getLeftPred(a);
    arcR = getRightPredDown(a->getParent());
  }
  else {
    arcL = getLeftPredDown(a->getParent());
    arcR = getRightPred(a);
  }

  setVPoint(getVEdge(arcL->getPoint(), a->getPoint()), p, arcL->getPoint(), a->getPoint());
  setVPoint(getVEdge(arcR->getPoint(), a->getPoint()), p, arcR->getPoint(), a->getPoint());

  VEdge *v = new VEdge(arcR->getPoint(), arcL->getPoint());
  v->setPoint(p);
  VEdges.push_back(v);
  DeleteVArc(a);

  VPoints.push_back(p);
  
  VPointchk(arcL, p);
  VPointchk(arcR, p);
  
  return;
}

void Voronoi::DeleteVArc(VArc *a) {
  VArc *pa = a->getParent();
  if(pa->isLeft()) {
    if(a->isLeft()) {
	  pa->getParent()->setLeft(pa->getRight());
    }
    else {
      pa->getParent()->setLeft(pa->getLeft());
    }
  }
  else {
    if(a->isLeft()) {
      pa->getParent()->setRight(pa->getRight());
    }
    else {
      pa->getParent()->setRight(pa->getLeft());
    }
  }

  for(std::multiset<VEvent>::iterator it = mp.begin();it!=mp.end();) {
    if(a == it->getVArc() || a->getParent() == it->getVArc()) {
      std::multiset<VEvent>::iterator it_;
      it_ = it;
      it++;
      mp.erase(it_);
    }
    else {
      it++;
    }
  }

  pa = a->getParent();
  delete pa;
  pa = NULL;
  delete a;
  a = NULL;
}

void Voronoi::VPointchk(VArc *a, const DPoint *p) {
  VArc *arcL;
  VArc *arcR;

  if(a->isLeft())  {
    arcL = getLeftPred(a);
    arcR = getRightPredDown(a->getParent());
  }
  else {
    arcL = getLeftPredDown(a->getParent());
    arcR = getRightPred(a);
  }

  if(arcL == NULL || arcR == NULL) {
    if(arcR != NULL) {
      VEdge *v = getVEdge(arcR->getPoint(), a->getPoint());
      if(v == NULL) {
        VEdges.push_back(new VEdge(arcR->getPoint(), a->getPoint()));
      }
      return;
    }

    if(arcL != NULL) {
      VEdge *v = getVEdge(arcL->getPoint(), a->getPoint());
      if(v == NULL) {
        VEdges.push_back(new VEdge(al->getPoint(), a->getPoint()));
      }
      return;
    }
  }

  if(DLine(*a->getPoint(), *al->getPoint()).slope() == DLine(*ar->getPoint(), *al->getPoint()).slope()) {
    return;
  }

  const DPoint *d = new DPoint(circumCenter(a->getPoint(), arcL->getPoint(), arcR->getPoint()));
  double rad = d->distance(*a->getPoint());
  VEvent v(d, false, a, d->m_y - rad);

  for(int i=0;i<deletedPoints.size();i++) {
    if(d->distance(*deletedPoints[i]) < d->distance(*a->getPoint()) - EPS) {
      return;
    }
  }

  for(int i=0;i<VPoints.size();i++) {
    if(d->distance(*VPoints[i]) < EPS) {
      return;
    }
  }

  for(std::multiset<VEvent>::iterator it = mp.begin();it!=mp.end();it++) {
    if(!it->isVPoint())  {
      continue;
    }
    if(d->distance(*it->getPoint()) < EPS) {
      return;
    }
  }

  mp.insert(v);
}

VArc *Voronoi::getVArc(const DPoint *p, double y) const {
  VArc *temp = root;

  while(temp->getLeft()!=NULL) {
    double x_inter = x_intersection(temp, y);
    if(x_inter < p->m_x) {
      temp = temp->getRight();
    }
    else {
      temp = temp->getLeft();
    }
  }

  return temp;
}

VEdge *Voronoi::getVEdge(const DPoint *p1, const DPoint *p2) const {
  for(int i = 0; i < VEdges.size() ; ++i)  {
    if((p1 == VEdges[i]->getr1() && p2 == VEdges[i]->getr2()) || (p1 == VEdges[i]->getr2() && p2 == VEdges[i]->getr1())) {
      return VEdges[i];
    }
  }

  return NULL;
}

VArc *Voronoi::getLeftPred(const VArc *a) const {
  VArc *temp = a->getParent();
  
  if(temp == NULL)
  return NULL;

  while(temp->isLeft()) {
    if(temp->getParent() == NULL) {
      return NULL;
    }
    temp = temp->getParent();
  }

  if(temp->getParent() == NULL) {
    return NULL;
  }

  temp = temp->getParent()->getLeft();

  while(temp->getRight() != NULL) {
    temp = temp->getRight();
  }

  return temp;
}

VArc *Voronoi::getRightPred(const VArc *a) const {
  VArc *temp = a->getParent();

  while(!temp->isLeft()) {
    if(temp->getParent() == NULL) {
      return NULL;
    }
    temp = temp->getParent();
  }

  if(temp->getParent() == NULL) {
    return NULL;
  }

  temp = temp->getParent()->getRight();

  while(temp->getLeft() != NULL) {
    temp = temp->getLeft();
  }

  return temp;
}

VArc *Voronoi::getLeftPredDown(const VArc *a) const {
  VArc *temp = a->getLeft();

  while(temp->getRight() != NULL)  {
    temp = temp->getRight();
  }

  return temp;
}

VArc *Voronoi::getRightPredDown(const VArc *a) const {
  VArc *temp = a->getRight();

  while(temp->getLeft() != NULL) {
    temp = temp->getLeft();
  }

  return temp;
}

double Voronoi::x_intersection(const VArc *a, double y) const {
  double x;

  VArc *al = getLeftPredDown(a);
  VArc *ar = getRightPredDown(a);

  return x_intersection(al->getPoint(), ar->getPoint(), y);
}

double Voronoi::x_intersection(const DPoint *pl, const DPoint *pr, double y) const {
  double x;

  double x1 = pl->m_x;
  double x2 = pr->m_x;
  double y1 = pl->m_y;
  double y2 = pr->m_y;

  if(y1 == y2) {
    return (x1 + x2)/2;
  }

  double m = (-1)*((x1 - x2)/(y1 - y2));
  double k = (y2*y2 - y1*y1 + x2*x2 - x1*x1)/(2*(y2-y1));

  double b = 2*m*y - 2*x1 - 2*m*y1;
  double c = y1*y1 - y*y + x1*x1 - 2*k*y1 + 2*k*y;

  double D = std::sqrt(b*b - 4*c);

  double x_1 = ((-1)*b)/2 + D/2;
  double x_2 = ((-1)*b)/2 - D/2;

  if(y1 < y2) {
    x = std::max(x_1, x_2);
  }
  else {
    x = std::min(x_1, x_2);
  }
  
  return x;
}

void Voronoi::setVPoint(VEdge *v, const DPoint *p, const DPoint *r1, const DPoint *r2) {
  if(v == NULL) {
    VEdge *v_ = new VEdge(r1, r2);
    v_->setPoint(p);
    VEdges.push_back(v_);
  }
  else {
    v->setPoint(p);
  }

  return;
}

DPoint Voronoi::circumCenter(const DPoint *p1, const DPoint *p2, const DPoint *p3) const {
  DPoint center;
  DPoint a, b, c, d;
  
  a = DPoint((p1->m_x + p2->m_x)/2, (p1->m_y + p2->m_y)/2);
  if(p1->m_y == p2->m_y) {
    b = DPoint(a.m_x, a.m_y+1);
  }
  else {
    b = DPoint(0, a.m_y + ((p1->m_x - p2->m_x)/(p1->m_y - p2->m_y))*a.m_x);
  }
  
  c = DPoint((p2->m_x + p3->m_x)/2, (p2->m_y + p3->m_y)/2);
  if(p2->m_y == p3->m_y) {
    d = DPoint(c.m_x, c.m_y+1);
  }
  else {
    d = DPoint(0, c.m_y + ((p2->m_x - p3->m_x)/(p2->m_y - p3->m_y))*c.m_x);
  }
  
  DLine l1(a, b);
  DLine l2(c, d);
  
  l1.intersection(l2, center, true);
  
  return center;
}
