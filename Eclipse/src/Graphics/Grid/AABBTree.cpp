#include "pch.h"
#include "AABBTree.h"
#include <stack>
#include "AABB.h"
#include "Graphics/Picker/PickingManager.h"

namespace Eclipse
{
    bool AABBNode::isLeaf() const
    {
        return leftNodeIndex == AABB_NULL_NODE;
    }

    AABBNode::AABBNode() :
        object(nullptr),
        parentNodeIndex(AABB_NULL_NODE),
        leftNodeIndex(AABB_NULL_NODE),
        rightNodeIndex(AABB_NULL_NODE),
        nextNodeIndex(AABB_NULL_NODE)
    {

    }

    AABBTree::AABBTree(unsigned initialSize) :
        RootNodeIndex(AABB_NULL_NODE),
        AllocatedNodeCount(0),
        NextFreeNoedIndex(0),
        NodeCapacity(initialSize),
        GrowthSize(initialSize)
    {
        AllNodes.reserve(initialSize);

        for (unsigned nodeIndex = 0; nodeIndex < initialSize; nodeIndex++)
        {
            AABBNode& node = AllNodes[nodeIndex];
            node.nextNodeIndex = nodeIndex + 1;
        }
        AllNodes[initialSize - 1].nextNodeIndex = AABB_NULL_NODE;
    }

    AABBTree::~AABBTree()
    {

    }

    unsigned AABBTree::AllocateNode()
    {
        // if we have no free tree nodes then grow the pool
        if (NextFreeNoedIndex == AABB_NULL_NODE)
        {
            assert(AllocatedNodeCount == NodeCapacity);

            NodeCapacity += GrowthSize;
            AllNodes.reserve(NodeCapacity);

            for (unsigned nodeIndex = AllocatedNodeCount; nodeIndex < NodeCapacity; nodeIndex++)
            {
                AABBNode& node = AllNodes[nodeIndex];
                node.nextNodeIndex = nodeIndex + 1;
            }
            AllNodes[NodeCapacity - 1].nextNodeIndex = AABB_NULL_NODE;
            NextFreeNoedIndex = AllocatedNodeCount;
        }

        unsigned nodeIndex = NextFreeNoedIndex;
        AABBNode& allocatedNode = AllNodes[nodeIndex];
        allocatedNode.parentNodeIndex = AABB_NULL_NODE;
        allocatedNode.leftNodeIndex = AABB_NULL_NODE;
        allocatedNode.rightNodeIndex = AABB_NULL_NODE;
        NextFreeNoedIndex = allocatedNode.nextNodeIndex;
        AllocatedNodeCount++;

        return nodeIndex;
    }

    void AABBTree::DeallocateNode(unsigned nodeIndex)
    {
        AABBNode& deallocatedNode = AllNodes[nodeIndex];
        deallocatedNode.nextNodeIndex = NextFreeNoedIndex;
        NextFreeNoedIndex = nodeIndex;
        AllocatedNodeCount--;
    }

    void AABBTree::InsertObject(const std::shared_ptr<IAABB>& object)
    {
        unsigned nodeIndex = AllocateNode();
        AABBNode& node = AllNodes[nodeIndex];

        node.aabb = object->getAABB();
        node.object = object;

        InsertLeaf(nodeIndex);
        AABBToNodeMap[object] = nodeIndex;
    }

    void AABBTree::RemoveObject(const std::shared_ptr<IAABB>& object)
    {
        unsigned nodeIndex = AABBToNodeMap[object];
        RemoveLeaf(nodeIndex);
        DeallocateNode(nodeIndex);
        AABBToNodeMap.erase(object);
    }

    void AABBTree::UpdateObject(const std::shared_ptr<IAABB>& object)
    {
        unsigned nodeIndex = AABBToNodeMap[object];
        UpdateLeaf(nodeIndex, object->getAABB());
    }

    std::vector<unsigned int> AABBTree::CheckOverlapAgainstGrid(DYN_AABB& object)
    {
        std::vector<unsigned int> overlaps;
        std::stack<unsigned> stack;
        DYN_AABB& testAabb = object;

        stack.push(RootNodeIndex);
        while (!stack.empty())
        {
            unsigned nodeIndex = stack.top();
            stack.pop();

            if (nodeIndex == AABB_NULL_NODE)
                continue;

            AABBNode& node = AllNodes[nodeIndex];

            if (node.aabb.Overlaps(testAabb))
            {
                if (node.isLeaf() && node.aabb.GetEntityID() != object.GetEntityID())
                {
                    if (overlaps.size() != 0)
                    {
                        unsigned int hi = overlaps[0];
                        node.aabb.DistanceToObject = node.aabb.Max.x - testAabb.Max.x;
                        float yo = engine->GridManager->GetDistanceToObject(hi);

                        if (abs(node.aabb.DistanceToObject) > abs(yo))
                        {
                            continue;
                        }
                        else
                        {
                            std::vector<unsigned int>::iterator it;
                            it = overlaps.begin();
                            overlaps.erase(it);
                            overlaps.push_back(node.aabb.GetEntityID());
                        }
                    }
                    else
                    {
                        engine->GridManager->SetDistance(node, object);
                        overlaps.push_back(node.aabb.GetEntityID());
                    }
                }
                else
                {
                    stack.push(node.leftNodeIndex);
                    stack.push(node.rightNodeIndex);
                }
            }
        }

        return overlaps;
    }

