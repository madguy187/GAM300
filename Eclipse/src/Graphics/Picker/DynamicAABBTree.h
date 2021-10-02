#pragma once

#include <memory>
#include <unordered_map>
#include "ECS/ComponentManager/Components/AABBComponent.h"

#ifndef DYNAMICAABBTREE_H
#define DYNAMICAABBTREE_H

namespace Eclipse
{
	class DynamicAABBTree
	{
		const float mFatteningFactor = 1.1f;

		struct Aabb
		{
			Aabb();
			Aabb(const glm::vec3 min, const glm::vec3& max);

			static Aabb BuildFromCenterAndHalfExtents(const glm::vec3& center, const glm::vec3& halfExtents);

			float GetSurfaceArea() const;
			bool Contains(const Aabb& aabb) const;
			static Aabb Combine(const Aabb& lhs, const Aabb& rhs);

			glm::vec3 mMin;
			glm::vec3 mMax;
		};

		struct Node
		{
			unsigned int ID = MAX_ENTITY;
			Aabb mAabb;
			Node* mLeft = nullptr;
			Node* mRight = nullptr;
			Node* mParent = nullptr;
			size_t mHeight = 0;

			size_t lastAxis = 0;
		};

		Node* root = nullptr;

		void InsertNode(Node* newNode, Node*& currNode);
		void BalanceAVLTree(Node* oldParent);
		bool isExternalNode(Node* node);
		size_t Height(Node* node);
		Node* FindNode(unsigned int ID);
		void RemoveData(unsigned int ID);

		std::unordered_map<unsigned int, Node*> TreeNodes;
	public:
		DynamicAABBTree();
		~DynamicAABBTree();

		void InsertData(unsigned int ID);
		void UpdateData(unsigned int ID);
		unsigned int RayCast(Node* node, glm::vec3 rayStart, glm::vec3 rayDir);

		Node* GetTreeRoot();
		void DeleteTree(Node* node);
	};
}

#endif  /* DYNAMICAABBTREE_H */