#include "pch.h"
#include "DynamicAABBTree.h"

namespace Eclipse
{
	//DynamicAABBTree::~DynamicAABBTree()
	//{
	//
	//}

	void DynamicAABBTree::InsertNode(Node* newNode, Node*& currNode)
	{
		if (!currNode)
		{
			currNode = newNode;
		}
		else
		{
			if (!isExternalNode(currNode))
			{
				Aabb newRight = Aabb::Combine(currNode->mRight->mAabb, newNode->mAabb);
				Aabb newLeft = Aabb::Combine(currNode->mLeft->mAabb, newNode->mAabb);

				float diffRight = std::abs(currNode->mRight->mAabb.GetSurfaceArea() - newRight.GetSurfaceArea());
				float diffLeft = std::abs(currNode->mLeft->mAabb.GetSurfaceArea() - newLeft.GetSurfaceArea());

				if (diffLeft < diffRight)
				{
					InsertNode(newNode, currNode->mLeft);
				}
				else
				{
					InsertNode(newNode, currNode->mRight);
				}
			}
			else
			{
				Node* newCombined = new Node;
				newCombined->mAabb = Aabb::Combine(currNode->mAabb, newNode->mAabb);
				newCombined->mLeft = currNode;
				newCombined->mRight = newNode;
				newCombined->mParent = currNode->mParent;
				newCombined->mHeight = 1;

				currNode->mParent = newCombined;
				newNode->mParent = newCombined;

				currNode->mHeight = 0;
				newNode->mHeight = 0;
				currNode->mLeft = nullptr;
				newNode->mLeft = nullptr;
				currNode->mRight = nullptr;
				newNode->mRight = nullptr;

				currNode = newCombined;

				if (currNode->mParent)
				{
					currNode->mParent->mHeight = 1 + (std::max)(Height(currNode->mParent->mLeft), Height(currNode->mParent->mRight));
					currNode->mParent->mAabb = Aabb::Combine(currNode->mParent->mLeft->mAabb, currNode->mParent->mRight->mAabb);
				}
			}
		}
	}

	void DynamicAABBTree::BalanceAVLTree(std::shared_ptr<Node> oldParent)
	{
		//if (!oldParent)
		//{
		//	return;
		//}
		//
		//size_t heightDiff = std::abs(static_cast<int>(oldParent->mLeft->mHeight - oldParent->mRight->mHeight));
		//
		//if (heightDiff > 1)
		//{
		//	std::shared_ptr<Node*> pivot = std::make_shared<Node*>();
		//	
		//	if (oldParent->mLeft->mHeight > oldParent->mRight->mHeight)
		//	{
		//		pivot = std::reinterpret_pointer_cast<Node*>(oldParent->mLeft);
		//	}
		//	else
		//	{
		//		pivot = std::reinterpret_pointer_cast<Node*>(oldParent->mRight);
		//	}
		//	
		//	std::shared_ptr<Node*> smallPivot = std::make_shared<Node*>();
		//	
		//	if ((*pivot)->mLeft->mHeight < (*pivot)->mRight->mHeight)
		//	{
		//		smallPivot = std::reinterpret_pointer_cast<Node*>((*pivot)->mLeft);
		//	}
		//	else
		//	{
		//		smallPivot = std::reinterpret_pointer_cast<Node*>((*pivot)->mRight);
		//	}
		//	
		//	std::shared_ptr<Node> grandParent;
		//	grandParent = oldParent->mParent;
		//	oldParent->mParent = std::reinterpret_pointer_cast<Node>(pivot);
		//	(*smallPivot)->mParent = oldParent;
		//	*pivot = *smallPivot;
		//	
		//	if (grandParent)
		//	{
		//		if (grandParent->mLeft == oldParent)
		//		{
		//			grandParent->mLeft = oldParent->mParent;
		//		}
		//		else if (grandParent->mRight == oldParent)
		//		{
		//			grandParent->mRight = oldParent->mParent;
		//		}
		//	}
		//	
		//	smallPivot = std::reinterpret_pointer_cast<Node*>(oldParent);
		//	
		//	if (oldParent == root)
		//	{
		//		root = oldParent->mParent;
		//	}
		//	
		//	oldParent->mAabb = Aabb::Combine(oldParent->mLeft->mAabb, oldParent->mRight->mAabb);
		//	oldParent->mHeight = 1 + (std::max)(Height(oldParent->mLeft), Height(oldParent->mRight));
		//}
		//else
		//{
		//	oldParent->mAabb = Aabb::Combine(oldParent->mLeft->mAabb, oldParent->mRight->mAabb);
		//	oldParent->mHeight = 1 + (std::max)(Height(oldParent->mLeft), Height(oldParent->mRight));
		//}
		//
		//BalanceAVLTree(oldParent->mParent);
	}

