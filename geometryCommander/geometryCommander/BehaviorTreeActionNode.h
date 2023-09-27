#pragma once
#include "BehaviorTreeNode.h"
class BehaviorTreeActionNode : public BehaviorTreeNode
{
private:
	void (*action)();

public:
	BehaviorTreeActionNode(void (*_action)());
	void Traverse() override;
};

