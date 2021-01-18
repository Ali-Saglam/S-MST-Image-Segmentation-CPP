
// =========================================================================
//	Implementation of class Fibonacci Heap
// =========================================================================


#include "FibonacciHeap.h"

FibonacciHeap::FibonacciHeap()
{
	minRoot = NULL;
}

FibonacciHeap::FibonacciHeap(Node *root)
{
	this->minRoot = root;
	minRoot->parent = NULL;
	minRoot->children = NULL;
	minRoot->leftSibling = NULL;
	minRoot->rightSibling = NULL;
	minRoot->rank = 0;
}

FibonacciHeap::~FibonacciHeap()
{
	delete[] rootListByRank;
}

bool FibonacciHeap::isEmpty()
{
	return (minRoot == NULL);
}

bool FibonacciHeap::insertVertex(Node * node)
{
	if(node == NULL)
		return false;

	if(minRoot == NULL)
		minRoot = node;
	else
	{
		minRoot->addSibling(node);
		if(minRoot->key > node->key)
			minRoot = node;
	}
	return true;
}

Node* FibonacciHeap::findMin()
{
	return minRoot;
}

Node* FibonacciHeap::deleteMin()
{
	Node *temp = minRoot->children->leftMostSibling();
	Node *nextTemp = NULL;

	// Adding Children to root list		
	while(temp != NULL)
	{
		nextTemp = temp->rightSibling; // Save next Sibling
		temp->remove();
		minRoot->addSibling(temp);
		temp = nextTemp;
	}

	// Select the left-most sibling of minRoot
	temp = minRoot->leftMostSibling();

	// Remove minRoot and set it to any sibling, if there exists one
	if(temp == minRoot)
	{
		if(minRoot->rightSibling)
			temp = minRoot->rightSibling;
		else
		{
			// Heap is obviously empty
			Node* out = minRoot;
			minRoot->remove();
			minRoot = NULL;
			return out;
		}
	}
	Node* out = minRoot;
	minRoot->remove();
	minRoot = temp;

	// Initialize list of roots	
	for(int i=0; i<100; i++)
		rootListByRank[i] = NULL;
	
	while(temp)
	{
		// Check if key of current vertex is smaller than the key of minRoot
		if(temp->key < minRoot->key)
		{
			minRoot = temp;
		}

		nextTemp = temp->rightSibling;		
		link(temp);
		temp = nextTemp;
	}

	return out;	
}

bool FibonacciHeap::link(Node* root)
{
	// Insert Vertex into root list
	if(rootListByRank[root->rank] == NULL)
	{
		rootListByRank[root->rank] = root;
		return false;
	}
	else
	{
		// Link the two roots
		Node* linkVertex = rootListByRank[root->rank];
		rootListByRank[root->rank] = NULL;
		
		if(root->key < linkVertex->key || root == minRoot)
		{
			linkVertex->remove();
			root->addChild(linkVertex);
			if(rootListByRank[root->rank] != NULL)
				link(root);
			else
				rootListByRank[root->rank] = root;
		}
		else
		{
			root->remove();
			linkVertex->addChild(root);
			if(rootListByRank[linkVertex->rank] != NULL)
				link(linkVertex);
			else
				rootListByRank[linkVertex->rank] = linkVertex;
		}
		return true;
	}
}


void FibonacciHeap::decreaseKey(float delta, Node* vertex)
{
	vertex->key = delta;

	if(vertex->parent != NULL) // The vertex has a parent
	{
		// Remove vertex and add to root list
		vertex->remove();
		minRoot->addSibling(vertex);		
	}
	// Check if key is smaller than the key of minRoot
	if(vertex->key < minRoot->key)
		minRoot = vertex;
}