	bool DynamicAABBTree::isExternalNode(Node* node)
	{
		if (!node->mLeft && !node->mRight)
		{
			return true;
		}

		return false;
	}

	size_t DynamicAABBTree::Height(Node* node)
	{
		if (!node)
		{
			return -1;
		}

		return node->mHeight;
	}

	std::shared_ptr<DynamicAABBTree::Node> DynamicAABBTree::FindNode(std::shared_ptr<Node> currNode, unsigned int ID)
	{
		//if (currNode->ID == ID)
		//{
		//	return currNode;
		//}
		//
		//if (currNode->mLeft)
		//{
		//	return FindNode(currNode->mLeft, ID);
		//}
		//else if (currNode->mRight)
		//{
		//	return FindNode(currNode->mRight, ID);
		//}
		//
		return nullptr;
	}

	void DynamicAABBTree::RemoveData(std::shared_ptr<Node> node)
	{
		//std::shared_ptr<Node> removeNode = std::make_shared<Node>();
		//removeNode = node;
		//
		//std::shared_ptr<Node> parent = std::make_shared<Node>();
		//parent = removeNode->mParent;
		//
		//if (parent)
		//{
		//	std::shared_ptr<Node> sibling = std::make_shared<Node>();
		//
		//	if (removeNode == parent->mLeft)
		//	{
		//		sibling = parent->mRight;
		//	}
		//	else
		//	{
		//		sibling = parent->mLeft;
		//	}
		//
		//	std::shared_ptr<Node> grandParent = std::make_shared<Node>();
		//	grandParent = parent->mParent;
		//
		//	if (grandParent)
		//	{
		//		if (grandParent->mLeft == parent)
		//		{
		//			grandParent->mLeft = sibling;
		//		}
		//		else
		//		{
		//			grandParent->mRight = sibling;
		//		}
		//
		//		sibling->mParent = grandParent;
		//
		//		grandParent->mAabb = Aabb::Combine(grandParent->mLeft->mAabb, grandParent->mRight->mAabb);
		//
		//		BalanceAVLTree(grandParent);
		//	}
		//	else
		//	{
		//		root = sibling;
		//		sibling->mParent = nullptr;
		//	}
		//}
		//else
		//{
		//	//Empty tree
		//	root = nullptr;
		//}
	}

	void DynamicAABBTree::InsertData(unsigned int ID)
	{
		//auto& AABB = engine->world.GetComponent<AABBComponent>(ID);
		//
		//glm::vec3 halfExt = (AABB.Max.ConvertToGlmVec3Type() - AABB.Min.ConvertToGlmVec3Type()) * 0.5f;
		//halfExt *= DynamicAABBTree::mFatteningFactor;
		//
		//Aabb newAabb = Aabb::BuildFromCenterAndHalfExtents(AABB.center.ConvertToGlmVec3Type(), halfExt);
		//
		//std::shared_ptr<Node> newData = std::make_shared<Node>();
		//newData->ID = ID;
		//newData->mAabb = newAabb;
		//newData->mLeft = nullptr;
		//newData->mRight = nullptr;
		//newData->mParent = nullptr;
		//newData->mHeight = 0;
		//
		//InsertNode(std::move(newData), root);

		//if (newData->mParent && newData->mParent->mParent && newData->mParent->mParent->mParent)
		//{
		//	BalanceAVLTree(newData->mParent->mParent->mParent);
		//}
	}

