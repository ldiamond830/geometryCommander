#pragma once
#include "BehaviorTreeNode.h"
template<class t>
class BehaviorTreeConditionalNode : public BehaviorTreeNode
{
private:
	bool(t::*condition)();
	t* parent;
public:
	BehaviorTreeConditionalNode(t* _parent, BehaviorTreeNode* _trueChild, BehaviorTreeNode* _falseChild, bool(t::* _condition)()) {
		condition = _condition;
		parent = _parent;
		leftChild = _trueChild;
		rightChild = _falseChild;
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

