/*
Saglam, Ali, & Baykan, N. A. (2017).
"Sequential image segmentation based on min- imum spanning tree representation".
Pattern Recognition Letters, 87 , 155–162.
https://doi.org/10.1016/j.patrec.2016.06.001 .
*/

#include "smst.h"

void smst::mat2array()
{
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++)
		{
			colors[i][j][0] = (float)im.at<Vec3b>(i, j).val[0];
			colors[i][j][1] = (float)im.at<Vec3b>(i, j).val[1];
			colors[i][j][2] = (float)im.at<Vec3b>(i, j).val[2];
		}
	}
}

void smst::setNodes() {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			nodes[i][j].x = i;
			nodes[i][j].y = j;
			nodes[i][j].color[0] = colors[i][j][0];
			nodes[i][j].color[1] = colors[i][j][1];
			nodes[i][j].color[2] = colors[i][j][2];
			nodes[i][j].nodeid = -1;
		}
	}
}

void smst::setConnects() {
	float d;

	for (int i = 0; i < M - 1; i++) {
		for (int j = 0; j < N - 1; j++) {

			d = sqrt(pow(colors[i + 1][j + 1][0] - colors[i][j][0], 2) +
				pow(colors[i + 1][j + 1][1] - colors[i][j][1], 2) +
				pow(colors[i + 1][j + 1][2] - colors[i][j][2], 2)
			);

			nodes[i][j].addNeighbor(&nodes[i + 1][j + 1], d);
			nodes[i + 1][j + 1].addNeighbor(&nodes[i][j], d);

			d = sqrt(pow(colors[i + 1][j][0] - colors[i][j + 1][0], 2) +
				pow(colors[i + 1][j][1] - colors[i][j + 1][1], 2) +
				pow(colors[i + 1][j][2] - colors[i][j + 1][2], 2)
			);

			nodes[i + 1][j].addNeighbor(&nodes[i][j + 1], d);
			nodes[i][j + 1].addNeighbor(&nodes[i + 1][j], d);

			d = sqrt(pow(colors[i][j + 1][0] - colors[i][j][0], 2) +
				pow(colors[i][j + 1][1] - colors[i][j][1], 2) +
				pow(colors[i][j + 1][2] - colors[i][j][2], 2)
			);

			nodes[i][j].addNeighbor(&nodes[i][j + 1], d);
			nodes[i][j + 1].addNeighbor(&nodes[i][j], d);

			d = sqrt(pow(colors[i + 1][j][0] - colors[i][j][0], 2) +
				pow(colors[i + 1][j][1] - colors[i][j][1], 2) +
				pow(colors[i + 1][j][2] - colors[i][j][2], 2)
			);

			nodes[i][j].addNeighbor(&nodes[i + 1][j], d);
			nodes[i + 1][j].addNeighbor(&nodes[i][j], d);
		}
	}

	for (int i = 0; i < M - 1; i++)
	{
		d = sqrt(pow(colors[i][N - 1][0] - colors[i][N - 1][0], 2) +
			pow(colors[i][N - 1][1] - colors[i][N - 1][1], 2) +
			pow(colors[i][N - 1][2] - colors[i][N - 1][2], 2)
		);

		nodes[i][N - 1].addNeighbor(&nodes[i + 1][N - 1], d);
		nodes[i + 1][N - 1].addNeighbor(&nodes[i][N - 1], d);
	}

	for (int j = 0; j < N - 1; j++)
	{
		d = sqrt(pow(colors[M - 1][j][0] - colors[M - 1][j + 1][0], 2) +
			pow(colors[M - 1][j][1] - colors[M - 1][j + 1][1], 2) +
			pow(colors[M - 1][j][2] - colors[M - 1][j + 1][2], 2)
		);

		nodes[M - 1][j].addNeighbor(&nodes[M - 1][j + 1], d);
		nodes[M - 1][j + 1].addNeighbor(&nodes[M - 1][j], d);
	}
}

