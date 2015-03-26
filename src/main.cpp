/*
  Author: Abhishek Gupta
  e-mail: a.gupps@gmail.com
*/

#include <ogdf/basic/geometry.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <math.h>
#include <iostream>
#include <queue>
#include <utility>
#include <conio.h>
#include <vector>
#include <VEdge.h>
#include <voronoi.h>
#include <string.h>

using namespace ogdf;

int main() {
  
  srand((unsigned)time(NULL));
  double X = ((double)rand()/(double)RAND_MAX);

  Graph G;
  GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics 
  | GraphAttributes::nodeLabel | GraphAttributes::edgeStyle);
  
  int nPoints;
  std::cout<<"Enter the number of points: "<<endl;
  std::cin>>nPoints;
  int width, height;
  std::cout<<"Enter the width and height of the plane: "<<endl;
  std:cin>>width>>height;

  nPoints = 200;
  width = 1000;
  height = 1000;
  
  std::vector<const DPoint *> points;
  std::vector<VEdge *> VEdges;

  for(int i=0;i<nPoints;i++){
    node nd = G.newNode(i);
    GA.x(nd) = ((double)rand()/(double)RAND_MAX)*width;
    GA.y(nd) = ((double)rand()/(double)RAND_MAX)*height;
    //GA.label(nd) = "(" + to_string(GA.x(nd)) + "," + to_string(GA.y(nd)) + ")";

    points.push_back(new DPoint(GA.x(nd), GA.y(nd)));
    cout<<GA.x(nd)<<" "<<GA.y(nd)<<endl;
  }

  //points.push_back(new DPoint(100,300));
  //points.push_back(new DPoint(300,300));
  //points.push_back(new DPoint(500,300));
  //points.push_back(new DPoint(700,300));
  //points.push_back(new DPoint(900,300));
  //points.push_back(new DPoint(400,600));

  //points.push_back(new DPoint(100,100));
  //points.push_back(new DPoint(300,300));
  //points.push_back(new DPoint(300,500));
  //points.push_back(new DPoint(300,700));
  //points.push_back(new DPoint(300,900));
  //points.push_back(new DPoint(600,400));

  /* points.push_back(new DPoint(917.447,894.436));
  points.push_back(new DPoint(375.042,504.501));
  points.push_back(new DPoint(483.84,698.141));
  points.push_back(new DPoint(332.224,897.366));
  points.push_back(new DPoint(404.706,959.563));
  points.push_back(new DPoint(508.957,384.35));
  points.push_back(new DPoint(190.802,963.347));
  points.push_back(new DPoint(100.803,730.918));
  points.push_back(new DPoint(989.868,434.767));
  points.push_back(new DPoint(926.45,997.223));

  for(int i=0;i<points.size();i++){
    node nd = G.newNode(i);
    GA.x(nd) = points[i]->m_x;
    GA.y(nd) = points[i]->m_y;
    GA.label(nd) = "(" + to_string(GA.x(nd)) + "," + to_string(GA.y(nd)) + ")";
  
    cout<<GA.x(nd)<<" "<<GA.y(nd)<<endl;
  }*/
  
  voronoi vor;
  VEdges = vor.getVEdges(points);

  List<node> l;
  G.allNodes(l);
  std::cout<<l.size()<<endl;

  for(int i=0 ; i<VEdges.size() ; i++){  
    auto n1 = std::find(points.begin(), points.end(), VEdges[i]->getr1()) - points.begin();
    auto n2 = std::find(points.begin(), points.end(), VEdges[i]->getr2()) - points.begin();
    G.newEdge(*l.get(n1), *l.get(n2));
  }
  
  node nd;

  nd = G.newNode();
  GA.x(nd) = 0;
  GA.y(nd) = 0;

  nd = G.newNode();
  GA.x(nd) = 0;
  GA.y(nd) = 1000;

  nd = G.newNode();
  GA.x(nd) = 1000;
  GA.y(nd) = 0;
  
  nd = G.newNode();
  GA.x(nd) = 1000;
  GA.y(nd) = 1000;
  
  GA.setAllHeight(1);
  GA.setAllWidth(1);
  GraphIO::drawSVG(GA, "Graph.svg");
   
  getch();
  return 0;
}
