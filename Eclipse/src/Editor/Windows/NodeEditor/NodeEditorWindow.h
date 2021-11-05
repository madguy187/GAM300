#pragma once

#include "NodeEditor.h"

namespace Eclipse
{
	class NodeEditorWindow : public ECGuiWindow
	{
		bool initialized= false;
		NodeEditor sceneEditor;
	public :

		NodeEditorWindow();
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();
		~NodeEditorWindow();


	};


	//template<typename T>
	//void NodeEditor::CompareCast(std::shared_ptr<T>& lhsNode, std::shared_ptr<T>& rhsNode)
	//{
	//	NodeEditor::Node::NodeType type1 = lhsNode->getType();
	//	NodeEditor::Node::NodeType type2 = rhsNode->getType();
	//	const bool linked = type1 != type2;

	//	if (linked)
	//	{
	//		// Output Nodes
	//		for (int index = static_cast<int>(NodeEditor::Node::NodeType::ENTITY); 
	//			index != static_cast<int>(NodeEditor::Node::NodeType::TRANSFORM); ++index)
	//		{
	//			NodeEditor::Node::NodeType Out_NT = static_cast<NodeEditor::Node::NodeType>(index);
	//			
	//			if (type1 == Out_NT)
	//			{
	//				// Input nodes
	//				for (int index = static_cast<int>(NodeEditor::Node::NodeType::TRANSFORM);
	//					index != static_cast<int>(NodeEditor::Node::NodeType::COUNT); ++index)
	//				{
	//					NodeEditor::Node::NodeType In_NT = static_cast<NodeEditor::Node::NodeType>(index);

	//					if (type2 == In_NT)
	//					{
	//						// DynamicCastNodes<T>(In_NT, rhsNode)->ID = DynamicCastNodes<T>(Out_NT, lhsNode)->ID;
	//						break;
	//					}
	//				}

	//				break;
	//			}
	//		}
	//	}
	//}
}

