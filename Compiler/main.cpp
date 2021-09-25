#include "pch.h"
#include "Compilers.h"
#include "CompilerSerializer.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <any>

using namespace EclipseCompiler;
using namespace std;

std::unique_ptr<Compilers> AllCompilers;

void process(std::string& s, glm::vec4& vec4)
{
    std::vector<int> T;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);

        T.push_back(std::stof(token.c_str()));
        s.erase(0, pos + delimiter.length());
    }

    s.erase(0, pos + delimiter.length());
    T.push_back(std::stof(s.c_str()));
    vec4 = glm::vec4{ T[0] , T[1] , T[2] , T[3] };
}

glm::vec3 ReturnVec3(std::string& s)
{
    std::vector<float> T;
    glm::vec3 NewT;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        T.push_back(std::stof(token.c_str()));
        s.erase(0, pos + delimiter.length());
    }

    s.erase(0, pos + delimiter.length());
    T.push_back(std::stof(s.c_str()));
    NewT = glm::vec3{ T[0] , T[1] , T[2] };

    return NewT;
}

glm::vec2 ReturnVec2(std::string& s)
{
    std::vector<int> T;
    glm::vec2 NewT;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        T.push_back(std::stof(token.c_str()));
        s.erase(0, pos + delimiter.length());
    }

    s.erase(0, pos + delimiter.length());
    T.push_back(std::stof(s.c_str()));
    NewT = glm::vec2{ T[0] , T[1] };

    return NewT;
}

int main()
{
    CompilerManager Manager;

    std::string data;

    AllCompilers = std::make_unique<Compilers>();
    AllCompilers->LoadModels("..//Eclipse//src//Assets//ASSModels");

    ofstream file1;

    file1.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        file1.open("CSC2134.txt", fstream::in | fstream::out | fstream::binary | ios::trunc);
    }
    catch (ifstream::failure e)
    {
        cout << "Exception opening/reading file" << e.what();
    }

    for (auto& i : AllCompilers->Geometry)
    {
        // Name , NoTextures , Diffuse , Specular , Ambient
        file1 << i.first << std::endl;
        file1 << (i.second->NoTex ? 1 : 0) << std::endl;
        file1 << i.second->Diffuse.x << " " << i.second->Diffuse.y << " " << i.second->Diffuse.z << " " << i.second->Diffuse.w << std::endl;
        file1 << i.second->Specular.x << " " << i.second->Specular.y << " " << i.second->Specular.z << " " << i.second->Specular.w << std::endl;
        file1 << i.second->Ambient.x << " " << i.second->Ambient.y << " " << i.second->Ambient.z << " " << i.second->Ambient.z << std::endl;
        file1 << "-" << std::endl;

        // Vertices
        // Position , Normal , TextureCoordinates
        for (auto& Vertices : i.second->Vertices)
        {
            file1 << Vertices.Position.x << " " << Vertices.Position.y << " " << Vertices.Position.z << std::endl;
            file1 << Vertices.Normal.x << " " << Vertices.Normal.y << " " << Vertices.Normal.z << std::endl;
            file1 << Vertices.TextureCoodinates.x << " " << Vertices.TextureCoodinates.y << std::endl;
        }
        file1 << "--" << std::endl;

        for (auto& Indices : i.second->Indices)
        {
            file1 << Indices << std::endl;
        }
        file1 << "---" << std::endl;

    }
    file1.close();

    //fstream  afile;
    //afile.open("CSC2134.txt", ios::out | ios::in);
    //std::string eachline;

    //// This getline is to getName
    //while (!afile.eof())
    //{
    //    Mesh Model;

    //    // Name
    //    std::getline(afile, eachline);
    //    Model.MeshName = eachline;

    //    // NoTextures
    //    std::getline(afile, eachline);
    //    Model.NoTex = std::atoi(eachline.c_str());

    //    // Diffuse
    //    std::getline(afile, eachline);
    //    process(eachline, Model.Diffuse);
    //    std::cout << eachline << std::endl;

    //    // Specular
    //    std::getline(afile, eachline);
    //    process(eachline, Model.Specular);
    //    std::cout << eachline << std::endl;

    //    // Ambient
    //    std::getline(afile, eachline);
    //    process(eachline, Model.Ambient);
    //    std::cout << eachline << std::endl;

    //    std::getline(afile, eachline);
    //    if (eachline == "-")
    //        std::cout << "Vertices Next" << std::endl;

    //    while (std::getline(afile, eachline))
    //    {
    //        if (eachline == "--")
    //        {
    //            std::cout << "Indices Next" << std::endl;
    //            break;
    //        }

    //        Vertex A;
    //        A.Position = ReturnVec3(eachline);

    //        std::getline(afile, eachline);
    //        A.Normal = ReturnVec3(eachline);

    //        std::getline(afile, eachline);
    //        A.TextureCoodinates = ReturnVec2(eachline);

    //        Model.Vertices.push_back(A);
    //    }

    //    while (std::getline(afile, eachline))
    //    {
    //        if (eachline == "---")
    //        {
    //            std::cout << "Next Model" << std::endl;
    //            break;
    //        }

    //        Model.Indices.push_back(std::atoi(eachline.c_str()));
    //    }

    //    int i = 0;
    //}

    return 0;
}

