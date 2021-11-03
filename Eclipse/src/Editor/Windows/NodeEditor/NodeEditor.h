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
				ENTITY,
				TRANSFORM,
				COUNT
			};


			Node(NodeEditor& data)
			: res(data), nodeId(data.generatedId()){}

			virtual ~Node() {}
			virtual NodeType getType() const = 0;

			//NodeInput getInput(unsigned char input_idx)
			//{
			//	for (const Link& link : res.links)
			//	{
			//		if (link.toNode() != nodeId) continue;
			//		//if (link.toPin() != input_idx) continue;

			//		NodeInput ressource;
			//		ressource.outputId = link.fromPin();
			//		ressource.node = res.getNodeByID(link.fromNode());
			//		return ressource;
			//	}

			//	return {};
			//}

			void beginInput(ImNodesPinShape pinShape = 1)
			{
				int bitshift = (5 << 16);
				int temp = nodeId ^ (5 << 16);
				ImNodes::BeginInputAttribute(temp, pinShape);
				//++inputCounter;
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
				//++outputCounter;
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
			EnitityNode(NodeEditor& res,Entity id)
			: Node(res),ID(id){}

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
			TransformNode(NodeEditor& res) : Node(res){}

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

			unsigned short toNode() const { return endPoint & 0xffFF; }
			unsigned short fromNode() const { return startPoint & 0xffFF; }
			
			unsigned char toPin() const { return (endPoint >> 16) & 0xff; }
			unsigned char fromPin() const { return (startPoint >> 16) & 0xff; }
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
			node = std::make_shared <EnitityNode>(*this,id);
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
	};


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


}
