/*
Saglam, Ali, & Baykan, N. A. (2017).
"Sequential image segmentation based on min- imum spanning tree representation".
Pattern Recognition Letters, 87 , 155–162.
https://doi.org/10.1016/j.patrec.2016.06.001 .
*/

#pragma once
#ifndef SMST_H
#define SMST_H

#include "Node.h"
#include "FibonacciHeap.h"
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>

typedef struct {
	Node *node1, *node2;
	float distance;
} Connection;

using namespace cv;
using namespace std;

class smst
{
public:
	Mat im;
	int M, N, psrlen, segsize;
	Node **nodes, *pnodes;
	float ***colors, **pcolors, *qcolors;
	Connection *psrmst;
	vector<vector<Node::Pixel>> segments;
	float c;

	void mat2array();
	void setNodes();
	void setConnects();
	void setPSRMST();
	void segmentation(int, float);
	Mat getColoredSegments();
	Mat getLabels();

	smst(Mat img)
	{
		im = img;
		
		M = im.rows;
		N = im.cols;
		segsize = 0;
		psrlen = M * N - 1;

		colors = new float**[M];
		pcolors = new float*[M * N];
		qcolors = new float[M * N * 3];
		nodes = new Node*[M];
		pnodes = new Node[M * N];
		for (int i = 0; i < M; i++)
		{
			colors[i] = &pcolors[i * N];
			nodes[i] = &pnodes[i * N];
			for (int j = 0; j < N; j++)
				colors[i][j] = &qcolors[(i * N + j) * 3];
		}

		mat2array();
		setNodes();
		setConnects();

		psrmst = new Connection[psrlen];

		setPSRMST();
	}
};

#endif
