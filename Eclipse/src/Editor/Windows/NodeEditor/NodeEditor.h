#pragma once
#include "../Interface/ECGuiWindow.h"
#include <imgui_internal.h>
#include "ImNodes/imnodes.h"
#include <imnodes.h>

#define DEFAULTVAL 99999
namespace Eclipse
{
	static constexpr unsigned int OUTPUT_FLAG = 1 << 31;

	struct NodeEditor
	{

		struct Node;

		struct	Node
		{
			enum class NodeType
			{
				DEFAULT,
				// Start of Output Nodes 
				ENTITY,
				// Start of Input Nodes
				TRANSFORM,
				COUNT
			};


			Node(NodeEditor& data)
				: res(data), nodeId(data.generatedId()) {}

			virtual ~Node() {}
			virtual NodeType getType() const = 0;

			void beginInput(ImNodesPinShape pinShape = 1)
			{
				int bitshift = (5 << 16);
				int temp = nodeId ^ (5 << 16);
				ImNodes::BeginInputAttribute(temp, pinShape);
			}

			static void endInput()
			{
				ImNodes::EndInputAttribute();
			}

			void beginOutput(ImNodesPinShape pinShape = 1)
			{
				int bitshift = (5 << 13);
				int temp = nodeId ^ (5 << 13);
				ImNodes::BeginOutputAttribute(temp, pinShape);
			}

			static void endOutput()
			{
				ImNodes::EndOutputAttribute();
			}

			bool nodeOnGui()
			{
				inputCounter = 0;
				outputCounter = 0;
				ImNodes::SetNodeEditorSpacePos(nodeId, editorSpacePos);
				ImNodes::BeginNode(nodeId);
				bool res = onGUI();
				ImNodes::EndNode();
				return res;
			}

			int nodeId;
			ImVec2 editorSpacePos = ImVec2(100, 100);
		protected:

			virtual bool onGUI() = 0;

			NodeEditor& res;
			unsigned char inputCounter;
			unsigned char outputCounter;
		};
		struct EnitityNode : public Node
		{
			EnitityNode(NodeEditor& res, Entity id)
				: Node(res), ID(id) {}

			~EnitityNode() {};

			NodeType getType() const override { return NodeType::ENTITY; }

			bool onGUI() override {
				getName();
				beginOutput();
				ImGui::SetNextItemWidth(60);
				ECGui::DrawTextWidget<std::string>("Edit Name", name);
				ECGui::DrawTextWidget<std::string>("Entity ID", std::to_string(ID));
				endOutput();
				return false;
			}

			void getName()
			{

				if (engine->world.CheckComponent<EntityComponent>(ID))
				{
					auto entComp = engine->world.GetComponent<EntityComponent>(ID);

					name = entComp.Name;
				}

			}

			std::string name;
			Entity ID = DEFAULTVAL;
		};
		struct TransformNode : public Node
		{
			TransformNode(NodeEditor& res) : Node(res) {}

			NodeType getType() const override { return NodeType::TRANSFORM; }

			bool onGUI() override {
				ImNodes::BeginNodeTitleBar();
				ImGui::TextUnformatted("TransFormNode");
				ImNodes::EndNodeTitleBar();

				beginInput(ImNodesPinShape_Quad);
				ImGui::SetNextItemWidth(160);
				if (ID == DEFAULTVAL)
				{
					ECGui::DrawTextWidget<std::string>("No Entity", EMPTY_STRING);
				}
				else
				{
					ECGui::DrawTextWidget<std::string>("Entity ID", std::to_string(ID));
				}
				if (ID != DEFAULTVAL)
				{
					if (engine->world.CheckComponent<TransformComponent>(ID))
					{
						ImGui::SetNextItemWidth(160);
						auto& transCom = engine->world.GetComponent<TransformComponent>(ID);
						ECGui::DrawSliderFloat3Widget("TransVec", &transCom.position, true, -100.f, 100.f, ID);
						ImGui::SetNextItemWidth(160);
						ECGui::DrawSliderFloat3Widget("Rotation", &transCom.rotation, true, -100.f, 100.f, ID);
						ImGui::SetNextItemWidth(160);
						ECGui::DrawSliderFloat3Widget("Scale", &transCom.scale, true, -100.f, 100.f, ID);
					}
				}
				endInput();

				return false;
			}

