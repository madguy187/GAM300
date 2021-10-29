#pragma once
#include "../Interface/ECGuiWindow.h"
#include "Utilities/builders.h"
#include "Utilities/widgets.h"

# include "../src/Editor/Windows/NodeEditor/NodeEditorBackBone/imgui_node_editor.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui_internal.h>


namespace Eclipse
{
    static inline ImRect ImGui_GetItemRect()
    {
        return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    }

    static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
    {
        auto result = rect;
        result.Min.x -= x;
        result.Min.y -= y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }

    namespace ed = ax::NodeEditor;

    using namespace ax;

    namespace util = ax::NodeEditor::Utilities;

    using ax::Widgets::IconType;

    static NodeEditor::EditorContext* g_Context = nullptr;

    enum class PinType
    {
        Flow,
        Bool,
        Int,
        Float,
        String,
        Object,
        Function,
        Delegate,
    };

    enum class NodeType
    {
        test,
        testmultiply
    };

    enum class PinKind
    {
        Output,
        Input
    };

    struct Node;

    struct Pin
    {
        ed::PinId   ID;
        ::Node* Node;
        char Data[128] = "";
        std::string Name;
        PinType     Type;
        PinKind     Kind;

        Pin(int id, const char* name, PinType type) :
            ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
        {
            memset(Data, 0, 128);
        }
    };

    struct Node
    {
        ed::NodeId ID;
        std::vector<Pin> Inputs;
        std::vector<Pin> Outputs;
        ImColor Color;
        NodeType Type;
        ImVec2 Size;
        std::string Name;
        std::string State;
        std::string SavedState;

        Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) :
            ID(id), Name(name), Color(color), Type(NodeType::test), Size(0, 0)
        {
        }
    };

    struct Link
    {
        ed::LinkId ID;

        ed::PinId StartPinID;
        ed::PinId EndPinID;

        ImColor Color;

        Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) :
            ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
        {
        }
    };

    struct NodeIdLess
    {
        bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
        {
            return lhs.AsPointer() < rhs.AsPointer();
        }
    };


	class NodeEditorWindow : public ECGuiWindow
	{
        const int            s_PinIconSize = 24;

        std::vector<Node>    s_Nodes;
        std::vector<Link>    s_Links;

        const float          s_TouchTime = 1.0f;
        std::map<ed::NodeId, float, NodeIdLess> s_NodeTouchTime;
        int s_NextId = 1;



        /////////////////////////////
        ed::NodeId contextNodeId = 0;
        ed::LinkId contextLinkId = 0;
        ed::PinId  contextPinId = 0;
        bool createNewNode = false;
        Pin* newNodeLinkPin = nullptr;
        Pin* newLinkPin = nullptr;

	public:

        Node* test();
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();
        ~NodeEditorWindow() override;

        int GetNextId();

        float GetTouchProgress(ed::NodeId id);

        ed::LinkId GetNextLinkId();

        void TouchNode(ed::NodeId id);

        void UpdateTouch();

        void BuildNode(Node* node);

        void BuildNodes();

        void NodeEditorWindow::DrawPinIcon(const Pin& pin, bool connected, int alpha);

        ImColor GetIconColor(PinType type);

        Node* FindNode(ed::NodeId id);

        Link* FindLink(ed::LinkId id);

        Pin* FindPin(ed::PinId id);

        bool IsPinLinked(ed::PinId id);

        bool CanCreateLink(Pin* a, Pin* b);

        Node* SpawnInputActionNode();

        Node* SpawnBranchNode();

        Node* SpawnOutputActionNode();

        Node* SpawnMultiplyNode();

        Node* NodeEditorWindow::testPrintOnConsole();

        bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);

    };
}
