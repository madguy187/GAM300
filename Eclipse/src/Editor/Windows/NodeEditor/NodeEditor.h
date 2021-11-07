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
				bool res = onGUI(nodeId);
				return res;
			}

			int nodeId;
			ImVec2 editorSpacePos = ImVec2(100, 100);
		protected:

			virtual bool onGUI(int nodeId) = 0;

			NodeEditor& res;
			unsigned char inputCounter;
			unsigned char outputCounter;
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

			bool onGUI(int nodeId) override 
			{
				ImNodes::BeginNode(nodeId);
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

			bool onGUI(int nodeId) override 
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(11, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(81, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(81, 0, 0, 255));
				ImNodes::BeginNode(nodeId);
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
			return ++current_id;
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
				return temp;
			}
			unsigned char fromPin() const
			{
				unsigned short temp = (startPoint >> 16) & 0xff;
				return temp;
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
			editor.m_context_link = -1;
			ImNodes::IsLinkHovered(&editor.m_context_link);
			editor.m_context_node = -1;
			ImNodes::IsNodeHovered(&editor.m_context_node);
		}

		{
			Link links;
			if (ImNodes::IsLinkCreated(&links.startPoint, &links.endPoint))
			{
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
				editor.links.erase(iter);
			}
		}

	}

	struct MaterialNode
	{

		struct Node;

		struct	Node
		{
			enum class NodeType
			{
				DEFAULT,
				ADD,
				SUB,
				MUL,
				MULTIPLY_ADD,
				DIV,
				FLOAT,
				ECVEC3,
				ECVEC3COLOUR,
				BASENODE,
				BOOL,
				COUNT
			};

			struct NodeInput {
				Node* node;
				unsigned char output_idx;
			};

			Node(MaterialNode& data)
				: res(data), nodeId(data.generatedId()) {}

			virtual ~Node() {}
			virtual NodeType getType() const = 0;

			NodeInput getInput(unsigned char input_idx)
			{
				for (const Link& link : res.links) 
				{
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
				bool res = onGUI(nodeId);
				return res;
			}

			int getId()
			{
				return nodeId;
			}

			int nodeId;
			ImVec2 editorSpacePos = ImVec2(100, 100);
		protected:

			virtual bool onGUI(int nodeId) = 0;

			MaterialNode& res;
			unsigned char inputCounter;
			unsigned char outputCounter;
		};

		Node* getNodeByID(unsigned short id) const 
		{
			for (auto& node : nodes) {
				if (node->nodeId == id)
				{
					return node.get();
				}
			}
			return nullptr;
		}

		template <NodeEditor::InstructionType operationType>
		struct BinOpNode : public Node
		{
			BinOpNode(MaterialNode& res) : Node(res) {}

			NodeType getType() const override
			{
				switch (operationType) 
				{
					case NodeEditor::InstructionType::DIV: return NodeType::DIV;
					case NodeEditor::InstructionType::MUL: return NodeType::MUL;
					case NodeEditor::InstructionType::ADD: return NodeType::ADD;
					case NodeEditor::InstructionType::SUB: return NodeType::SUB;
					default: EDITOR_LOG_INFO("Case default"); return NodeType::MUL;
				}
			}

			bool onGUI(int nodeId) override
			{
				ImNodes::BeginNode(nodeId);
				ImNodes::BeginNodeTitleBar();
				ImGui::SetNextItemWidth(60);
				switch (operationType)
				{
				case NodeEditor::InstructionType::DIV:
					ECGui::DrawTextWidget<std::string>("Divide" ICON_MDI_SLASH_FORWARD, EMPTY_STRING);
					output = A / B;

					break;
				case NodeEditor::InstructionType::MUL:
					ECGui::DrawTextWidget<std::string>("Multiply" ICON_MDI_CLOSE, EMPTY_STRING);
					output = A * B;
					break;
				case NodeEditor::InstructionType::ADD:
					ECGui::DrawTextWidget<std::string>("Add" ICON_MDI_PLUS, EMPTY_STRING);
					output = A + B;
					break;
				case NodeEditor::InstructionType::SUB:
					ECGui::DrawTextWidget<std::string>("Sub" ICON_MDI_MINUS, EMPTY_STRING);
					output = A - B;
					break;
				default: EDITOR_LOG_INFO("Should not come in here one"); break;
				}
				ImNodes::EndNodeTitleBar();

				beginOutput();
				endOutput();

				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(0, 25, 0, 255));
				beginInput();
				// check for first pin if its linked
				if (getInput(0).node)
				{
					//might case problem down the line 
					if (getInput(0).node->getType() == Node::NodeType::FLOAT)
					{
						auto downcastedPtr = dynamic_cast<FloatNode*>(getInput(0).node);
						A = downcastedPtr->inputFloat;
					}
					else
					{
						ECGui::OpenPopup("Link Error");
					}
					if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ECGui::DrawTextWidget<const char*>("Operator Nodes only accept Floats", EMPTY_STRING);
						ECGui::InsertHorizontalLineSeperator();
						ECGui::NextColumn();
						if (ECGui::ButtonBool("OK", ImVec2(320, 0)))
						{
							res.links.pop_back();
							ECGui::CloseCurrentPopup();
						}
						ECGui::EndPopup();
					}
					ImGui::SetNextItemWidth(60);
					ECGui::DrawTextWidget<std::string>("A", EMPTY_STRING);
				}
				else 
				{
					ImGui::SetNextItemWidth(60);
					ECGui::DrawTextWidget<std::string>("A", EMPTY_STRING);
				}
				endInput();
				ImNodes::PopColorStyle();

				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(0, 25, 0, 255));
				beginInput();
				// check for first pin if its linked
				if (getInput(1).node)
				{
					if (getInput(1).node->getType() == Node::NodeType::FLOAT)
					{
						auto downcastedPtr = dynamic_cast<FloatNode*>(getInput(1).node);
						B = downcastedPtr->inputFloat;
					}
					else
					{

						ECGui::OpenPopup("Link Error");
					}
					if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ECGui::DrawTextWidget<const char*>("Operator Nodes only accept Floats", EMPTY_STRING);
						ECGui::NextColumn();
						if (ECGui::ButtonBool("OK", ImVec2(140, 0)))
						{
							res.links.pop_back();
							ECGui::CloseCurrentPopup();
						}
						ECGui::EndPopup();
					}
					ImGui::SetNextItemWidth(60);
					ECGui::DrawTextWidget<std::string>("B", EMPTY_STRING);
				}
				else
				{
					ImGui::SetNextItemWidth(60);
					ECGui::DrawTextWidget<std::string>("B", EMPTY_STRING);
				}
				endInput();
				ImNodes::PopColorStyle();

				ImNodes::EndNode();
				return false;
			}

			float A = 0.f;
			float B = 0.f;
			float output = 0.f;
			//testing for future implimentation
			ECVec3 vec3A = { 0.0f,0.0f,0.0f };
			ECVec3 vec3B = { 0.0f,0.0f,0.0f };
			ECVec3 vec3Output = { 0.0f,0.0f,0.0f };
		};

		struct FloatNode : public Node
		{
			FloatNode(MaterialNode& res) : Node(res) {}

			NodeType getType() const override
			{
				return NodeType::FLOAT;
			}

			bool onGUI(int nodeId) override 
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(0, 25, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(0, 45, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(0, 45, 0, 255));
				ImNodes::BeginNode(nodeId);
				ImNodes::BeginNodeTitleBar();
				ECGui::DrawTextWidget<std::string>("Float", EMPTY_STRING);
				ImNodes::EndNodeTitleBar();
				beginOutput();
				ImGui::SetNextItemWidth(60);
				ImGui::DragFloat("", &inputFloat);
				endOutput();
				ImNodes::EndNode();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				return false;
			}
			float inputFloat = 0.f;
		};

		template <NodeEditor::VEC3TYPE operationType>
		struct Vec3Nodes : public Node
		{
			Vec3Nodes(MaterialNode& res): Node(res){}

			NodeType getType() const override
			{
				switch (operationType)
				{
				case NodeEditor::VEC3TYPE::ECVEC3: return NodeType::ECVEC3;
				case NodeEditor::VEC3TYPE::ECVEC3COLOUR: return NodeType::ECVEC3COLOUR;
				default: EDITOR_LOG_INFO("Case default"); return NodeType::ECVEC3;
				}
			}

			bool onGUI(int nodeId) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(100, 42, 42, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(120, 42, 42, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(120, 42, 42, 255));
				ImNodes::BeginNode(nodeId);
				ImNodes::BeginNodeTitleBar();
				switch (operationType)
				{
				case NodeEditor::VEC3TYPE::ECVEC3:
					ECGui::DrawTextWidget<std::string>("Vec3", EMPTY_STRING);
					break;
				case NodeEditor::VEC3TYPE::ECVEC3COLOUR:
					ECGui::DrawTextWidget<std::string>("Vec3_Colour" , EMPTY_STRING);
					break;
				default: EDITOR_LOG_INFO("Should not come in here one"); break;
				}
				ImNodes::EndNodeTitleBar();
				beginOutput();
				switch (operationType)
				{
				case NodeEditor::VEC3TYPE::ECVEC3:
					ImGui::SetNextItemWidth(150);
					ECGui::DrawSliderFloat3Widget("Vec3", &inputVec3, true, -100.f, 100.f);
					break;
				case NodeEditor::VEC3TYPE::ECVEC3COLOUR:
					ImGui::SetNextItemWidth(150);
					ECGui::ColorPicker3("Vec3", (float*)&inputVec3, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
					break;
				default: EDITOR_LOG_INFO("Should not come in here one"); break;
				}
				endOutput();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::EndNode();
				return false;
			}
			ECVec3 inputVec3 = 0.f;
		};

		struct BasedMaterialNode : public Node
		{
			BasedMaterialNode(MaterialNode& res) : Node(res) {}
			NodeType getType() const override
			{
				return NodeType::BASENODE;
			}

			bool onGUI(int nodeId) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_NodeBackground, IM_COL32(0, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_NodeBackgroundHovered, IM_COL32(0, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_NodeBackgroundSelected, IM_COL32(0, 0, 0, 255));
				ImNodes::BeginNode(nodeId);
				ImNodes::BeginNodeTitleBar();
				ImNodes::EndNodeTitleBar();
				
				// Albedo Constant
				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(100, 42, 42, 255));
				beginInput();
				// check for first pin if its linked
				if (getInput(0).node)
				{
					//might case problem down the line 
					if (getInput(0).node->getType() == Node::NodeType::ECVEC3)
					{
						auto downcastedPtr = dynamic_cast<Vec3Nodes< NodeEditor::VEC3TYPE::ECVEC3>*>(getInput(0).node);
						//A = downcastedPtr->inputFloat;
						engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant = downcastedPtr->inputVec3;
					}
					else
						if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR)
						{
							auto downcastedPtr = dynamic_cast<Vec3Nodes< NodeEditor::VEC3TYPE::ECVEC3COLOUR>*>(getInput(0).node);
							//A = downcastedPtr->inputFloat;
							engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant = downcastedPtr->inputVec3;
						}
						else
						{
							ECGui::OpenPopup("Link Error");
						}

					if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ECGui::DrawTextWidget<const char*>("Worng Node linkedm link action canceled.", EMPTY_STRING);
						ECGui::InsertHorizontalLineSeperator();
						if (ECGui::ButtonBool("OK", ImVec2(320, 0)))
						{
							res.links.pop_back();
							ECGui::CloseCurrentPopup();
						}
						ECGui::EndPopup();
					}

					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Albedo Constant", EMPTY_STRING);
				}
				else
				{
					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Albedo Constant", EMPTY_STRING);
				}
				endInput();
				ImNodes::PopColorStyle();

				// metallic constant
				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(0, 60, 0, 255));
				beginInput();
				if (getInput(1).node)
				{
					//might case problem down the line 
					if (getInput(1).node->getType() == Node::NodeType::FLOAT)
					{
						auto downcastedPtr = dynamic_cast<FloatNode*>(getInput(1).node);
						//A = downcastedPtr->inputFloat;
						engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.MetallicConstant = downcastedPtr->inputFloat;
					}
					else
					{
						ECGui::OpenPopup("Link Error");
					}

					if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ECGui::DrawTextWidget<const char*>("Worng Node linkedm link action canceled.", EMPTY_STRING);
						ECGui::InsertHorizontalLineSeperator();
						if (ECGui::ButtonBool("OK", ImVec2(320, 0)))
						{
							res.links.pop_back();
							ECGui::CloseCurrentPopup();
						}
						ECGui::EndPopup();
					}

					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Metallic Constant", EMPTY_STRING);
				}
				else
				{
					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Metallic Constant", EMPTY_STRING);
				}
				endInput();
				ImNodes::PopColorStyle();

				// Roughness constant
				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(0, 60, 0, 255));
				beginInput();
				if (getInput(2).node)
				{
					//might case problem down the line 
					if (getInput(2).node->getType() == Node::NodeType::FLOAT)
					{
						auto downcastedPtr = dynamic_cast<FloatNode*>(getInput(2).node);
						//A = downcastedPtr->inputFloat;
						engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.RoughnessConstant = downcastedPtr->inputFloat;
					}
					else
					{
						ECGui::OpenPopup("Link Error");
					}

					if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ECGui::DrawTextWidget<const char*>("Worng Node linkedm link action canceled.", EMPTY_STRING);
						ECGui::InsertHorizontalLineSeperator();
						if (ECGui::ButtonBool("OK", ImVec2(320, 0)))
						{
							res.links.pop_back();
							ECGui::CloseCurrentPopup();
						}
						ECGui::EndPopup();
					}

					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Roughness Constant", EMPTY_STRING);
				}
				else
				{
					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Roughness Constant", EMPTY_STRING);
				}
				endInput();
				ImNodes::PopColorStyle();
				// AO constant
				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(0, 60, 0, 255));
				beginInput();
				if (getInput(3).node)
				{
					//might case problem down the line 
					if (getInput(3).node->getType() == Node::NodeType::FLOAT)
					{
						auto downcastedPtr = dynamic_cast<FloatNode*>(getInput(3).node);
						//A = downcastedPtr->inputFloat;
						engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AoConstant = downcastedPtr->inputFloat;
					}
					else
					{
						ECGui::OpenPopup("Link Error");
					}

					if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ECGui::DrawTextWidget<const char*>("Worng Node linkedm link action canceled.", EMPTY_STRING);
						ECGui::InsertHorizontalLineSeperator();
						if (ECGui::ButtonBool("OK", ImVec2(320, 0)))
						{
							res.links.pop_back();
							ECGui::CloseCurrentPopup();
						}
						ECGui::EndPopup();
					}

					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Ao Constant", EMPTY_STRING);
				}
				else
				{
					ImGui::SetNextItemWidth(50);
					ECGui::DrawTextWidget<std::string>("Ao Constant", EMPTY_STRING);
				}
				endInput();
				ImNodes::PopColorStyle();
				//Has Texture
				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(60, 0, 0, 255));
				beginInput();
				if (getInput(4).node)
				{
					auto downcastedPtr = dynamic_cast<BoolNode*>(getInput(4).node);
					//A = downcastedPtr->inputFloat;
					hasTexture = downcastedPtr->inputBool;
					ImGui::SetNextItemWidth(50);
					ECGui::CheckBoxBool("",&hasTexture,false);
				}
				else
				{
					ImGui::SetNextItemWidth(50);
					ECGui::CheckBoxBool("Bool", &hasTexture, false);
				}
				endInput();
				ImNodes::PopColorStyle();
				//if(hasTexture)

				ImNodes::EndNode();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				return false;
			}

			bool hasTexture = false;
		};

		struct BoolNode : public Node
		{
			BoolNode(MaterialNode& res) : Node(res) {}
			NodeType getType() const override
			{
				return NodeType::BOOL;
			}

			bool onGUI(int nodeId) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(60, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(70, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(70, 0, 0, 255));
				ImNodes::BeginNode(nodeId);
				ImNodes::BeginNodeTitleBar();
				ECGui::DrawTextWidget<std::string>("Bool", EMPTY_STRING);
				ImNodes::EndNodeTitleBar();
				beginOutput();
				ImGui::SetNextItemWidth(60);
				ECGui::CheckBoxBool("Input Bool",&inputBool,true);
				endOutput();
				ImNodes::EndNode();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				return false;
			}
			bool inputBool = false;
		};

		unsigned short generatedId()
		{
			return ++current_id;
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
				return temp;
			}
			unsigned char fromPin() const
			{
				unsigned short temp = (startPoint >> 16) & 0xff;
				return temp;
			}
		};

		Node* addNode(Node::NodeType type)
		{
			std::shared_ptr<Node> node;

			switch (type)
			{
					//MULTIPLY_ADD,
				case Node::NodeType::DIV:
					node = std::make_shared<BinOpNode<NodeEditor::InstructionType::DIV>>(*this); 
					break;
				case Node::NodeType::MUL:
					node = std::make_shared<BinOpNode<NodeEditor::InstructionType::MUL>>(*this);
					break;
				case Node::NodeType::ADD:
					node = std::make_shared<BinOpNode<NodeEditor::InstructionType::ADD>>(*this);
					break;
				case Node::NodeType::SUB:
					node = std::make_shared<BinOpNode<NodeEditor::InstructionType::SUB>>(*this);
					break;
				case Node::NodeType::FLOAT:
					node = std::make_shared <FloatNode>(*this);
					break;
				case Node::NodeType::ECVEC3:
					node = std::make_shared <Vec3Nodes<NodeEditor::VEC3TYPE::ECVEC3>>(*this);
					break;
				case Node::NodeType::ECVEC3COLOUR:
					node = std::make_shared <Vec3Nodes<NodeEditor::VEC3TYPE::ECVEC3COLOUR>>(*this);
					break;
				case Node::NodeType::BOOL:
					node = std::make_shared <BoolNode>(*this);
					break;
			}
			nodes.push_back(node);
			return nodes.back().get();
		}

		ImNodesEditorContext* context = nullptr;
		std::vector<std::shared_ptr<Node>>     nodes;
		std::vector<Link>     links;
		int                   current_id = 0;

		bool initialized = false;
		bool BaseNodeExist = false;
		int m_context_link = 0;
		int m_context_node = 0;


		int findMaterialNodePos(MaterialNode editor, int id);

		void DrawMaterialNodeEditor(const char* graphName, MaterialNode& editor);

		void Reset();

		void CreateBaseMaterialNode();
	};

	inline int MaterialNode::findMaterialNodePos(MaterialNode editor, int id)
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

	inline void MaterialNode::DrawMaterialNodeEditor(const char* graphName, MaterialNode& editor)
	{

		ImNodes::EditorContextSet(editor.context);

		ImNodes::BeginNodeEditor();
		bool context_open = false;

		if (!BaseNodeExist)
		{
			CreateBaseMaterialNode();
			BaseNodeExist = true;
		}
		
		if (ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("context_menu");
			context_open = true;
		}

		if (ImGui::BeginPopup("context_menu"))
		{
			if (ImGui::MenuItem("Divide"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::DIV);
			}
			if (ImGui::MenuItem("Multiply"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::MUL);
			}
			if (ImGui::MenuItem("Plus"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::ADD);
			}
			if (ImGui::MenuItem("Minus"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::SUB);
			}
			if (ImGui::MenuItem("Float"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::FLOAT);
			}
			if (ImGui::MenuItem("Vec3"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::ECVEC3);
			}
			if (ImGui::MenuItem("Vec3Colour"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::ECVEC3COLOUR);
			}
			if (ImGui::MenuItem("Bool"))
			{
				MaterialNode::addNode(MaterialNode::Node::NodeType::BOOL);
			}
			ImGui::EndPopup();
		}		

		for (std::shared_ptr<MaterialNode::Node>& n : editor.nodes)
		{
			n->nodeOnGui();
		}

		for (const const MaterialNode::Link& link : editor.links)
		{
			ImNodes::Link(link.linkId, link.startPoint, link.endPoint);
		}

		ImNodes::EndNodeEditor();

		for (std::shared_ptr<MaterialNode::Node>& n : editor.nodes)
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
			Link links;
			if (ImNodes::IsLinkCreated(&links.startPoint, &links.endPoint))
			{
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

				editor.links.erase(iter);
			}
		}

	}

	inline void MaterialNode::Reset()
	{
		this->current_id = 0;
		this->links.clear();
		this->m_context_link = 0;
		this->m_context_node = 0;
		this->nodes.clear();
		BaseNodeExist = false;
	}

	inline void MaterialNode::CreateBaseMaterialNode()
	{
		std::shared_ptr<Node> node = std::make_shared <BasedMaterialNode>(*this);
		nodes.push_back(node);
	}

};