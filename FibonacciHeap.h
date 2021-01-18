
#ifndef _FIBONACCI_HEAP_H_
#define _FIBONACCI_HEAP_H_

#include <stdio.h>
#include "Node.h"

class FibonacciHeap
{
private:
	Node* rootListByRank[100];

	bool link(Node* root);	
	Node * minRoot;

public:

	FibonacciHeap();
	FibonacciHeap(Node *root);

	~FibonacciHeap();

	bool isEmpty();
	bool insertVertex(Node * node);	
	void decreaseKey(float delta, Node* vertex);

	Node* findMin();
	Node* deleteMin();
};

#endif
