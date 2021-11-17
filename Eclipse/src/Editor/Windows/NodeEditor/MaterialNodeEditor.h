#pragma once
#include"pch.h"
#include "ImNodes/imnodes.h"
#include <imnodes.h>
 
#define DEFAULTVAL 99999
namespace Eclipse
{
	static constexpr int MAT_OUTPUT_Flag = 1 << 31;
	//TODO FOR JIANHERNG
	struct MaterialNode
	{

		struct Node;

		struct	Node
		{
			enum class InstructionType : unsigned char {
				ADD,
				SUB,
				MUL,
				DIV
			};

			enum class VEC3TYPE : unsigned char {
				ECVEC3,
				ECVEC3COLOUR,
			};


			enum class NodeType
			{
				DEFAULT,
				ADD,
				SUB,
				MUL,
				DIV,
				FLOAT,
				ECVEC3,
				ECVEC3COLOUR,
				BASENODE,
				BOOL,
				TEXTURE,
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
				ImNodes::BeginOutputAttribute(nodeId | (unsigned int(outputCounter) << 16) | MAT_OUTPUT_Flag, pinShape);
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
				bool nods = onGUI(nodeId);
				return nods;
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
			unsigned char inputCounter = 0;
			unsigned char outputCounter = 0;
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

		template <MaterialNode::Node::InstructionType operationType>
		struct BinOpNode : public Node
		{
			BinOpNode(MaterialNode& res) : Node(res) {}

			NodeType getType() const override
			{
				switch (operationType)
				{
				case MaterialNode::Node::InstructionType::DIV: return NodeType::DIV;
				case MaterialNode::Node::InstructionType::MUL: return NodeType::MUL;
				case MaterialNode::Node::InstructionType::ADD: return NodeType::ADD;
				case MaterialNode::Node::InstructionType::SUB: return NodeType::SUB;
				default: EDITOR_LOG_INFO("Case default"); return NodeType::MUL;
				}
			}

