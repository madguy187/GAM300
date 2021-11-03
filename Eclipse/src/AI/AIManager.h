#pragma once
#include "ECS/ComponentManager/Components/AIComponent.h"
using ASNodeID = uint16_t;
namespace Eclipse
{
	struct grid
	{
		float _squareDist{ 0 };
	};

	struct BaseNode
	{
		ASNodeID _ID;
		ECVec3 Position;
		int hcost;
		int gcost;
		int fcost;
		BaseNode* prev;
	};

	struct NeighbourInfo
	{
		BaseNode* neighbour;
		int distfromNeighbour;
	};

	struct AstarNode
	{
		BaseNode* _Node;
		std::vector<NeighbourInfo> neighbours;
		
	};

	struct AstarPathComponent
	{


	};


	class AIManager
	{
	private:
		AstarNode current;
		std::list<AstarNode*> openlist;
		std::list<AstarNode*> closedlist;
		std::vector<AstarNode> _Nodes;
	public:
		/*AstarNode* GetNextCurrent();
		void CalculatePath(AstarNode& start, AstarNode& end);*/
	};
}