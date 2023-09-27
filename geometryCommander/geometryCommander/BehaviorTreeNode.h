#pragma once
class BehaviorTreeNode
{
protected:
	BehaviorTreeNode* leftChild;
	BehaviorTreeNode* rightChild;
	BehaviorTreeNode();
public:
	BehaviorTreeNode(BehaviorTreeNode* _leftChild, BehaviorTreeNode* _rightChild);
	~BehaviorTreeNode();
	virtual void Traverse();
};