	void DynamicAABBTree::UpdateData(unsigned int ID)
	{
		//std::shared_ptr<Node> node = FindNode(root, ID);
		//
		//auto& AABB = engine->world.GetComponent<AABBComponent>(ID);
		//glm::vec3 halfExt = (AABB.Max.ConvertToGlmVec3Type() - AABB.Min.ConvertToGlmVec3Type()) * 0.5f;
		//halfExt *= DynamicAABBTree::mFatteningFactor;
		//
		//Aabb newAabb = Aabb::BuildFromCenterAndHalfExtents(AABB.center.ConvertToGlmVec3Type(), halfExt);
		//
		//if (!node->mAabb.Contains(newAabb))
		//{
		//	RemoveData(node);
		//	InsertData(ID);
		//}
	}

	unsigned int DynamicAABBTree::RayCast(std::shared_ptr<Node> node, glm::vec3 rayStart, glm::vec3 rayDir)
	{
		//static unsigned int nodeID = MAX_ENTITY;
		//
		//if (!node)
		//{
		//	return nodeID;
		//}
		//
		//float time;
		//bool checkIntersect = engine->gPicker.RayAabb(rayStart, rayDir, node->mAabb.mMin, node->mAabb.mMax, time);
		//
		//if (!checkIntersect)
		//{
		//	return nodeID;
		//}
		//else
		//{
		//	if (!isExternalNode(node))
		//	{
		//		RayCast(node->mLeft, rayStart, rayDir);
		//		RayCast(node->mRight, rayStart, rayDir);
		//	}
		//}

		return 0;
	}

	DynamicAABBTree::Aabb::Aabb()
	{
		mMin = glm::vec3{ (std::numeric_limits<float>::min)() };
		mMax = glm::vec3{ (std::numeric_limits<float>::max)() };
	}

	DynamicAABBTree::Aabb::Aabb(const glm::vec3 min, const glm::vec3& max)
	{
		mMin = min;
		mMax = max;
	}

	DynamicAABBTree::Aabb DynamicAABBTree::Aabb::BuildFromCenterAndHalfExtents(const glm::vec3& center, const glm::vec3& halfExtents)
	{
		return Aabb(center - halfExtents, center + halfExtents);
	}

	float DynamicAABBTree::Aabb::GetSurfaceArea() const
	{
		float dx = this->mMax.x - this->mMin.x;
		float dy = this->mMax.y - this->mMin.y;
		float dz = this->mMax.z - this->mMin.z;

		float surfaceArea = (2.0f * dx * dy) + (2.0f * dy * dz) + (2.0f * dx * dz);

		return surfaceArea;
	}

	bool DynamicAABBTree::Aabb::Contains(const Aabb& aabb) const
	{
		if (aabb.mMax.x < this->mMax.x && aabb.mMin.x > this->mMin.x &&
			aabb.mMax.y < this->mMax.y && aabb.mMin.y > this->mMin.y &&
			aabb.mMax.z < this->mMax.z && aabb.mMin.z > this->mMin.z)
		{
			return true;
		}

		return false;
	}

	DynamicAABBTree::Aabb DynamicAABBTree::Aabb::Combine(const Aabb& lhs, const Aabb& rhs)
	{
		Aabb result;

		for (size_t i = 0; i < 3; ++i)
		{
			result.mMin[i] = (std::min)(lhs.mMin[i], rhs.mMin[i]);
			result.mMax[i] = (std::max)(lhs.mMax[i], rhs.mMax[i]);
		}

		return result;;
	}
}


