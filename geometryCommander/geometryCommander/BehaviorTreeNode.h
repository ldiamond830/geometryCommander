#pragma once
#include "GridBox.h"
class BehaviorTreeNode
{
private:
	BehaviorTreeNode* leftChild;
	BehaviorTreeNode* rightChild;
public:
	BehaviorTreeNode(BehaviorTreeNode* _leftChild, BehaviorTreeNode* _rightChild);
	~BehaviorTreeNode();
	virtual void Traverse();
};

