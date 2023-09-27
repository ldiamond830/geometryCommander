#pragma once
#include "BehaviorTreeNode.h"
class BehaviorTreeConditionalNode : public BehaviorTreeNode
{
private:
	bool(*condition)();
public:
	BehaviorTreeConditionalNode(BehaviorTreeNode* _leftChild, BehaviorTreeNode* _rightChild, bool(*_condition)());
	void Traverse() override;
};

