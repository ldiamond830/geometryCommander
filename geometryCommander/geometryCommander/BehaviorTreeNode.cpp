#include "BehaviorTreeNode.h"

BehaviorTreeNode::BehaviorTreeNode()
{
}

BehaviorTreeNode::BehaviorTreeNode(BehaviorTreeNode* _leftChild, BehaviorTreeNode* _rightChild)
{
	leftChild = _leftChild;
	rightChild = _rightChild;
}

BehaviorTreeNode::~BehaviorTreeNode()
{
	delete leftChild;
	delete rightChild;
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