void smst::setPSRMST() {
	FibonacciHeap* heap = new FibonacciHeap();
	heap->insertVertex(&(nodes[0][0]));

	Node* node = heap->findMin();
	Connection currentConn;

	int a = 0;

	// Scan
	do {
		// Delete minimum path
		node = heap->deleteMin();
		node->clearNeighbors();
		for (int i = 0; i < node->neighnum; i++)
		{
			currentConn.distance = node->neighbors[i].distance;
			currentConn.node1 = node->neighbors[i].node;

			Node* headOfCurrentEdge = currentConn.node1;

			if (headOfCurrentEdge->marked == false)
			{
				// Insert a vertex with infinite key
				headOfCurrentEdge->marked = true;
				headOfCurrentEdge->prev = node;
				headOfCurrentEdge->key = currentConn.distance;
				heap->insertVertex(headOfCurrentEdge);
			}
			else if (currentConn.distance < headOfCurrentEdge->key)
			{
				// decrease the key of a vertex with finite key
				headOfCurrentEdge->prev = node;
				heap->decreaseKey(currentConn.distance, headOfCurrentEdge);
			}
		}

		if (node->prev != NULL) {
			node->addConnection(node->prev);
			node->prev->addConnection(node);

			psrmst[a].distance = node->key;
			psrmst[a].node1 = node;
			psrmst[a].node1->nodeid = 0;
			psrmst[a].node2 = node->prev;

			a++;
		}
	} while (!heap->isEmpty());
	
	float sumdiff = 0;
	for (int i = 1; i < psrlen; i++)
		sumdiff += abs(psrmst[i].distance - psrmst[i - 1].distance);

	c = (float)sumdiff / (psrlen - 1);
}

void smst::segmentation(int l, float m) {
	int start, finish, first, last, maxin1 = -1000, maxin2 = -1000;
	float max1 = -1000, max2 = -1000;

	vector<Connection> cuts;

	segments.clear();

	if (l == 0) {
		l = (int) sqrt(M * N) / 2;
	}

	if (psrlen < (2 * l) + 1)
		return;

	float curr_c = c * m;

	start = l;
	finish = psrlen - l;

	int segid = 1;

	for (int i = start; i < finish; i++) {

		first = i - l;
		last = i - 1;

		if (psrmst[last].distance > max1) {
			max1 = psrmst[last].distance;
			maxin1 = last;
		} else if (maxin1 < first) {
			max1 = psrmst[first].distance;
			maxin1 = first;
			for (int j = first + 1; j <= last; j++) {
				if (psrmst[j].distance > max1) {
					max1 = psrmst[j].distance;
					maxin1 = j;
				}
			}
		}


		first = i + 1;
		last = i + l;

		if (psrmst[last].distance > max2) {
			max2 = psrmst[last].distance;
			maxin2 = last;
		} else if (maxin2 < first) {
			max2 = psrmst[first].distance;
			maxin2 = first;
			for (int j = first + 1; j <= last; j++) {
				if (psrmst[j].distance > max2) {
					max2 = psrmst[j].distance;
					maxin2 = j;
				}
			}
		}

		if (psrmst[i].distance > min(max1, max2) + curr_c) {
			psrmst[i].node1->removeConnection(psrmst[i].node2);
			psrmst[i].node2->removeConnection(psrmst[i].node1);
			Connection conn;
			conn.node1 = psrmst[i].node1;
			conn.node2 = psrmst[i].node2;
			cuts.push_back(conn);
		}
	}

	for (int i = 0; i < cuts.size(); i++) {
		vector<Node::Pixel> segment1;
		cuts[i].node1->collectPixels(segment1, cuts[i].node2);
		if (segment1.size() > 0)
			segments.push_back(segment1);

		vector<Node::Pixel> segment2;
		cuts[i].node2->collectPixels(segment2, cuts[i].node1);
		if (segment2.size() > 0)
			segments.push_back(segment2);
	}

	for (int i = 0; i < cuts.size(); i++) {
		cuts[i].node1->addConnection(cuts[i].node2);
		cuts[i].node2->addConnection(cuts[i].node1);
	}
	cuts.clear();
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			nodes[i][j].mark = false;
}

Mat smst::getColoredSegments() {

	Mat newimg(M, N, CV_8UC3, Scalar::all(0));

	uchar r = 10, g = 10, b = 10;

	int len = (int)segments.size();
	int segsize;

	for (int i = 0; i < len; i++) {
		srand((unsigned int)time(0));
		r = (uchar)r + rand() % 200 + 20;
		g = (uchar)g + rand() % 200 + 20;
		b = (uchar)b + rand() % 200 + 20;
		segsize = (int)segments[i].size();
		for (int j = 0; j < segsize; j++) {
			newimg.at<Vec3b>(segments[i][j].x, segments[i][j].y).val[0] = r;
			newimg.at<Vec3b>(segments[i][j].x, segments[i][j].y).val[1] = g;
			newimg.at<Vec3b>(segments[i][j].x, segments[i][j].y).val[2] = b;
		}
	}

	return newimg;
}

Mat smst::getLabels() {
	Mat labels(M, N, CV_32S);

	int len = (int)segments.size();
	int segsize;

	int a = 0;

	for (int i = 0; i < len; i++) {
		segsize = (int)segments[i].size();
		for (int j = 0; j < segsize; j++)
			labels.at<int>(segments[i][j].x, segments[i][j].y) = a;
		a++;
	}

	return labels;
}