			void factoryReset()
			{
				ID = DEFAULTVAL;
			}

			Entity ID = DEFAULTVAL;
		};

		Node* getNodeByID(unsigned short id) const {
			for (auto& node : nodes) {
				if (node->nodeId == id)
				{
					return node.get();
				}
			}
			return nullptr;
		}

		unsigned short generatedId()
		{
			return ++current_id;
		}

		struct Link
		{
			int linkId;
			int startPoint, endPoint;
		};

		Node* addNode(Node::NodeType type)
		{
			std::shared_ptr<Node> node;

			switch (type)
			{
			case Node::NodeType::TRANSFORM:
				node = std::make_shared < TransformNode>(*this);
				break;
			}

			nodes.push_back(node);
			return nodes.back().get();
		}


		Node* addEntityNode(Entity id)
		{
			std::shared_ptr<Node> node;
			node = std::make_shared <EnitityNode>(*this, id);
			nodes.push_back(node);
			return nodes.back().get();
		}

		ImNodesEditorContext* context = nullptr;
		std::vector<std::shared_ptr<Node>>     nodes;
		std::vector<Link>     links;
		int                   current_id = 0;

		bool initialized = false;

		int m_context_link;
		int m_context_node;


		int findNodePos(NodeEditor editor, int id);

		void DrawNodeEditor(const char* graphName, NodeEditor& editor);

		/*template <typename T>
		void CompareCast(std::shared_ptr<T>& lhsNode, std::shared_ptr<T>& rhsNode);*/

		template <typename T>
		void LinkNodes(const std::shared_ptr<T>& lhsNode,
			const std::shared_ptr<T>& rhsNode, bool IsLinked);
	};