    std::vector<unsigned int> AABBTree::QueryAgainstTrustrum(AABBComponent& object)
    {
        std::vector<unsigned int> overlaps;

        if (AABBToNodeMap.size() == 0)
            return overlaps;

        if (AllNodes.size() == 0)
            return overlaps;

        std::stack<unsigned> stack;
        AABBComponent& CheckedObjectAgsintTree = object;

        stack.push(RootNodeIndex);
        while (!stack.empty())
        {
            unsigned nodeIndex = stack.top();
            stack.pop();

            if (nodeIndex == AABB_NULL_NODE)
                continue;

            AABBNode& node = AllNodes[nodeIndex];

            if (node.aabb.Overlaps(CheckedObjectAgsintTree))
            {
                if (node.isLeaf())
                {
                    overlaps.push_back(node.aabb.GetEntityID());
                }
                else
                {
                    stack.push(node.leftNodeIndex);
                    stack.push(node.rightNodeIndex);
                }
            }
        }

        return overlaps;
    }

    std::vector<unsigned int> AABBTree::CheckOverlapAgainstGrid(AABBComponent& object)
    {
        std::vector<unsigned int> overlaps;
        std::stack<unsigned> stack;
        AABBComponent& CheckedObjectAgsintTree = object;

        stack.push(RootNodeIndex);
        while (!stack.empty())
        {
            unsigned nodeIndex = stack.top();
            stack.pop();

            if (nodeIndex == AABB_NULL_NODE)
                continue;

            AABBNode& node = AllNodes[nodeIndex];

            if (node.aabb.Overlaps(CheckedObjectAgsintTree))
            {
                if (node.isLeaf())
                {
                    if (overlaps.size() != 0)
                    {
                        unsigned int FirstTileRegistered = overlaps[0];
                        engine->GridManager->SetDistance(node, CheckedObjectAgsintTree);
                        float RegisteredTileDistance = engine->GridManager->GetDistanceToObject(FirstTileRegistered);

                        if (abs(node.aabb.DistanceToObject) > abs(RegisteredTileDistance))
                        {
                            continue;
                        }
                        else
                        {
                            std::vector<unsigned int>::iterator it;
                            it = overlaps.begin();
                            overlaps.erase(it);
                            overlaps.push_back(node.aabb.GetEntityID());
                        }
                    }
                    else
                    {
                        engine->GridManager->SetDistance(node, object, node.aabb.GetEntityID());
                        overlaps.push_back(node.aabb.GetEntityID());
                    }
                }
                else
                {
                    stack.push(node.leftNodeIndex);
                    stack.push(node.rightNodeIndex);
                }
            }
        }

        return overlaps;
    }

    std::vector<unsigned int> AABBTree::CheckOverlap(DYN_AABB& object)
    {
        std::vector<unsigned int> overlaps;
        std::stack<unsigned> stack;
        DYN_AABB& testAabb = object;

        stack.push(RootNodeIndex);
        while (!stack.empty())
        {
            unsigned nodeIndex = stack.top();
            stack.pop();

            if (nodeIndex == AABB_NULL_NODE)
                continue;

            AABBNode& node = AllNodes[nodeIndex];

            if (node.aabb.Overlaps(testAabb))
            {
                if (node.isLeaf() && node.aabb.GetEntityID() != object.GetEntityID())
                {
                    overlaps.push_back(node.aabb.GetEntityID());
                }
                else
                {
                    stack.push(node.leftNodeIndex);
                    stack.push(node.rightNodeIndex);
                }
            }
        }

        return overlaps;
    }

