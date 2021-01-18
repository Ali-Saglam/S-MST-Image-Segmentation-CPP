/*
Saglam, Ali, & Baykan, N. A. (2017).
"Sequential image segmentation based on min- imum spanning tree representation".
Pattern Recognition Letters, 87 , 155–162.
https://doi.org/10.1016/j.patrec.2016.06.001 .
*/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Node
{
public:
	int x, y, nodeid, neighnum, connum;
	float color[3];
	bool mark;

	typedef struct {
		Node *node;
		float distance;
	} Connection;

	typedef struct {
		int x, y;
	} Pixel;

	/////////// FIBONACCI HEAP SECTION /////

	Node *parent;
	Node *leftSibling, *rightSibling;
	Node *children;
	Node *next;
	Node *prev;

	float key;
	int rank;

	bool marked;

	bool addChild(Node *node);
	bool addSibling(Node *node);
	bool remove();

	Node* leftMostSibling();
	Node* rightMostSibling();

	///////////////////////////////////////

	Connection neighbors[8];
	Node *connections[8];

	Node()
	{
		neighnum = 0;
		connum = 0;
		nodeid = -1;
		mark = false;

		parent = NULL;
		children = NULL;
		leftSibling = NULL;
		rightSibling = NULL;
		prev = NULL;
		rank = 0;
		marked = false;
		key = -1;
	}

	void addNeighbor(Node *N, float d)
	{
		Connection neigh;
		neigh.node = N;
		neigh.distance = d;
		neighbors[neighnum] = neigh;
		neighnum++;
	}

	void clearNeighbors()
	{
		for (int i = 0; i < neighnum; i++) {
			if ((neighbors[i].node)->nodeid == 0) {
				neighnum--;
				neighbors[i] = neighbors[neighnum];
				i--;
			}
		}
	}

	void addConnection(Node *N)
	{
		connections[connum] = N;
		connum++;
	}

	void removeConnection(Node *N)
	{
		for (int i = 0; i < connum; i++) {
			if (connections[i] == N) {
				connum--;
				connections[i] = connections[connum];
				return;
			}
		}
	}

	void collectPixels(vector<Pixel> &list, Node *node)
	{
		if (mark)
			return;

		mark = true;

		Pixel p;
		p.x = x;
		p.y = y;
		list.push_back(p);

		for (int i = 0; i < connum; i++)
		{
			if (connections[i] != node)
				connections[i]->collectPixels(list, this);
		}
	}
};

#endif
