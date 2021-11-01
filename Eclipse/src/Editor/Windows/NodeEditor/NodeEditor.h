#pragma once
#include "../Interface/ECGuiWindow.h"
#include <imgui_internal.h>
#include "ImNodes/imnodes.h"
#include <imnodes.h>

namespace Eclipse
{
	enum class NodeType
	{
		DEFAULT,
		FLOAT,
		PRINT,
		VEC2,
		VEC3,
		VEC4
	};

	struct	Node
	{
        std::string name;
		int nodeId;
		float value;
		NodeType nodeType;
		Node(const std::string nodeName ,const int id, const float data, NodeType inputType = NodeType::DEFAULT)
			: name(nodeName),nodeId(id), value(data), nodeType(inputType){}

	};

    template<class T>
    T clamp(T x, T a, T b)
    {
        return std::min(b, std::max(x, a));
    }


	struct Link
	{
		int linkId;
		int startPoint, endPoint;
	};

	struct Editor_
	{
		ImNodesEditorContext* context = nullptr;
		std::vector<Node>     nodes;
		std::vector<Link>     links;
		int                   current_id = 0;
	};


	class NodeEditorWindow : public ECGuiWindow
	{
        Editor_ editor1;
	public :

		void CreateInputFloatNode();

		void CreatePrintNode();

        void DrawNodeEditor(const char* graphName, Editor_& editor);

		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();
		~NodeEditorWindow();
	};


}
