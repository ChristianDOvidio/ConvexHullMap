///Christian D'Ovidio

#include "stdafx.h"
#include "KDTree.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <stack>
using namespace std;

KDNode::KDNode(double la, double lo, const std::string &desc) {
	left = NULL;
	right = NULL;
	name = desc;
	lat = la;
	lon = lo;
}


KDNode::~KDNode() {
}

//returns distance between two points
double KDNode::distance(double _la, double _lo) {
	double param = M_PI / 180.0; // required for conversion from degrees to radians
	double rad = 3956.0;  // radius of earth in miles
	double d_lat = (_la - lat) * param;
	double d_lon = (_lo - lon) * param;
	double dist = sin(d_lat / 2) * sin(d_lat / 2) + cos(lat*param) * cos(_la*param) * sin(d_lon / 2) * sin(d_lon / 2);
	dist = 2.0 * atan2(sqrt(dist), sqrt(1 - dist));
	return rad * dist;
}


KDTree::KDTree() {
	root = NULL;
	//std::list<KDNode> neighbors;
	//vector(dynamic array) of pointers to KDNodes

	//std::vector<KDNode*> neighbors(0);
	std::vector<KDNode*> neighbors;
	std::stack<KDNode*> threePoint;
}


//destroys root which recursivly calls destroy on whole tree
KDTree::~KDTree() {
	destroy(root);

}


//destroys node based on four situations
void KDTree::destroy(KDNode *p) {
	if (p) {
		destroy(p->left);
		destroy(p->right);
		delete p;
	}
}


//inserts root node or calls insertNode
//to insert any other node
void KDTree::insert(double la, double lo, const std::string &desc) {
	if (root == NULL) {
		root = new KDNode(la, lo, desc);
	}
	else {
		insertNode(root, la, lo, desc, 0);
	}
}


//inserts a root given the input parameters
void KDTree::insertNode(KDNode* r, double la, double lo, const std::string &desc, int level) {
	//if r is on an even level (X level)
	//next node will be compared to X value(latitude)
	if (level % 2 == 0) {
		if (la <= r->lat) {
			if (r->left != NULL)
				insertNode(r->left, la, lo, desc, level + 1);
			else {
				r->left = new KDNode(la, lo, desc);
			}
		}
		else {
			if (r->right != NULL)
				insertNode(r->right, la, lo, desc, level + 1);
			else {
				r->right = new KDNode(la, lo, desc);
			}
		}
	}

	//if r is on an odd level (Y level)
	//next node will be compared to Y value(longitude)
	else {
		if (lo <= r->lon) {
			if (r->left != NULL)
				insertNode(r->left, la, lo, desc, level + 1);
			else {
				r->left = new KDNode(la, lo, desc);
			}
		}
		else {
			if (r->right != NULL)
				insertNode(r->right, la, lo, desc, level + 1);
			else {
				r->right = new KDNode(la, lo, desc);
			}
		}
	}
}


//print text and then calls continuePrint to traverse through tree
//returns count of number of matched locations
unsigned int KDTree::printNeighbors(double la, double lo, double rad, const std::string &filter) {
	ofstream markers;
	markers.open("data/markers.js");

	markers << "var markers = [\n";
	markers << "\t[\"" << "CENTER" << "\", " << la << ", " << lo << "],\n";
	count = continuePrint(root, la, lo, rad, filter, 0, markers);
	markers << "];\n";

	markers.close();
	return count;
}


//traverses through tree and "prunes" branches depending on if their children could
//fall into radius
//returns count
unsigned int KDTree::continuePrint(KDNode *r, double la, double lo, double rad, const std::string &filter, int level, std::ofstream &markers) {

	//if r is on an even level (X level)
	if (level % 2 == 0) {
		//point is inside radius
		if (r->distance(la, lo) <= rad) {
			//point matches all passed parameters
			if (r->name.find(filter) != std::string::npos) {
				//call array entering helper method
				enterVector(r, count);
				markers << "\t[\"" << r->name << "\", " << r->lat << ", " << r->lon << "],\n";
				std::cout << "\t[\"" << r->name << "\", " << r->lat << ", " << r->lon << "],\n";
				count++;
			}
			//both left and right children can have matching points
			if (r->left)
				continuePrint(r->left, la, lo, rad, filter, level + 1, markers);
			if (r->right)
				continuePrint(r->right, la, lo, rad, filter, level + 1, markers);
		}
		//point is not inside radius but left and right children can be
		//inside radius
		else if (r->lat > la - rad && r->lat < la + rad) {
			if (r->left)
				continuePrint(r->left, la, lo, rad, filter, level + 1, markers);
			if (r->right)
				continuePrint(r->right, la, lo, rad, filter, level + 1, markers);
		}
		//if point is above radius, only right children could match
		else if (r->lat <= la - rad && r->right)
			continuePrint(r->right, la, lo, rad, filter, level + 1, markers);
		//if point is below of radius, only left children could match
		else if (r->lat >= la + rad && r->left)
			continuePrint(r->left, la, lo, rad, filter, level + 1, markers);
	}

	//if r is on an odd level (Y level)
	else if (level % 2 != 0) {
		//point is inside radius
		if (r->distance(la, lo) <= rad) {
			//point matches all passed parameters
			if (r->name.find(filter) != std::string::npos) {
				//call array entering helper method
				enterVector(r, count);
				markers << "\t[\"" << r->name << "\", " << r->lat << ", " << r->lon << "],\n";
				std::cout << "\t[\"" << r->name << "\", " << r->lat << ", " << r->lon << "],\n";
				count++;
			}
			//both left and right children can have matching points
			if (r->left)
				continuePrint(r->left, la, lo, rad, filter, level + 1, markers);
			if (r->right)
				continuePrint(r->right, la, lo, rad, filter, level + 1, markers);
		}
		//point is not inside radius but left and right children can be
		//inside radius
		else if (r->lon > lo - rad && r->lon < lo + rad) {
			if (r->left)
				continuePrint(r->left, la, lo, rad, filter, level + 1, markers);
			if (r->right)
				continuePrint(r->right, la, lo, rad, filter, level + 1, markers);
		}
		//if point is above radius, only right children could match
		else if (r->lon <= lo - rad && r->right)
			continuePrint(r->right, la, lo, rad, filter, level + 1, markers);
		//if point is below of radius, only left children could match
		else if (r->lon >= lo + rad && r->left)
			continuePrint(r->left, la, lo, rad, filter, level + 1, markers);
	}
	//returns count 
	return count;
}


