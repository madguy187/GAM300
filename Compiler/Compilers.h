#pragma once
#include "AssimpLoader.h"
#include "CompilerSerializer.h"

namespace EclipseCompiler
{
    class Compilers
    {
        Serializer sz;

    public:
        inline void SerializeData() {}

        template <typename CompType>
        inline void SerializeComponentData(const CompType& data)
        {
            sz.AddAttributeToElement("Unavailable", true);
        }

        template <typename CompType>
        inline void SerializeComponent(Mesh& ent)
        {
            sz.StartElement("MeshTest");
            SerializeComponentData<Mesh>(ent);
            sz.CloseElement();
        }

        template <typename T, typename... Args>
        inline void SerializeData(const char* name, T element, Args... elements)
        {
            sz.StartElement(name);
            sz.AddAttributeToElement("value", element);
            sz.CloseElement();
            SerializeData(elements...);
        }

        template <>
        inline void SerializeComponentData<Mesh>(const Mesh& data)
        {
            SerializeData(
                "MeshName", data.MeshName,
                "NoTextures", data.NoTex,
                "Diffuse", data.Diffuse,
                "Specular", data.Specular,
                "Ambient", data.Ambient
            );

            for (int i = 0; i < data.Vertices.size(); i++)
            {
                SerializeData(
                    "Position", data.Vertices[i].Position
                    );
            }

            for (int i = 0; i < data.Vertices.size(); i++)
            {
                SerializeData(
                    "Normals", data.Vertices[i].Normal
                );
            }

            for (int i = 0; i < data.Vertices.size(); i++)
            {
                SerializeData(
                    "TextureCoordinates", data.Vertices[i].TextureCoodinates
                );
            }

            SerializeData(
                "Indices", data.Indices
            );
        }

        void SaveXML(const std::string& savePath)
        {
            std::filesystem::path p{ savePath };
            std::filesystem::create_directories(p.remove_filename());
            sz._doc.SaveFile(savePath.c_str());
            sz._doc.Clear();
            sz._currElement = nullptr;
        }

        std::unordered_map<std::string, std::unique_ptr<Mesh>> Geometry;
        AssimpLoader Loader;

        void LoadModels(const std::string& modelFile);
    };
}