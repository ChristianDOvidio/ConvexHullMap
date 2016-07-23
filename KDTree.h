#pragma once
#include "stdafx.h"
#pragma once
#ifndef __kdtree__
#define __kdtree__
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>

class KDNode {
private:
	double lat;
	double lon;
	std::string name;
	KDNode *left;
	KDNode *right;

	double distance(double _la, double _lo);

public:
	KDNode(double la, double lo, const std::string &desc);
	~KDNode();

	friend class KDTree;
};

class KDTree {
private:
	unsigned int size = 0;
	KDNode *root;
	unsigned int count;
	int max;

	void destroy(KDNode *p);
	unsigned int continuePrint(KDNode *r, double la, double lo, double rad, const std::string &filter, int level, std::ofstream &markers);
	void insertNode(KDNode* r, double la, double lo, const std::string &desc, int level);
	void enterVector(KDNode *r, int count);
	int threePointCalc(KDNode *one, KDNode *two, KDNode *three);
	void insertionSort(int n);

public:
	KDTree();
	~KDTree();
	std::vector <KDNode*> neighbors;
	std::stack <KDNode*> threePoint;

	void insert(double la, double lo, const std::string &desc);
	unsigned int printNeighbors(double la, double lo, double rad, const std::string &filter);
	unsigned int getSize();
	KDNode* findLeftmost(KDNode *p);
	int treeSize(KDNode *node);
	int printConvexHull(double la, double lo, double rad, const std::string &filter);
	KDNode* nextTop();
	void convexPrint();
	double polarAngle(KDNode* p, KDNode* pivot);
};

#endif