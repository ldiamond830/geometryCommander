#pragma once
#include "BehaviorTreeNode.h"
template<class t>
class BehaviorTreeActionNode : public BehaviorTreeNode
{
private:
	void (t::*action)();
	t* parent;

public:
	BehaviorTreeActionNode(t* _parent, void (t::* _action)()) {
		action = _action;
		parent = _parent;
	}
	void Traverse() override {
		(parent->*action)();
	}
};

