#include "pch.h"
#include "BehaviourTree.h"
namespace Eclipse
{
	const std::vector<BehaviourTree::Node*>& BehaviourTree::CompositeNode::getChildren() const
	{
		return _Children;
	}

	void BehaviourTree::CompositeNode::AddChild(Node* Child)
	{
		_Children.emplace_back(Child);
	}

	std::vector<BehaviourTree::Node*> BehaviourTree::CompositeNode::childrenShuffled() const
	{
		std::vector<Node*> temp = _Children;
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(temp.begin(), temp.end(), g);
		return temp;
	}

	bool BehaviourTree::Selector::run()
	{
		for (Node* Child : getChildren())
		{
			if (Child->run())
				return true;
		}

		return false;
	}

	bool BehaviourTree::Sequence::run()
	{
		for (Node* Child : getChildren())
		{
			if (!Child->run())
				return false;
		}
		return true;
	}


	bool BehaviourTree::RandomSelector::run()
	{
		for (Node* child : childrenShuffled())
		{
			if (child->run())
				return true;
		}
		return false;
	}


	BehaviourTree::Node* BehaviourTree::DecoratorNode::getChild() const
	{
		return _Child;
	}

	void BehaviourTree::DecoratorNode::SetChild(Node* newChild)
	{
		if (_Child != nullptr)
			delete[] _Child;

		_Child = newChild;
	}

	bool BehaviourTree::Root::run()
	{
		return getChild()->run();
	}

	bool BehaviourTree::Repeater::run()
	{
		if (numRepeats == NOT_FOUND)
		{
			while (true)
			{
				getChild()->run();
			}
		}
		else
		{
			for (int i = 0; i < numRepeats - 1; i++)
				getChild()->run();

			return getChild()->run();
		}
	}

	BehaviourTree::Repeater::Repeater(int num)
		:numRepeats{num}
	{

	}
	BehaviourTree::BehaviourTree()
	: root {new Root}
	{
	}
	void BehaviourTree::setRootChild(Node* rootChild) const
	{
		root->SetChild(rootChild);
	}
	bool BehaviourTree::run() const
	{
		return root->run();
	}
	BehaviourTree::CheckBool::CheckBool(bool* t)
	{
		_bool = t;
	}
	bool BehaviourTree::CheckBool::run()
	{
		if (_bool == false)
			return false;

		for (Node* Child : getChildren())
		{
			if (!Child->run())
				return false;
		}
		return true;
	}
}
