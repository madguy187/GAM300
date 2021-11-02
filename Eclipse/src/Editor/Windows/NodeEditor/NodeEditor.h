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
		ENTITY,
		VEC3,
		VEC4
	};

	struct	Node
	{
        std::string name;
		int nodeId;
		float value = 0.0f;
		NodeType nodeType;
		Node(const std::string nodeName ,const int id, NodeType inputType = NodeType::DEFAULT)
			: name(nodeName),nodeId(id), nodeType(inputType){}

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
		bool initialized= false;
        Editor_ editor1;

		int findNodePos(Editor_ editor, int id);
	public :

		NodeEditorWindow();
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();
		~NodeEditorWindow();



		void CreateInputFloatNode(Editor_& editor);

		void CreatePrintNode(Editor_& editor);

		void DrawNodeEditor(const char* graphName, Editor_& editor);

		void CreateEnitityNode(Editor_& editor, std::string name);
	};


}
