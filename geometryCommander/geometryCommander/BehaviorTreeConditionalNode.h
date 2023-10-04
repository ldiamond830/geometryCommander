#pragma once
#include "BehaviorTreeNode.h"
template<class t>
class BehaviorTreeConditionalNode : public BehaviorTreeNode
{
private:
	bool(t::*condition)();
	t* parent;
public:
	BehaviorTreeConditionalNode(t* _parent, BehaviorTreeNode* _leftChild, BehaviorTreeNode* _rightChild, bool(t::* _condition)()) {
		condition = _condition;
		parent = _parent;
		leftChild = _leftChild;
		rightChild = _rightChild;
	}
	void Traverse() override {
		if ((parent->*condition)()) {
			leftChild->Traverse();
		}
		else {
			rightChild->Traverse();
		}
	}
};

