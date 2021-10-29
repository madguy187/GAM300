#pragma once
#include"pch.h"
namespace Eclipse
{
	class BehaviourTree
	{
		class Node
		{
		public:
			NodeType _type;
			virtual bool run() = 0;
		};

		class CompositeNode : public Node
		{
		private:
			std::vector<Node*> _Children;
			Node* Parent;
		public:
			const std::vector<Node*>& getChildren() const;
			void AddChild(Node* Child);
			std::vector<Node*> childrenShuffled() const;
		};
		class Selector :public CompositeNode
		{
		public:
			virtual bool run() override;
		};
		class RandomSelector :public CompositeNode
		{
		public:
			virtual bool run() override;
		};

		class Sequence :public CompositeNode
		{
		public:
			virtual bool run() override;
		};

		class CheckBool : public BehaviourTree::CompositeNode
		{
		private:
			bool* _bool;
		public:
			CheckBool(bool* t);

			virtual bool run() override;
		};

		class DecoratorNode : public Node
		{
		private:
			Node* _Child;
		protected:
			Node* getChild() const;
		public:
			void SetChild(Node* newChild);
		};

		class Root : public DecoratorNode
		{
		private:
			friend class BehaviourTree;
			virtual bool run() override;
		};

		class Repeater : public DecoratorNode
		{
		private:
			int numRepeats;
			static const int NOT_FOUND = -1;
		public:
			virtual bool run() override;
			Repeater(int num = NOT_FOUND);
		};

		template<typename T>
		class SetVariable : public BehaviourTree::Node 
		{
		private: 
			T& variable , &object;
		public:
			SetVariable(T& t, T& obj)
				:variable{ t }, object{obj}
			{
			}
			virtual bool run() override
			{
				variable = object;
				return true;
			}
		};



	private:
		Root* root;
	public:
		BehaviourTree();
		void setRootChild(Node* rootChild) const;
		bool run() const;

	};
}