    std::vector<unsigned int> AABBTree::CheckMouseOverlapAgainstGrid(glm::vec3& rayStart, glm::vec3& rayDir, float& t)
    {
        std::vector<unsigned int> overlaps;
        std::stack<unsigned> stack;

        stack.push(RootNodeIndex);
        while (!stack.empty())
        {
            unsigned nodeIndex = stack.top();
            stack.pop();

            if (nodeIndex == AABB_NULL_NODE) continue;

            AABBNode& node = AllNodes[nodeIndex];
            glm::vec3 AABBmin = { node.aabb.Min.x,node.aabb.Min.y , node.aabb.Min.z };
            glm::vec3 AABBmax = { node.aabb.Max.x ,node.aabb.Max.y , node.aabb.Max.z };

            //bool collision = PickingManager::RayAabb(rayStart, rayDir, AABBmin, AABBmax, t);      
            bool collision = engine->gPicker.RayAabb(rayStart, rayDir, AABBmin, AABBmax, t);

            if (collision)
            {
                if (node.isLeaf())
                {
                    overlaps.push_back(node.aabb.GetEntityID());
                }
                else
                {
                    stack.push(node.leftNodeIndex);
                    stack.push(node.rightNodeIndex);
                }
            }
        }

        return overlaps;
    }

    unsigned int AABBTree::NumberOfIntersections(std::vector<unsigned int>& in)
    {
        return in.size();
    }

    void AABBTree::ClearTree()
    {
        AABBToNodeMap.clear();
        //AllNodes.clear();
        RootNodeIndex = 0;
        AllocatedNodeCount = 0;
        NextFreeNoedIndex = 0;
    }

    void AABBTree::InsertLeaf(unsigned leafNodeIndex)
    {
        ENGINE_LOG_ASSERT(AllNodes[leafNodeIndex].parentNodeIndex == AABB_NULL_NODE, " Not Inserting a new Lead");
        ENGINE_LOG_ASSERT(AllNodes[leafNodeIndex].leftNodeIndex == AABB_NULL_NODE, " Not Inserting a new Lead");
        ENGINE_LOG_ASSERT(AllNodes[leafNodeIndex].rightNodeIndex == AABB_NULL_NODE, " Not Inserting a new Lead");

        // if the tree is empty then we make the root the leaf
        if (RootNodeIndex == AABB_NULL_NODE)
        {
            RootNodeIndex = leafNodeIndex;
            return;
        }

        // search for the best place to put the new leaf in the tree
        // we use surface area and depth 
        unsigned treeNodeIndex = RootNodeIndex;
        AABBNode& leafNode = AllNodes[leafNodeIndex];

        while (!AllNodes[treeNodeIndex].isLeaf())
        {
            // because of the test in the while loop above we know we are never a leaf inside it
            const AABBNode& treeNode = AllNodes[treeNodeIndex];
            unsigned leftNodeIndex = treeNode.leftNodeIndex;
            unsigned rightNodeIndex = treeNode.rightNodeIndex;
            const AABBNode& leftNode = AllNodes[leftNodeIndex];
            const AABBNode& rightNode = AllNodes[rightNodeIndex];

            DYN_AABB combinedAabb = treeNode.aabb.Merge(leafNode.aabb);

            float newParentNodeCost = 2.0f * combinedAabb.surfaceArea;
            float minimumPushDownCost = 2.0f * (combinedAabb.surfaceArea - treeNode.aabb.surfaceArea);

            // use the costs to figure out whether to create a new parent here or descend
            float costLeft;
            float costRight;
            if (leftNode.isLeaf())
            {
                costLeft = leafNode.aabb.Merge(leftNode.aabb).surfaceArea + minimumPushDownCost;
            }
            else
            {
                DYN_AABB newLeftAabb = leafNode.aabb.Merge(leftNode.aabb);
                costLeft = (newLeftAabb.surfaceArea - leftNode.aabb.surfaceArea) + minimumPushDownCost;
            }
            if (rightNode.isLeaf())
            {
                costRight = leafNode.aabb.Merge(rightNode.aabb).surfaceArea + minimumPushDownCost;
            }
            else
            {
                DYN_AABB newRightAabb = leafNode.aabb.Merge(rightNode.aabb);
                costRight = (newRightAabb.surfaceArea - rightNode.aabb.surfaceArea) + minimumPushDownCost;
            }

            // if the cost of creating a new parent node here is less than descending in either direction then
            // we know we need to create a new parent node, errrr, here and attach the leaf to that
            if (newParentNodeCost < costLeft && newParentNodeCost < costRight)
            {
                break;
            }

            // otherwise descend in the cheapest direction
            if (costLeft < costRight)
            {
                treeNodeIndex = leftNodeIndex;
            }
            else
            {
                treeNodeIndex = rightNodeIndex;
            }
        }

        // the leafs sibling is going to be the node we found above so create a new parent node and attach the leaf and this item
        unsigned leafSiblingIndex = treeNodeIndex;
        AABBNode& leafSibling = AllNodes[leafSiblingIndex];
        unsigned oldParentIndex = leafSibling.parentNodeIndex;
        unsigned newParentIndex = AllocateNode();

        AABBNode& newParent = AllNodes[newParentIndex];
        newParent.parentNodeIndex = oldParentIndex;

        // the new parents aabb is the leaf aabb combined with it's siblings aabb
        newParent.aabb = leafNode.aabb.Merge(leafSibling.aabb);
        newParent.leftNodeIndex = leafSiblingIndex;
        newParent.rightNodeIndex = leafNodeIndex;

        leafNode.parentNodeIndex = newParentIndex;
        leafSibling.parentNodeIndex = newParentIndex;

        if (oldParentIndex == AABB_NULL_NODE)
        {
            // the old parent was the root and so this is now the root
            RootNodeIndex = newParentIndex;
        }
        else
        {
            // the old parent was not the root and so we need to patch the left or right index to
            // point to the new node
            AABBNode& oldParent = AllNodes[oldParentIndex];
            if (oldParent.leftNodeIndex == leafSiblingIndex)
            {
                oldParent.leftNodeIndex = newParentIndex;
            }
            else
            {
                oldParent.rightNodeIndex = newParentIndex;
            }
        }

        // finally we need to walk back up the tree fixing heights and areas
        treeNodeIndex = leafNode.parentNodeIndex;
        FixUpwardsTree(treeNodeIndex);
    }

