/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#ifndef VORONOI_H
#define VORONOI_H
#pragma once

#include <ogdf/basic/Graph_d.h>
#include <VArc.h>
#include <VEvent.h>
#include <VEdge.h>
#include <set>
#include <vector>
#include <utility>

using namespace ogdf;

class Voronoi {
private:
  VArc *root;
  std::vector<const DPoint *> VPoints;
  std::vector<VEdge *> VEdges;
  std::multiset<VEvent> mp;
  std::vector<const DPoint *> deletedPoints;
  double currentY;

public:
  std::vector<const DPoint *> &getVPoints();
  std::vector<VEdge *> &getVEdges(std::vector<const DPoint *> &v);

  void setVPoint(VEdge *v, const DPoint *p, const DPoint *r1, const DPoint *r2);

  void InsertVArc(const DPoint *p);
  void RemoveVArc(const DPoint *p, double y, VArc *a);
  void DeleteVArc(VArc *a);
  void VPointchk(VArc *a, const DPoint *p);

  VArc *getVArc(const DPoint *p, double y) const;
  VEdge *getVEdge(const DPoint *p1, const DPoint *p2) const;

  VArc *getLeftPred(const VArc *a) const;
  VArc *getRightPred(const VArc *a) const;
  VArc *getLeftPredDown(const VArc *a) const;
  VArc *getRightPredDown(const VArc *a) const;

  double x_intersection(const VArc *a, double y) const;
  double x_intersection(const DPoint *pl, const DPoint *pr, double y) const;
  DPoint circumCenter(const DPoint *p1, const DPoint *p2, const DPoint *p3) const;
};

#endif
