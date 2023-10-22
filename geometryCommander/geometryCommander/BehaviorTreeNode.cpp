#include "BehaviorTreeNode.h"

BehaviorTreeNode::BehaviorTreeNode(BehaviorTreeNode* _leftChild, BehaviorTreeNode* _rightChild)
{
	leftChild = _leftChild;
	rightChild = _rightChild;
}

BehaviorTreeNode::BehaviorTreeNode()
{
	//only used for BehaviorTreeAction node to use polymorphism and those should always be leaf nodes so these don't need to be declared
	leftChild = nullptr;
	rightChild = nullptr;
}

BehaviorTreeNode::~BehaviorTreeNode()
{
	/*
	if (leftChild != nullptr) {
		delete leftChild;
		leftChild = nullptr;
	}
	if (rightChild != nullptr) {
		delete rightChild;
		rightChild = nullptr;
	}
	*/
	
}

void BehaviorTreeNode::Traverse()
{
	if (leftChild != nullptr) {
		leftChild->Traverse();
	}
	else if (rightChild != nullptr) {
		rightChild->Traverse();
	}
}