			bool onGUI(int nodeId_) override
			{
				ImNodes::BeginNode(nodeId_);
				ImNodes::BeginNodeTitleBar();
				ImGui::SetNextItemWidth(60);
				switch (operationType)
				{
					case MaterialNode::Node::InstructionType::DIV:
						ECGui::DrawTextWidget<std::string>("Divide" " A" ICON_MDI_SLASH_FORWARD "B = C", EMPTY_STRING);
						if (getInput(0).node)
						{
							if (getInput(1).node)
							{
								if (getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = true;
									vec3Mode = false;
									output = A / B;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x / B , vec3A.y / B, vec3A.z / B };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x / B , vec3A.y / B, vec3A.z / B };
								}
								if ((getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR) ||
									(getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3))
								{
									floatMode = false;
									vec3Mode = false;
									LinkError = true;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x / vec3B.x ,  vec3A.y / vec3B.y,  vec3A.z / vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x / vec3B.x ,  vec3A.y / vec3B.y,  vec3A.z / vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x / vec3B.x ,  vec3A.y / vec3B.y,  vec3A.z / vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x / vec3B.x ,  vec3A.y / vec3B.y,  vec3A.z / vec3B.z };
								}
							}
							else
							{
								//if only one is linked 
								vec3Output = { 0.0f,0.0f,0.0f };
								output = 0.0f;
							}
						}
						else
						{	
							//if only one is linked 
							vec3Output = { 0.0f,0.0f,0.0f };
							output = 0.0f;
						}
						break;
					case MaterialNode::Node::InstructionType::MUL:
						ECGui::DrawTextWidget<std::string>("Multiply" " A" ICON_MDI_CLOSE "B = C", EMPTY_STRING);
						if (getInput(0).node)
						{
							if (getInput(1).node)
							{
								if (getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = true;
									vec3Mode = false;
									output = A * B;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x * B , vec3A.y * B, vec3A.z * B };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x * B , vec3A.y * B, vec3A.z * B };
								}
								if ((getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR) ||
									(getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3))
								{
									floatMode = false;
									vec3Mode = false;
									LinkError = true;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x * vec3B.x ,  vec3A.y * vec3B.y,  vec3A.z * vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x * vec3B.x ,  vec3A.y * vec3B.y,  vec3A.z * vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x * vec3B.x ,  vec3A.y * vec3B.y,  vec3A.z * vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x * vec3B.x ,  vec3A.y * vec3B.y,  vec3A.z * vec3B.z };
								}
							}
							else
							{
								//if only one is linked 
								vec3Output = { 0.0f,0.0f,0.0f };
								output = 0.0f;
							}
						}
						else
						{
							//if only one is linked 
							vec3Output = { 0.0f,0.0f,0.0f };
							output = 0.0f;
						}
						break;
					case MaterialNode::Node::InstructionType::ADD:
						ECGui::DrawTextWidget<std::string>("Add" " A" ICON_MDI_PLUS "B = C", EMPTY_STRING);
						if (getInput(0).node)
						{
							if (getInput(1).node)
							{
								if (getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = true;
									vec3Mode = false;
									output = A + B;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x + B , vec3A.y + B, vec3A.z + B };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x + B , vec3A.y + B, vec3A.z + B };
								}
								if ((getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR) ||
									(getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3))
								{
									floatMode = false;
									vec3Mode = false;
									LinkError = true;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x + vec3B.x ,  vec3A.y + vec3B.y,  vec3A.z + vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x + vec3B.x ,  vec3A.y + vec3B.y,  vec3A.z + vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x + vec3B.x ,  vec3A.y + vec3B.y,  vec3A.z + vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x + vec3B.x ,  vec3A.y + vec3B.y,  vec3A.z + vec3B.z };
								}
							}
							else
							{
								//if only one is linked 
								vec3Output = { 0.0f,0.0f,0.0f };
								output = 0.0f;
							}
						}
						else
						{
							//if only one is linked 
							vec3Output = { 0.0f,0.0f,0.0f };
							output = 0.0f;
						}
						break;
					case MaterialNode::Node::InstructionType::SUB:
						ECGui::DrawTextWidget<std::string>("Sub" " A" ICON_MDI_MINUS "B = C", EMPTY_STRING);
						if (getInput(0).node)
						{
							if (getInput(1).node)
							{
								if (getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = true;
									vec3Mode = false;
									output = A - B;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x - B , vec3A.y - B, vec3A.z - B };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::FLOAT)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x - B , vec3A.y - B, vec3A.z - B };
								}
								if ((getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR) ||
									(getInput(0).node->getType() == Node::NodeType::FLOAT && getInput(1).node->getType() == Node::NodeType::ECVEC3))
								{
									floatMode = false;
									vec3Mode = false;
									LinkError = true;
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x - vec3B.x ,  vec3A.y - vec3B.y,  vec3A.z - vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3 && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x - vec3B.x ,  vec3A.y - vec3B.y,  vec3A.z - vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x - vec3B.x ,  vec3A.y - vec3B.y,  vec3A.z - vec3B.z };
								}
								if (getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR && getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
								{
									floatMode = false;
									vec3Mode = true;
									vec3Output = { vec3A.x - vec3B.x ,  vec3A.y - vec3B.y,  vec3A.z - vec3B.z };
								}
							}
							else
							{
								//if only one is linked 
								vec3Output = { 0.0f,0.0f,0.0f };
								output = 0.0f;
							}
						}
						else
						{
							//if only one is linked 
							vec3Output = { 0.0f,0.0f,0.0f };
							output = 0.0f;
						}
						break;
					default: 
						EDITOR_LOG_INFO("Should not come in here one");
						break;
				}
				ImNodes::EndNodeTitleBar();

				beginOutput();
				if (getInput(0).node && getInput(1).node)
				{
					if (getInput(0).node->getType() == Node::NodeType::ECVEC3 || getInput(1).node->getType() == Node::NodeType::ECVEC3
						|| getInput(0).node->getType() == Node::NodeType::ECVEC3COLOUR || getInput(1).node->getType() == Node::NodeType::ECVEC3COLOUR)
					{
						ImGui::SetNextItemWidth(120);
						ECGui::DrawSliderFloat3Widget("C", &vec3Output, false, 0.0f, 0.0f);
					}
					else
					{
						ImGui::SetNextItemWidth(60);
						ECGui::DrawTextWidget<std::string>("C", std::to_string(output));
					}
				}
				endOutput();

				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(0, 25, 0, 255));
				beginInput();
				// check for first pin if its linked
				if (getInput(0).node)
				{
					switch (getInput(0).node->getType())
					{
						case Node::NodeType::FLOAT:
							A = (dynamic_cast<FloatNode*>(getInput(0).node))->inputFloat;
							break;
						case Node::NodeType::ECVEC3:
							vec3A = (dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3>*>(getInput(0).node))->inputVec3;
							break;
						case Node::NodeType::ECVEC3COLOUR:
							vec3A = (dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR>*>(getInput(0).node))->inputVec3;
							break;
						case Node::NodeType::DIV:
							A = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(0).node))->output;
							break;
						case Node::NodeType::MUL:
							A = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(0).node))->output;
							break;
						case Node::NodeType::ADD:
							A = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(0).node))->output;
							break;
						case Node::NodeType::SUB:
							A = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(0).node))->output;
							break;
						default:
							LinkError = true;
							break;
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
					switch (getInput(1).node->getType())
					{
						case Node::NodeType::FLOAT:
							B = (dynamic_cast<FloatNode*>(getInput(1).node))->inputFloat;
							break;
						case Node::NodeType::ECVEC3:
							vec3B = (dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3>*>(getInput(1).node))->inputVec3;
							break;
						case Node::NodeType::ECVEC3COLOUR:
							vec3B = (dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR>*>(getInput(1).node))->inputVec3;
							break;
						case Node::NodeType::DIV:
							B = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(1).node))->output;
							break;
						case Node::NodeType::MUL:
							B = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(1).node))->output;
							break;
						case Node::NodeType::ADD:
							B = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(1).node))->output;
							break;
						case Node::NodeType::SUB:
							B = (dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(1).node))->output;
							break;
						default:
							LinkError = true;
							break;
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

				if (LinkError)
				{
					ECGui::OpenPopup("Link Error");
				}

				if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ECGui::DrawTextWidget<const char*>("Link Error, Nodes Linked Wrongly", EMPTY_STRING);
					ECGui::NextColumn();
					if (ECGui::ButtonBool("OK", ImVec2(320, 0)))
					{
						res.links.pop_back();
						LinkError = false;
						ECGui::CloseCurrentPopup();
					}
					ECGui::EndPopup();
				}
				ImNodes::EndNode();
				return false;
			}

			float A = 0.f;
			float B = 0.f;
			float output = 0.f;
			bool LinkError = false;
			bool floatMode = false;
			bool vec3Mode = false;
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

			bool onGUI(int nodeId_) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(0, 25, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(0, 45, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(0, 45, 0, 255));
				ImNodes::BeginNode(nodeId_);
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

		template <MaterialNode::Node::VEC3TYPE operationType>
		struct Vec3Nodes : public Node
		{
			Vec3Nodes(MaterialNode& res) : Node(res) {}

			NodeType getType() const override
			{
				switch (operationType)
				{
				case MaterialNode::Node::VEC3TYPE::ECVEC3: return NodeType::ECVEC3;
				case MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR: return NodeType::ECVEC3COLOUR;
				default: EDITOR_LOG_INFO("Case default"); return NodeType::ECVEC3;
				}
			}

			bool onGUI(int nodeId_) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(100, 42, 42, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(120, 42, 42, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(120, 42, 42, 255));
				ImNodes::BeginNode(nodeId_);
				ImNodes::BeginNodeTitleBar();
				switch (operationType)
				{
				case MaterialNode::Node::VEC3TYPE::ECVEC3:
					ECGui::DrawTextWidget<std::string>("Vec3", EMPTY_STRING);
					break;
				case MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR:
					ECGui::DrawTextWidget<std::string>("Vec3_Colour", EMPTY_STRING);
					break;
				default: EDITOR_LOG_INFO("Should not come in here one"); break;
				}
				ImNodes::EndNodeTitleBar();
				beginOutput();
				switch (operationType)
				{
				case MaterialNode::Node::VEC3TYPE::ECVEC3:
					ImGui::SetNextItemWidth(150);
					ECGui::DrawSliderFloat3Widget("Vec3", &inputVec3, true, -100.f, 100.f);
					break;
				case MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR:
					ImGui::SetNextItemWidth(100);
					ECGui::ColorPicker3("Vec3_Colour", (float*)&inputVec3, ImGuiColorEditFlags_DisplayRGB);
					inputVec3.x = (Saturate(((float*)(&inputVec3))[0]) * 255.0f);
					inputVec3.y = (Saturate(((float*)(&inputVec3))[1]) * 255.0f);
					inputVec3.z = (Saturate(((float*)(&inputVec3))[2]) * 255.0f);
					inputVec3.w = 1.0f;
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
			ECVec3 inputVec3 = { 0.0f,0.0f,0.0f };
		};

		struct BasedMaterialNode : public Node
		{
			BasedMaterialNode(MaterialNode& res) : Node(res) {}
			NodeType getType() const override
			{
				return NodeType::BASENODE;
			}

			bool onGUI(int nodeId_) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_NodeBackground, IM_COL32(0, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_NodeBackgroundHovered, IM_COL32(0, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_NodeBackgroundSelected, IM_COL32(0, 0, 0, 255));
				ImNodes::BeginNode(nodeId_);
				ImNodes::BeginNodeTitleBar();
				ImNodes::EndNodeTitleBar();

				//is Emissive
				ImNodes::PushColorStyle(
					ImNodesCol_Pin, IM_COL32(60, 0, 0, 255));
				beginInput();
				if (getInput(0).node)
				{
					if (getInput(0).node->getType() == Node::NodeType::BOOL)
					{
						auto downcastedPtr = dynamic_cast<BoolNode*>(getInput(0).node);
						//A = downcastedPtr->inputFloat;
						isEmissive = downcastedPtr->inputBool;
						engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveMaterial = isEmissive;
						ImGui::SetNextItemWidth(50);
					}
					else
					{
						LinkError = true;
					}
					ECGui::CheckBoxBool("Is Emissive", &isEmissive, false);
					engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveMaterial = isEmissive;
				}
				else
				{
					ImGui::SetNextItemWidth(50);
					ECGui::CheckBoxBool("Is Emissive", &isEmissive, false);
					engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveMaterial = isEmissive;
				}
				endInput();
				ImNodes::PopColorStyle();

				if (isEmissive)
				{
					// Albedo Constant
					ImNodes::PushColorStyle(
						ImNodesCol_Pin, IM_COL32(100, 42, 42, 255));
					beginInput();
					// check for first pin if its linked
					if (getInput(1).node)
					{
						//might case problem down the line 
						switch (getInput(1).node->getType())
						{
						case Node::NodeType::ECVEC3:
							engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveColour =
								(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3>*>(getInput(1).node))->inputVec3;
							break;
						case Node::NodeType::ECVEC3COLOUR:
							engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveColour =
								(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR>*>(getInput(1).node))->inputVec3;
							break;
						case Node::NodeType::MUL:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(1).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveColour =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(1).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						case Node::NodeType::DIV:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(1).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveColour =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(1).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						case Node::NodeType::ADD:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(1).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveColour =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(1).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						case Node::NodeType::SUB:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(1).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.EmissiveColour =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(1).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						default:
							LinkError = true;
						}
						ImGui::SetNextItemWidth(50);
						ECGui::DrawTextWidget<std::string>("EmissiveColour", EMPTY_STRING);
					}
					else
					{
						engine->gPBRManager->ResetAlbedoConstant();
						ImGui::SetNextItemWidth(50);
						ECGui::DrawTextWidget<std::string>("EmissiveColour", EMPTY_STRING);
					}
					endInput();
					ImNodes::PopColorStyle();
				}
				else
				{
					//Has Texture
					ImNodes::PushColorStyle(
						ImNodesCol_Pin, IM_COL32(60, 0, 0, 255));
					beginInput();
					if (getInput(1).node)
					{
						if (getInput(1).node->getType() == Node::NodeType::BOOL)
						{
							auto downcastedPtr = dynamic_cast<BoolNode*>(getInput(1).node);
							//A = downcastedPtr->inputFloat;
							hasTexture = downcastedPtr->inputBool;
							engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.HasTexture = hasTexture;
							ImGui::SetNextItemWidth(50);
						}
						else
						{
							LinkError = true;
						}
						ECGui::CheckBoxBool("Has Textures", &hasTexture, false);
						engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.HasTexture = hasTexture;
					}
					else
					{
						ImGui::SetNextItemWidth(50);
						ECGui::CheckBoxBool("Has Texture", &hasTexture, false);
						engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.HasTexture = hasTexture;
					}
					endInput();
					ImNodes::PopColorStyle();

					//base reflective
					ImNodes::PushColorStyle(
						ImNodesCol_Pin, IM_COL32(200, 42, 42, 255));
					beginInput();
					if (getInput(2).node)
					{
						switch (getInput(2).node->getType())
						{
						case Node::NodeType::ECVEC3:
							engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity =
								(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3>*>(getInput(2).node))->inputVec3;
							break;
						case Node::NodeType::ECVEC3COLOUR:
							engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity =
								(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR>*>(getInput(2).node))->inputVec3;
							break;
						case Node::NodeType::MUL:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(2).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(2).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						case Node::NodeType::DIV:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(2).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(2).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						case Node::NodeType::ADD:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(2).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(2).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						case Node::NodeType::SUB:
							if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(2).node))->vec3Mode)
							{
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity =
									(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(2).node))->vec3Output;
							}
							else
							{
								LinkError = true;
							}
							break;
						default:
							LinkError = true;
						}

						ImGui::SetNextItemWidth(50);
						ECGui::DrawTextWidget<std::string>("Base Reflective", EMPTY_STRING);
					}
					else
					{
						engine->gPBRManager->ResetReflectivity();
						ImGui::SetNextItemWidth(50);
						ECGui::DrawTextWidget<std::string>("Base Reflective", EMPTY_STRING);
					}
					endInput();
					ImNodes::PopColorStyle();

					if (!hasTexture)
					{
						// Albedo Constant
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(100, 42, 42, 255));
						beginInput();
						// check for first pin if its linked
						if (getInput(3).node)
						{
							//might case problem down the line 
							switch (getInput(3).node->getType())
							{
							case Node::NodeType::ECVEC3:
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant =
									(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3>*>(getInput(3).node))->inputVec3;
								break;
							case Node::NodeType::ECVEC3COLOUR:
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant =
									(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR>*>(getInput(3).node))->inputVec3;
								break;
							case Node::NodeType::MUL:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(3).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(3).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::DIV:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(3).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(3).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::ADD:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(3).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(3).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::SUB:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(3).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(3).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							default:
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Albedo Constant", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->ResetAlbedoConstant();
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Albedo Constant", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();

						// metallic constant
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(0, 60, 0, 255));
						beginInput();
						if (getInput(4).node)
						{
							//might case problem down the line 
							switch (getInput(4).node->getType())
							{
							case Node::NodeType::FLOAT:
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.MetallicConstant =
									(dynamic_cast<FloatNode*>(getInput(4).node))->inputFloat;
								break;
							case Node::NodeType::MUL:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(4).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.MetallicConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(4).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::DIV:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(4).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.MetallicConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(4).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::ADD:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(4).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.MetallicConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(4).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::SUB:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(4).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.MetallicConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(4).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							default:
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Metallic Constant", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->ResetMetallicConstant();
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Metallic Constant", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();

						// Roughness constant
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(0, 60, 0, 255));
						beginInput();
						if (getInput(5).node)
						{
							//might case problem down the line 
							switch (getInput(5).node->getType())
							{
							case Node::NodeType::FLOAT:
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.RoughnessConstant =
									(dynamic_cast<FloatNode*>(getInput(5).node))->inputFloat;
								break;
							case Node::NodeType::MUL:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(5).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.RoughnessConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(5).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::DIV:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(5).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.RoughnessConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(5).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::ADD:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(5).node))->output)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.RoughnessConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(5).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::SUB:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(5).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.RoughnessConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(5).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							default:
								LinkError = true;
							}

							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Roughness Constant", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->ResetRoughnessConstant();
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Roughness Constant", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();
						// AO constant
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(0, 60, 0, 255));
						beginInput();
						if (getInput(6).node)
						{
							switch (getInput(6).node->getType())
							{
							case Node::NodeType::FLOAT:
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AoConstant =
									(dynamic_cast<FloatNode*>(getInput(6).node))->inputFloat;
								break;
							case Node::NodeType::MUL:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(6).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(6).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::DIV:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(6).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(6).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::ADD:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(6).node))->output)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(6).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::SUB:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(6).node))->floatMode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AoConstant =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(6).node))->output;
								}
								else
								{
									LinkError = true;
								}
								break;
							default:
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Ao Constant", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->ResetAoConstant();
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Ao Constant", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();
					}
					else
					{
						//normal map
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(60, 0, 0, 255));
						beginInput();
						if (getInput(3).node)
						{
							if (getInput(3).node->getType() == Node::NodeType::BOOL)
							{
								auto downcastedPtr = dynamic_cast<BoolNode*>(getInput(3).node);
								NormalMap = downcastedPtr->inputBool;
								ImGui::SetNextItemWidth(50);

								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.IsNormalMap = NormalMap;
							}
							else
							{
								LinkError = true;
							}
							ECGui::CheckBoxBool("", &NormalMap, false);
						}
						else
						{
							ImGui::SetNextItemWidth(50);
							ECGui::CheckBoxBool("Normal Map", &NormalMap, false);
							engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.IsNormalMap = NormalMap;
						}

						endInput();
						ImNodes::PopColorStyle();

						// colours
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(100, 42, 42, 255));
						beginInput();
						// check for first pin if its linked
						if (getInput(4).node)
						{
							//might case problem down the line 
							switch (getInput(4).node->getType())
							{
							case Node::NodeType::ECVEC3:
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.SurfaceColour =
									(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3>*>(getInput(4).node))->inputVec3;
								break;
							case Node::NodeType::ECVEC3COLOUR:
								engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.SurfaceColour =
									(dynamic_cast<Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR>*>(getInput(4).node))->inputVec3;
								break;
							case Node::NodeType::MUL:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(4).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.SurfaceColour =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::MUL>*>(getInput(4).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::DIV:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(4).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.SurfaceColour =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::DIV>*>(getInput(4).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::ADD:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(4).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.SurfaceColour =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::ADD>*>(getInput(4).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							case Node::NodeType::SUB:
								if ((dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(4).node))->vec3Mode)
								{
									engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.SurfaceColour =
										(dynamic_cast<BinOpNode<MaterialNode::Node::InstructionType::SUB>*>(getInput(4).node))->vec3Output;
								}
								else
								{
									LinkError = true;
								}
								break;
							default:
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Material Colour", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->ResetSurfaceColour();
							ECGui::DrawTextWidget<std::string>("Material Colour", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();


						// for Albdeo texture nodes
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(255, 255, 255, 255));
						beginInput();
						if (getInput(5).node)
						{
							if (getInput(5).node->getType() == Node::NodeType::TEXTURE)
							{
								auto downcastedPtr = dynamic_cast<TextureNode*>(getInput(5).node);
								engine->gPBRManager->GenerateMaterialTexture(downcastedPtr->folderName_, downcastedPtr->fileName_);
								//auto i = engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.Albedo;
							}
							else
							{
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Albdeo Texture", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->AlbedoTexture, MaterialType::MT_ALBEDO);
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Albdeo Texture", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();


						// for Normal texture nodes
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(255, 255, 255, 255));
						beginInput();
						if (getInput(6).node)
						{
							if (getInput(6).node->getType() == Node::NodeType::TEXTURE)
							{
								auto downcastedPtr = dynamic_cast<TextureNode*>(getInput(6).node);
								//A = downcastedPtr->inputFloat;
								engine->gPBRManager->GenerateMaterialTexture(downcastedPtr->folderName_, downcastedPtr->fileName_);
							}
							else
							{
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Normal Texture", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->NormalTexture, MaterialType::MT_NORMAL);
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Normal Texture", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();


						// for metallic texture nodes
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(255, 255, 255, 255));
						beginInput();
						if (getInput(7).node)
						{
							if (getInput(7).node->getType() == Node::NodeType::TEXTURE)
							{
								auto downcastedPtr = dynamic_cast<TextureNode*>(getInput(7).node);
								//A = downcastedPtr->inputFloat;
								engine->gPBRManager->GenerateMaterialTexture(downcastedPtr->folderName_, downcastedPtr->fileName_);
							}
							else
							{
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Metallic Texture", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->MetallicTexture, MaterialType::MT_METALLIC);
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Metallic Texture", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();

						// for Roughness texture nodes
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(255, 255, 255, 255));
						beginInput();
						if (getInput(8).node)
						{
							if (getInput(8).node->getType() == Node::NodeType::TEXTURE)
							{
								auto downcastedPtr = dynamic_cast<TextureNode*>(getInput(8).node);
								//A = downcastedPtr->inputFloat;
								engine->gPBRManager->GenerateMaterialTexture(downcastedPtr->folderName_, downcastedPtr->fileName_);
							}
							else
							{
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Roughness Texture", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->RoughnessTexture, MaterialType::MT_ROUGHNESS);
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Roughness Texture", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();

						// for Ao texture nodes
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(255, 255, 255, 255));
						beginInput();
						if (getInput(9).node)
						{
							if (getInput(9).node->getType() == Node::NodeType::TEXTURE)
							{
								auto downcastedPtr = dynamic_cast<TextureNode*>(getInput(9).node);
								//A = downcastedPtr->inputFloat;
								engine->gPBRManager->GenerateMaterialTexture(downcastedPtr->folderName_, downcastedPtr->fileName_);
							}
							else
							{
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Ao Texture", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->AoTexture, MaterialType::MT_AO);
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Ao Texture", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();

						// for Height texture nodes
						ImNodes::PushColorStyle(
							ImNodesCol_Pin, IM_COL32(255, 255, 255, 255));
						beginInput();
						if (getInput(10).node)
						{
							if (getInput(10).node->getType() == Node::NodeType::TEXTURE)
							{
								auto downcastedPtr = dynamic_cast<TextureNode*>(getInput(10).node);
								//A = downcastedPtr->inputFloat;
								engine->gPBRManager->GenerateMaterialTexture(downcastedPtr->folderName_, downcastedPtr->fileName_);
							}
							else
							{
								LinkError = true;
							}
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Height Texture", EMPTY_STRING);
						}
						else
						{
							engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->HeightTexture, MaterialType::MT_HEIGHT);
							ImGui::SetNextItemWidth(50);
							ECGui::DrawTextWidget<std::string>("Height Texture", EMPTY_STRING);
						}
						endInput();
						ImNodes::PopColorStyle();
					}
				}

				if (LinkError)
				{
					ECGui::OpenPopup("Link Error");
				}

				if (ECGui::BeginPopupModal("Link Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ECGui::DrawTextWidget<const char*>("Wrong Node linked, link action canceled.", EMPTY_STRING);
					ECGui::InsertHorizontalLineSeperator();
					if (ECGui::ButtonBool("OK", ImVec2(320, 0)))
					{
						res.links.pop_back();
						LinkError = false;
						ECGui::CloseCurrentPopup();
					}
					ECGui::EndPopup();
				}
				ImNodes::EndNode();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				return false;
			}

			bool LinkError = false;
			bool NormalMap = false;
			bool hasTexture = false;
			bool isEmissive = false;
		};

		struct BoolNode : public Node
		{
			BoolNode(MaterialNode& res) : Node(res) {}
			NodeType getType() const override
			{
				return NodeType::BOOL;
			}

			bool onGUI(int nodeId_) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(60, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(70, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(70, 0, 0, 255));
				ImNodes::BeginNode(nodeId_);
				ImNodes::BeginNodeTitleBar();
				ECGui::DrawTextWidget<std::string>("Bool", EMPTY_STRING);
				ImNodes::EndNodeTitleBar();
				beginOutput();
				ImGui::SetNextItemWidth(60);
				ECGui::CheckBoxBool("Input Bool", &inputBool, true);
				endOutput();
				ImNodes::EndNode();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				return false;
			}
			bool inputBool = false;
		};

		struct TextureNode :public Node
		{
			TextureNode(MaterialNode& res) : Node(res) {}

			NodeType getType() const override
			{
				return NodeType::TEXTURE;
			}

			bool onGUI(int nodeId_) override
			{
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBar, IM_COL32(0, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarSelected, IM_COL32(0, 0, 0, 255));
				ImNodes::PushColorStyle(
					ImNodesCol_TitleBarHovered, IM_COL32(0, 0, 0, 255));
				ImNodes::BeginNode(nodeId_);
				ImNodes::BeginNodeTitleBar();
				ECGui::DrawTextWidget<std::string>(fileName_.c_str(), EMPTY_STRING);
				ImNodes::EndNodeTitleBar();

				ImGui::SetNextItemWidth(100);
				ECGui::Image((ImTextureID)(size_t)textureID, { 69,69 }, { 1,0 }, { 2,1 });
				beginOutput();

				endOutput();
				ImNodes::EndNode();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
				return false;
			}

			void setTextureId(const std::string folderName, std::string fileName)
			{
				fileName_ = fileName;
				folderName_ = folderName;
				textureID = engine->gPBRManager->GetMaterialTextureID(folderName_.c_str(), fileName_.c_str());
			}

			std::string folderName_;
			std::string fileName_;
			int textureID = 0;
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
				//MULTIPLY_ADD,
			case Node::NodeType::DIV:
				node = std::make_shared<BinOpNode<MaterialNode::Node::InstructionType::DIV>>(*this);
				break;
			case Node::NodeType::MUL:
				node = std::make_shared<BinOpNode<MaterialNode::Node::InstructionType::MUL>>(*this);
				break;
			case Node::NodeType::ADD:
				node = std::make_shared<BinOpNode<MaterialNode::Node::InstructionType::ADD>>(*this);
				break;
			case Node::NodeType::SUB:
				node = std::make_shared<BinOpNode<MaterialNode::Node::InstructionType::SUB>>(*this);
				break;
			case Node::NodeType::FLOAT:
				node = std::make_shared <FloatNode>(*this);
				break;
			case Node::NodeType::ECVEC3:
				node = std::make_shared <Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3>>(*this);
				break;
			case Node::NodeType::ECVEC3COLOUR:
				node = std::make_shared <Vec3Nodes<MaterialNode::Node::VEC3TYPE::ECVEC3COLOUR>>(*this);
				break;
			case Node::NodeType::BOOL:
				node = std::make_shared <BoolNode>(*this);
				break;
			}
			nodes.push_back(node);
			return nodes.back().get();
		}

		//TODO JIANHERNG PLSSSS PAPA
		ImNodesEditorContext* context = nullptr;
		std::vector<std::shared_ptr<Node>>     nodes;
		std::vector<Link>     links;
		int                   current_id = 0;

		bool initialized = false;
		bool BaseNodeExist = false;
		int m_context_link = 0;
		int m_context_node = 0;

		//
		int findMaterialNodePos(MaterialNode editor, int id);

		void DrawMaterialNodeEditor(const char* graphName, MaterialNode& editor);

		void Reset();

		void CreateBaseMaterialNode();

		void CreateTextureNode(std::string folderName, std::string fileName);
	};

	static inline float  Saturate(float f) 
	{ 
		return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; 
	}

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
		(void)graphName;
		ImNodes::EditorContextSet(editor.context);

		ImNodes::BeginNodeEditor();

		static std::string TextureString = { "" };
		bool context_open = false;
		if (TextureString.size() != 0)
		{
			size_t lastindex = TextureString.find_last_of(".");
			std::string tempName = TextureString.substr(0, lastindex);

			std::string fileName = tempName;
			size_t last_slash_idx = fileName.find_last_of("\\/");
			if (std::string::npos != last_slash_idx)
			{
				fileName.erase(0, last_slash_idx + 1);
			}


			std::string directory;
			last_slash_idx = TextureString.rfind('\\');
			if (std::string::npos != last_slash_idx)
			{
				directory = TextureString.substr(0, last_slash_idx);
			}

			last_slash_idx = directory.find_last_of("\\/");
			if (std::string::npos != last_slash_idx)
			{
				directory.erase(0, last_slash_idx + 1);
			}

			CreateTextureNode(directory, fileName);
			TextureString.clear();
		}

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

		for (const MaterialNode::Link lnks : editor.links)
		{
			ImNodes::Link(lnks.linkId, lnks.startPoint, lnks.endPoint);
		}

		ImNodes::EndNodeEditorReturnString(TextureString);
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

	inline void MaterialNode::CreateTextureNode(std::string folderName, std::string fileName)
	{
		TextureNode temp = TextureNode(*this);
		temp.setTextureId(folderName, fileName);
		std::shared_ptr<Node> node = std::make_shared <TextureNode>(temp);
		nodes.push_back(node);
	}
};