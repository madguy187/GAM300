#pragma once
#include"pch.h"
#include "ImNodes/imnodes.h"
#include <imnodes.h>

#define DEFAULTVAL 99999
namespace Eclipse
{
	static constexpr int OUTPUT_FLAG = 1 << 31;

	struct NodeEditor
	{

		struct Node;

		enum class InstructionType : unsigned char {
			ADD,
			SUB,
			MUL,
			MULTIPLY_ADD,
			DIV
		};

		enum class VEC3TYPE : unsigned char {
			ECVEC3,
			ECVEC3COLOUR,
		};


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

			struct NodeInput {
				Node* node;
				unsigned char output_idx;
			};

			Node(NodeEditor& data)
				: res(data), nodeId(data.generatedId()) {}

			virtual ~Node() {}
			virtual NodeType getType() const = 0;

			NodeInput getInput(unsigned char input_idx)
			{
				for (const Link& link : res.links) {
					if (link.toNode() != nodeId) continue;
					if (link.toPin() != input_idx) continue;

					NodeInput res_;
					res_.output_idx = link.fromPin();
					res_.node = res.getNodeByID(link.fromNode());
					return res_;
				}

				return {};
			}

			void beginInput(ImNodesPinShape pinShape = 1)
			{
				ImNodes::BeginInputAttribute(nodeId | (unsigned int(inputCounter) << 16), pinShape);
				++inputCounter;
			}

			static void endInput()
			{
				ImNodes::EndInputAttribute();
			}

			void beginOutput(ImNodesPinShape pinShape = 1)
			{
				ImNodes::BeginOutputAttribute(nodeId | (unsigned int(outputCounter) << 16) | OUTPUT_FLAG, pinShape);
				++outputCounter;
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
				bool nd = onGUI(nodeId);
				return nd;
			}

			int nodeId;
			ImVec2 editorSpacePos = ImVec2(100, 100);
		protected:

			virtual bool onGUI(int nodeId) = 0;

			NodeEditor& res;
			unsigned char inputCounter = 0;
			unsigned char outputCounter = 0;
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

		struct EnitityNode : public Node
		{
			EnitityNode(NodeEditor& res, Entity id)
				: Node(res), ID(id) {}

			~EnitityNode() {};

			NodeType getType() const override { return NodeType::ENTITY; }

			bool onGUI(int nodeId_) override 
			{
				ImNodes::BeginNode(nodeId_);
				getName();
				beginOutput();
				ImGui::SetNextItemWidth(60);
				ECGui::DrawTextWidget<std::string>("Edit Name", name);
				ECGui::DrawTextWidget<std::string>("Entity ID", std::to_string(ID));
				endOutput();
				ImNodes::EndNode();
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

			bool onGUI(int nodeId_) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(11, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(81, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(81, 0, 0, 255));
				ImNodes::BeginNode(nodeId_);
				ImNodes::BeginNodeTitleBar();
				ImGui::TextUnformatted("TransFormNode");
				ImNodes::EndNodeTitleBar();

				beginInput(ImNodesPinShape_Quad);
				ImGui::SetNextItemWidth(160);
				if (getInput(0).node)
				{
					//might case problem down the line 
					auto downcastedPtr = dynamic_cast<EnitityNode*>(getInput(0).node);
					ID = downcastedPtr->ID;
				}
				else
				{
					ID = DEFAULTVAL;
				}
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
				ImNodes::EndNode(); 
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				return false;
			}
			Entity ID = DEFAULTVAL;
		};

		unsigned short generatedId()
		{
			return static_cast<unsigned short>(++current_id);
		}

		struct Link
		{
			int linkId;
			int startPoint, endPoint;

			unsigned short toNode() const
			{
				unsigned short temp = endPoint & 0xffFF;
				return temp;
			}
			unsigned short fromNode() const
			{
				unsigned short temp = startPoint & 0xffFF;
				return temp;
			}

			unsigned char toPin() const
			{
				unsigned short temp = (endPoint >> 16) & 0xff;
				return static_cast<unsigned char>(temp);
			}
			unsigned char fromPin() const
			{
				unsigned short temp = (startPoint >> 16) & 0xff;
				return static_cast<unsigned char>(temp);
			}
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

		int m_context_link = 0;
		int m_context_node = 0;



		int findNodePos(NodeEditor editor, int id);

		void DrawNodeEditor(const char* graphName, NodeEditor& editor);

	};


	inline int NodeEditor::findNodePos(NodeEditor editor, int id)
	{
		int pos = 0;
		for (auto& it : editor.nodes)
		{
			if (it->nodeId == id)
			{
				return pos;
			}
			pos++;
		}

		return -1;
	}

	inline void Eclipse::NodeEditor::DrawNodeEditor(const char* graphName, NodeEditor& editor)
	{
		(void)graphName;
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

		for (const Link& link_ : editor.links)
		{
			ImNodes::Link(link_.linkId, link_.startPoint, link_.endPoint);
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
			editor.m_context_link = -1;
			ImNodes::IsLinkHovered(&editor.m_context_link);
			editor.m_context_node = -1;
			ImNodes::IsNodeHovered(&editor.m_context_node);
		}

		{
			Link lnk;
			if (ImNodes::IsLinkCreated(&lnk.startPoint, &lnk.endPoint))
			{
				editor.links.push_back(lnk);
				editor.links.back().linkId = editor.generatedId();
				editor.links.back().startPoint = lnk.startPoint;
				editor.links.back().endPoint = lnk.endPoint;

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
				editor.links.erase(iter);
			}
		}

	}

};