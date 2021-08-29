#ifndef DYNAMICTREE_AABBTREE_H
#define DYNAMICTREE_AABBTREE_H
#include "AABB.h"
#include "IAABB.h"
#include "Graphics/ModelFactory/AssimpModel/AssimpModel.h"

#define AABB_NULL_NODE 0xffffffff

namespace Eclipse
{
    struct AABBNode
    {
        AABB aabb;
        std::shared_ptr<IAABB> object;

        unsigned int parentNodeIndex = 0;
        unsigned int leftNodeIndex = 0;
        unsigned int rightNodeIndex = 0;
        unsigned int nextNodeIndex = 0;

        bool isLeaf() const;
        AABBNode();
    };

    class AABBTree
    {
    private:
        std::map<std::shared_ptr<IAABB>, unsigned int> AABBToNodeMap;
        std::unordered_map<unsigned int , AABBNode> AllNodes;
        unsigned int RootNodeIndex = 0;
        unsigned int AllocatedNodeCount = 0;
        unsigned int NextFreeNoedIndex = 0;
        unsigned int NodeCapacity = 0;
        unsigned int GrowthSize = 0;

    public:
        AABBTree(unsigned initialSize);
        ~AABBTree();

        void InsertObject(const std::shared_ptr<IAABB>& object);
        void RemoveObject(const std::shared_ptr<IAABB>& object);
        void UpdateObject(const std::shared_ptr<IAABB>& object);
        std::vector<unsigned int> CheckOverlap(AABB& object);

    private:
        unsigned AllocateNode();
        void DeallocateNode(unsigned nodeIndex);
        void InsertLeaf(unsigned leafNodeIndex);
        void RemoveLeaf(unsigned leafNodeIndex);
        void UpdateLeaf(unsigned leafNodeIndex, const AABB& newAaab);
        void FixUpwardsTree(unsigned treeNodeIndex);

    };
}
#endif // DYNAMICTREE_AABB_H