	inline void Eclipse::NodeEditor::DrawNodeEditor(const char* graphName, NodeEditor& editor)
	{
		if (!editor.initialized)
		{
			initialized = true;
			editor.context = ImNodes::EditorContextCreate();
			ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
			ImNodesIO& io = ImNodes::GetIO();
			io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
		}

		ImNodes::EditorContextSet(editor.context);

		ImNodes::BeginNodeEditor();

		bool context_open = false;
		bool create_ent = false;
		if (ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("context_menu");
			context_open = true;
		}

		if (ImGui::BeginPopup("context_menu"))
		{
			if (ImGui::MenuItem("ENT NODE"))
			{
				create_ent = true;
			}
			if (ImGui::MenuItem("Transform NODE"))
			{
				NodeEditor::addNode(NodeEditor::Node::NodeType::TRANSFORM);
			}

			ImGui::EndPopup();
		}

		if (create_ent)
		{
			ImGui::OpenPopup("Entity Creation");
		}

		if (ImGui::BeginPopup("Entity Creation"))
		{
			for (int i = 0; i < engine->editorManager->GetEntityListSize(); ++i)
			{
				Entity ent = engine->editorManager->GetEntityID(i);

				auto entComp = engine->world.GetComponent<EntityComponent>(ent);
				if (ImGui::MenuItem(entComp.Name.c_str()))
				{
					NodeEditor::addEntityNode(ent);
					create_ent = false;
				}
				else
				{
					create_ent = false;
				}
			}
			ImGui::EndPopup();
		}




		for (std::shared_ptr<NodeEditor::Node>& n : editor.nodes)
		{
			n->nodeOnGui();
		}

		for (const const NodeEditor::Link& link : editor.links)
		{
			ImNodes::Link(link.linkId, link.startPoint, link.endPoint);
		}

		ImNodes::EndNodeEditor();

		for (std::shared_ptr<NodeEditor::Node>& n : editor.nodes)
		{
			ImVec2 p = ImNodes::GetNodeEditorSpacePos(n->nodeId);

			if (p.x != n->editorSpacePos.x || p.y != n->editorSpacePos.y)
			{
				n->editorSpacePos = p;
			}
		}

		if (context_open) {
			m_context_link = -1;
			ImNodes::IsLinkHovered(&m_context_link);
			m_context_node = -1;
			ImNodes::IsNodeHovered(&m_context_node);
		}

		{
			Link links;
			if (ImNodes::IsLinkCreated(&links.startPoint, &links.endPoint))
			{
				int from = links.startPoint ^ (5 << 13);
				int to = links.endPoint ^ (5 << 16);
				int pos = findNodePos(editor, from);

				int pos2 = findNodePos(editor, to);

				NodeEditor::Node::NodeType type1 = editor.nodes[pos]->getType();
				NodeEditor::Node::NodeType type2 = editor.nodes[pos2]->getType();
				const bool linked = type1 != type2;

				if (linked)
				{
					/* if (type1 == NodeEditor::Node::NodeType::ENTITY)
					 {
						 if (type2 == NodeEditor::Node::NodeType::TRANSFORM)
						 {

							 std::dynamic_pointer_cast<TransformNode>(editor.nodes[pos2])->ID
								 = std::dynamic_pointer_cast<EnitityNode>(editor.nodes[pos])->ID;

						 }
					 }*/

					LinkNodes<NodeEditor::Node>(editor.nodes[pos], editor.nodes[pos2], true);
				}


				editor.links.push_back(links);
				editor.links.back().linkId = editor.generatedId();
				editor.links.back().startPoint = links.startPoint;
				editor.links.back().endPoint = links.endPoint;

			}
		}

		{
			int link_id;
			if (ImNodes::IsLinkDestroyed(&link_id))
			{
				auto iter = std::find_if(
					editor.links.begin(), editor.links.end(), [link_id](const Link& link) -> bool
					{
						return link.linkId == link_id;
					});

				assert(iter != editor.links.end());

				int from = iter->startPoint ^ (5 << 13);
				int to = iter->endPoint ^ (5 << 16);
				int pos = findNodePos(editor, from);

				int pos2 = findNodePos(editor, to);

				NodeEditor::Node::NodeType type1 = editor.nodes[pos]->getType();
				NodeEditor::Node::NodeType type2 = editor.nodes[pos2]->getType();
				const bool notlinked = type1 != type2;
				if (notlinked)
				{
					LinkNodes<NodeEditor::Node>(editor.nodes[pos], editor.nodes[pos2], false);
				}
				editor.links.erase(iter);
			}
		}


		// {
		//     Link link;
		//     if (ImNodes::IsLinkCreated(&link.startPoint, &link.endPoint))
		//     {
		//         int pos = findNodePos(editor, link.startPoint);
		// 
		//         int pos2 = findNodePos(editor, link.endPoint);
		// 
		//         const bool linked = editor.nodes[pos].nodeType != editor.nodes[pos2].nodeType;
		// 
		//         if (linked)
		//         {
		//             editor.nodes[pos2].value = editor.nodes[pos].value;
		//         }
		//         link.linkId = ++editor.current_id;
		//         editor.links.push_back(link);
		//     }
		// }
		//
		// {
		//     int link_id;
		//     if (ImNodes::IsLinkDestroyed(&link_id))
		//     {
		//         auto iter = std::find_if(
		//             editor.links.begin(), editor.links.end(), [link_id](const Link& link) -> bool {
		//                 return link.linkId == link_id;
		//             });
		//         assert(iter != editor.links.end());
		//         editor.links.erase(iter);
		//     }
		// }
	}

	template <typename T>
	inline void NodeEditor::LinkNodes(const std::shared_ptr<T>& lhsNode,
		const std::shared_ptr<T>& rhsNode, bool IsLinked)
	{
		switch (lhsNode->getType())
		{
		case NodeEditor::Node::NodeType::ENTITY:
		{
			auto outPtr = std::dynamic_pointer_cast<EnitityNode>(lhsNode);

			switch (rhsNode->getType())
			{
			case NodeEditor::Node::NodeType::TRANSFORM:
				if (IsLinked)
					std::dynamic_pointer_cast<TransformNode>(rhsNode)->ID = outPtr->ID;
				else
					std::dynamic_pointer_cast<TransformNode>(rhsNode)->factoryReset();
				break;
			default:
				break;
			}

			break;
		}
		default:
			break;
		}
	}
};