//writes to convex.js
//lists all vertices of convex hull
//uses Graham Scan algorithm
int KDTree::printConvexHull(double la, double lo, double rad, const std::string &filter) {
	int count = KDTree::printNeighbors(la, lo, rad, filter);

	double smallestY = neighbors.at(0)->lon;
	int smallestPos = 0;

	//first find point with lowest y-coor
	//if lowest y-coor is duplicate, take one with lowest x-coor
	for (int i = 1; i < count; i++) {
		double currentY = neighbors.at(i)->lon;

		if ((currentY < smallestY) || (smallestY == currentY &&
			neighbors.at(i)->lat < neighbors.at(smallestPos)->lat)) {
			smallestY = neighbors[i]->lon;
			smallestPos = i;
		}
	}

	//swaps found smallest Y point with
	//first point in vector
	KDNode* temp = neighbors.at(0);
	neighbors.at(0) = neighbors.at(smallestPos);
	neighbors.at(smallestPos) = temp;

	//uses insertion sort to sort array by polar angle
	insertionSort(count);

	std::cout << "\n" << "    Polar Angles:" << "\n";
	for (int x = 0; x < count; x++){
		std::cout << "    " << polarAngle(neighbors.at(x), neighbors.at(0)) << "\n";
	}

	if (count < 3)
		std::cout << "\n Impossible Paramaters \n";

	//pushes first three values onto stack
	threePoint.push(neighbors.at(0));
	threePoint.push(neighbors.at(1));
	threePoint.push(neighbors.at(2));

	//keeps pushing values onto stack
	for (int i = 3; i < count; i++) {
		while (threePointCalc(nextTop(), threePoint.top(), neighbors.at(i)) != 2) {
			threePoint.pop();
		}
		threePoint.push(neighbors.at(i));
	}

	//write stack to convex.js
	convexPrint();

	return count;
}


//prints list of vectors in clockwise direction
void KDTree::convexPrint() {
	ofstream convex;
	convex.open("data/convex.js");
	convex << "var convex = [\n";
	std::cout << "\nPoints of convex hull: \n";

	while (!threePoint.empty()) {
		KDNode* currentVertex = threePoint.top();
		convex << "{lat:" << currentVertex->lat << ",lng:" << currentVertex->lon << "},\n";
		std::cout << "        {lat: " << currentVertex->lat << ", lng: " << currentVertex->lon << "},\n";
		threePoint.pop();
	}
	convex << "];";

	convex.close();
}


//puts passed KDNode at the end of the vector
void KDTree::enterVector(KDNode *r, int count) {
	neighbors.push_back(r);

}


//returns second to top element of stack
KDNode* KDTree::nextTop() {
	KDNode* p = threePoint.top();
	threePoint.pop();

	KDNode* next = threePoint.top();
	threePoint.push(p);

	return next;
}


//calculates whether a grouping of three points is a
//clockwise, counterclockwise, or straight turn.
int KDTree::threePointCalc(KDNode *one, KDNode *two, KDNode *three) {
	double angle = (two->lon - one->lon) * (three->lat - two->lat) -
		(two->lat - one->lat) * (three->lon - two->lon);

	if (angle == 0)
		return 0;//colinear
	if (angle > 0)
		return 1;
	else
		return 2;
}


//uses insertionsort to sort all nodes by the angle the line
//between it and p0 makes with the the x line
void KDTree::insertionSort(int size) {
	int j;
	KDNode *temp;

	for (int i = 1; i < size; i++) {
		j = i;

		while (polarAngle(neighbors.at(j), neighbors.at(0))   <   polarAngle(neighbors.at(j - 1), neighbors.at(0)))
		{
			temp = neighbors.at(j);
			neighbors.at(j) = neighbors.at(j - 1);
			neighbors.at(j - 1) = temp;
			j--;
		}
	}
}


//returns angle between line made by two points and X-axis in degrees
double KDTree::polarAngle(KDNode*p, KDNode* pivot) {
	return atan2(p->lon - pivot->lon, p->lat - pivot->lat) * 180 / M_PI;
}


//Calls treeSize on root node
unsigned int KDTree::getSize() {
	return(treeSize(root));
}


//Returns size of tree
//Basecase is when node doesn't exist
int KDTree::treeSize(KDNode *node) {
	if (node == NULL)
		return 0;
	else
		return(treeSize(node->left) + treeSize(node->right) + 1);
}


//returns leftmost node in subtree with p as root
KDNode* KDTree::findLeftmost(KDNode *p) {
	while (p->left != NULL) {
		p = p->left;
	}
	return p;
}

