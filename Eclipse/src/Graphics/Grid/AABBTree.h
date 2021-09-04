#ifndef DYNAMICTREE_AABBTREE_H
#define DYNAMICTREE_AABBTREE_H
#include "AABB.h"
#include "IAABB.h"
#include "Graphics/ModelFactory/AssimpModel/AssimpModel.h"
#include "ECS/ComponentManager/Components/AabbComponent.h"
#include "Graphics/Grid/Grid.h"

#define AABB_NULL_NODE 0xffffffff

namespace Eclipse
{
    struct AABBNode
    {
        DYN_AABB aabb;
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
        unsigned int RootNodeIndex = 0;
        unsigned int AllocatedNodeCount = 0;
        unsigned int NextFreeNoedIndex = 0;
        unsigned int NodeCapacity = 0;
        unsigned int GrowthSize = 0;

    public:
        std::unordered_map<unsigned int, AABBNode> AllNodes;
        AABBTree(unsigned initialSize);
        ~AABBTree();

        void InsertObject(const std::shared_ptr<IAABB>& object);
        void RemoveObject(const std::shared_ptr<IAABB>& object);
        void UpdateObject(const std::shared_ptr<IAABB>& object);
        std::vector<unsigned int> CheckOverlapAgainstGrid(DYN_AABB& object);
        std::vector<unsigned int> CheckOverlapAgainstGrid(AabbComponent& object);
        std::vector<unsigned int> CheckOverlap(DYN_AABB& object);
        std::vector<unsigned int> CheckMouseOverlapAgainstGrid(glm::vec3& rayStart, glm::vec3& rayDir, float& t);
        unsigned int NumberOfIntersections(std::vector<unsigned int>& in);

    private:
        unsigned AllocateNode();
        void DeallocateNode(unsigned nodeIndex);
        void InsertLeaf(unsigned leafNodeIndex);
        void RemoveLeaf(unsigned leafNodeIndex);
        void UpdateLeaf(unsigned leafNodeIndex, const DYN_AABB& newAaab);
        void FixUpwardsTree(unsigned treeNodeIndex);

    };
}
#endif // DYNAMICTREE_AABB_H