    void AABBTree::RemoveLeaf(unsigned leafNodeIndex)
    {
        // if the leaf is the root then we can just clear the root pointer and return
        if (leafNodeIndex == RootNodeIndex)
        {
            RootNodeIndex = AABB_NULL_NODE;
            return;
        }

        AABBNode& leafNode = AllNodes[leafNodeIndex];
        unsigned parentNodeIndex = leafNode.parentNodeIndex;
        const AABBNode& parentNode = AllNodes[parentNodeIndex];
        unsigned grandParentNodeIndex = parentNode.parentNodeIndex;
        unsigned siblingNodeIndex = parentNode.leftNodeIndex == leafNodeIndex ? parentNode.rightNodeIndex : parentNode.leftNodeIndex;
        assert(siblingNodeIndex != AABB_NULL_NODE); // we must have a sibling
        AABBNode& siblingNode = AllNodes[siblingNodeIndex];

        if (grandParentNodeIndex != AABB_NULL_NODE)
        {
            // if we have a grand parent, destroy the parent and connect the sibling to the grandparent in its place
            AABBNode& grandParentNode = AllNodes[grandParentNodeIndex];
            if (grandParentNode.leftNodeIndex == parentNodeIndex)
            {
                grandParentNode.leftNodeIndex = siblingNodeIndex;
            }
            else
            {
                grandParentNode.rightNodeIndex = siblingNodeIndex;
            }
            siblingNode.parentNodeIndex = grandParentNodeIndex;
            DeallocateNode(parentNodeIndex);

            FixUpwardsTree(grandParentNodeIndex);
        }
        else
        {
            // if we have no grandparent then the parent is the root and so our sibling becomes the root and has it's parent removed
            RootNodeIndex = siblingNodeIndex;
            siblingNode.parentNodeIndex = AABB_NULL_NODE;
            DeallocateNode(parentNodeIndex);
        }

        leafNode.parentNodeIndex = AABB_NULL_NODE;
    }

    void AABBTree::UpdateLeaf(unsigned leafNodeIndex, const DYN_AABB& newAaab)
    {
        AABBNode& node = AllNodes[leafNodeIndex];

        if (node.aabb.Contains(newAaab))
        {
            EDITOR_LOG_WARN("Failed to Add Node");
            return;
        }

        RemoveLeaf(leafNodeIndex);
        node.aabb = newAaab;
        InsertLeaf(leafNodeIndex);
    }


    void AABBTree::FixUpwardsTree(unsigned treeNodeIndex)
    {
        while (treeNodeIndex != AABB_NULL_NODE)
        {
            AABBNode& treeNode = AllNodes[treeNodeIndex];

            ENGINE_LOG_ASSERT(treeNode.leftNodeIndex != AABB_NULL_NODE && treeNode.rightNodeIndex != AABB_NULL_NODE, "Every Node should be a Parent");

            // fix height and area
            const AABBNode& leftNode = AllNodes[treeNode.leftNodeIndex];
            const AABBNode& rightNode = AllNodes[treeNode.rightNodeIndex];
            treeNode.aabb = leftNode.aabb.Merge(rightNode.aabb);

            treeNodeIndex = treeNode.parentNodeIndex;
        }
    }
}