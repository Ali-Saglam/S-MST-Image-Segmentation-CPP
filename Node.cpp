/*
Saglam, Ali, & Baykan, N. A. (2017).
"Sequential image segmentation based on min- imum spanning tree representation".
Pattern Recognition Letters, 87 , 155–162.
https://doi.org/10.1016/j.patrec.2016.06.001 .
*/

#include "Node.h"

bool Node::addChild(Node *node)
{
	if (children != NULL)
		children->addSibling(node);
	else
	{
		children = node;
		node->parent = this;
		rank = 1;
	}

	return true;
}

bool Node::addSibling(Node *node)
{
	Node* temp = rightMostSibling();
	if (temp == NULL)
		return false;

	temp->rightSibling = node;
	node->leftSibling = temp;
	node->parent = this->parent;
	node->rightSibling = NULL;

	if (parent)
		parent->rank++;

	return true;
}

bool Node::remove()
{
	if (parent)
	{
		parent->rank--;
		if (leftSibling)
			parent->children = leftSibling;
		else if (rightSibling)
			parent->children = rightSibling;
		else
			parent->children = NULL;
	}

	if (leftSibling)
		leftSibling->rightSibling = rightSibling;
	if (rightSibling)
		rightSibling->leftSibling = leftSibling;

	leftSibling = NULL;
	rightSibling = NULL;
	parent = NULL;

	return true;
}

Node* Node::leftMostSibling()
{
	if (this == NULL)
		return NULL;

	Node* temp = this;
	while (temp->leftSibling)
		temp = temp->leftSibling;
	return temp;
}

Node* Node::rightMostSibling()
{
	if (this == NULL)
		return NULL;

	Node* temp = this;
	while (temp->rightSibling)
		temp = temp->rightSibling;
	return temp;
}
