#pragma once
#include"Global.h"
#include <list>
namespace Eclipse
{
	class Node
	{
	public:
		virtual bool run() = 0;
	};

	class CompositeNode : public Node
	{
	private:
		NodeType _type;
		std::list<Node*> _Children;
	};
	class Selector:public CompositeNode
	{
	public:
		 bool run() override;
	};

}