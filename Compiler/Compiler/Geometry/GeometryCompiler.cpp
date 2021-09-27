#include "pch.h"
#include "GeometryCompiler.h"
#include <string> 

struct vec3
{
    float x = 30;
    float y = 40;
    float z = 50;
};

class Test
{
public:
    bool j = false; //OK
    float x = 10;
    int y = 20;
    vec3 z;
    char* mystring;

    std::array<int, 6> offsets;
    Test()
    {
        mystring = new char[20];
        strcpy_s(mystring, 5, "poop");
        mystring[5] = '\0';
        offsets = { 0, sizeof(bool), sizeof(x), sizeof(y), sizeof(z), 20 };
    }
};

//struct Class1
//{
//    int x;
//    float y;
//    float z;
//    std::string a = "11111111111111111111";
//
//    std::vector<size_t> offSetsforObject;
//
//    Class1()
//    {
//        offSetsforObject = { 4, sizeof(y), 4, sizeof(a) };
//    }
//};
//
//struct FileManager
//{
//    Class1* obj = new Class1;
//    std::ofstream GeometryFile;
//
//    for (int i = 0; i < 3; ++i)
//    {
//        char* address = reinterpret_cast<char*>(obj) + obj->offSetsforObject[i];
//        GeometryFile.write(reinterpret_cast<const char*>(address), obj->offSetsforObject[i]);
//    }
//};

//    void WriteToBinary(const char* path)
//    {
//
//       f.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
//                std::fstream::out |
//                std::fstream::binary |
//                std::ios::trunc |
//                std::ios::binary);
//
//        GeometryFile.write(reinterpret_cast<const char*>(&x), sizeof(int));
//        GeometryFile.write(reinterpret_cast<const char*>(&y), sizeof(float));
//        GeometryFile.write(reinterpret_cast<const char*>(&z), sizeof(float));
//        GeometryFile.close();
//    }
//    void ReadFile()
//    {
//        GeometryFile.write(reinterpret_cast<const char*>(&x), sizeof(int));
//        GeometryFile.write(reinterpret_cast<const char*>(&y), sizeof(float));
//        GeometryFile.write(reinterpret_cast<const char*>(&z), sizeof(float));
//    }

namespace EclipseCompiler
{
    void GeometryCompiler::Init()
    {
        LoadFile("..//Eclipse//src//Assets//ASSModels");
    }

    void GeometryCompiler::LoadFile(const std::string& modelFile)
    {
        for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
        {
            const auto& path = dirEntry.path();
            auto relativePath = relative(path, "..//Eclipse//src//");
            std::string FolderName = relativePath.filename().string();

            std::string GoIntoModelFolder = ("..//Eclipse//src/Assets/ASSModels/" + FolderName);

            for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
            {
                const auto& FbxOrGltf = dirEntry.path();
                auto relativePath = relative(FbxOrGltf, "..//Eclipse//src//");
                std::string FbxOrGltfName = relativePath.filename().string();

                if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
                {
                    std::string PathName = ("..//Eclipse//src/Assets/ASSModels/" + FolderName + "/" + FbxOrGltfName).c_str();
                    std::unique_ptr<AssimpLoader> ptr = std::make_unique< AssimpLoader>();
                    ptr->LoadAssimpModel(PathName, Geometry);
                }
            }
        }
    }

    void GeometryCompiler::WriteToFile(std::unordered_map<std::string, Mesh>& In)
    {

        GeometryFile.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);


#ifndef NEW_IMPLEMENTATION
        //int LoopCounter = 0;
        //std::unordered_map<std::string, std::unique_ptr<Mesh>>::iterator hi;

        //Test A;
        //int SizeOfBoolean = sizeof(glm::vec3);
        //GeometryFile.write(reinterpret_cast<const char*>(&SizeOfBoolean), sizeof(int));
        //GeometryFile.write(reinterpret_cast<const char*>(&A.Hi), SizeOfBoolean);

        //GeometryFile.close();

        //for (auto i = Geometry.begin(); i != Geometry.end(); i++)
        //{
        //    std::cout << std::addressof(i->second.get()->MeshName) << std::endl;
        //    std::cout << ((unsigned char*)i->second.get()) << std::endl;

        //    glm::vec4 Hi = glm::vec4(1.1, 2.2, 3.3, 4.2);

        //    i->second.get()->NoTex = true;
        //    i->second.get()->Diffuse = Hi;

        //    // debug this
        //    for (size_t Internal = 0; Internal < 3; ++Internal)
        //    {
        //        char* obj;

        //        if (Internal == 0)
        //        {
        //            obj = *(reinterpret_cast<char**>(i->second.get())) + i->second->offSetsforObject[Internal];
        //        }
        //        else
        //        {
        //            obj = reinterpret_cast<char*>(i->second.get()) + i->second->offSetsforObject[Internal];
        //        }

        //        GeometryFile.write(reinterpret_cast<const char*>(&i->second->offSetsforObject[Internal + 1]), sizeof(int));
        //        GeometryFile.write(obj, i->second->offSetsforObject[Internal + 1]);
        //    }

        //    //// Mesh Name
        //    //int SizeOfMeshName = std::strlen(i.second->MeshName)+1;
        //    //GeometryFile.write(reinterpret_cast<const char*>(&SizeOfMeshName), sizeof(int));
        //    //GeometryFile.write(reinterpret_cast<const char*>(i.second->MeshName), SizeOfMeshName);

        //    //// HasTexture
        //    //int SizeOfBoolean = sizeof(NoTextures);
        //    //GeometryFile.write(reinterpret_cast<const char*>(&SizeOfBoolean), sizeof(bool));
        //    //GeometryFile.write(reinterpret_cast<const char*>(&i.second->NoTex), SizeOfBoolean);

        //}
        //GeometryFile.close();

        //Test A;
        //int SizeOfBoolean = sizeof(glm::vec3);
        //GeometryFile.write(reinterpret_cast<const char*>(&SizeOfBoolean), sizeof(int));
        //GeometryFile.write(reinterpret_cast<const char*>(&A.Hi), SizeOfBoolean);

        //for (auto i = Geometry.begin(); i != Geometry.end(); i++)
        //{
        //    i->second.NoTex = true;
        int totaloffset = 0;

        MeshA A;
        strcpy_s(A.MeshName, 5, "hell");
        A.MeshName[5] = '\0';

        A.Diffuse = glm::vec4{ 1.1, 1.2, 1.3, 11.4 };

        // debug this
        for (int i = 0; i < A.offSetsforObject.size()-1; ++i)
        {
            totaloffset += A.offSetsforObject[i];
            int size = A.offSetsforObject[i + 1];

            char* data = reinterpret_cast<char*>(&A) + totaloffset;
            void* v = reinterpret_cast<void*>(data);

            GeometryFile.write(reinterpret_cast<const char*>(&A.offSetsforObject[i + 1]), sizeof(A.offSetsforObject[i + 1]));
            GeometryFile.write(reinterpret_cast<const char*>(v), size);
        }

        //}
        GeometryFile.close();

        GeometryFileWrite.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
            std::ios_base::in |
            std::ios_base::binary);

        MeshA B;

        int offset = 0;

        for (int i = 0; i < B.offSetsforObject.size()-1; ++i)
        {
            int size = 0;
            char* data = reinterpret_cast<char*>(&B) + offset;
            void* v = reinterpret_cast<void*>(data);

            GeometryFileWrite.read(reinterpret_cast<char*>(&size), sizeof(size));

            // Read all the data
            GeometryFileWrite.read(reinterpret_cast<char*>(v), size);

            offset += size;
        }

        //std::cout << B.MeshName << std::endl;

        std::cout << B.Diffuse.x << std::endl;
        //if (GeometryFile)
        //{
        //    int offset = 0;

        //    A.x = 123;
        //    A.y = 456;
        //    A.z.x = 10000;
        //    A.j = true;
        //    strcpy_s(A.mystring, 5, "hell");
        //    A.mystring[5] = '\0';
        //    A.offsets[4] = 5;

        //    for (int i = 0; i < A.offsets.size() - 1; ++i)
        //    {
        //        offset += A.offsets[i];
        //        int size = A.offsets[i + 1];

        //        char* data = reinterpret_cast<char*>(&A) + offset;
        //        void* v = reinterpret_cast<void*>(data);

        //        GeometryFile.write(reinterpret_cast<const char*>(&A.offsets[i + 1]), sizeof(A.offsets[i + 1]));
        //        GeometryFile.write(reinterpret_cast<const char*>(v), size);
        //    }

        //    std::cout << "--------WRITING FILE--------" << std::endl;
        //    std::cout << "Bool: " << A.j << std::endl;
        //    std::cout << "float: " << A.x << std::endl;
        //    std::cout << "Int : " << A.y << std::endl;
        //    std::cout << "Vec3 : " << A.z.x << std::endl;
        //    std::cout << "String : " << A.mystring << std::endl;
        //}

        //GeometryFile.close();

        //ReadFile();
#else
        for (auto& i : In)
        {
            // Name , NoTextures , Diffuse , Specular , Ambient
            GeometryFile << i.first << std::endl;
            GeometryFile << (i.second->NoTex ? 1 : 0) << std::endl;
            GeometryFile << i.second->Diffuse.x << " " << i.second->Diffuse.y << " " << i.second->Diffuse.z << " " << i.second->Diffuse.w << std::endl;
            GeometryFile << i.second->Specular.x << " " << i.second->Specular.y << " " << i.second->Specular.z << " " << i.second->Specular.w << std::endl;
            GeometryFile << i.second->Ambient.x << " " << i.second->Ambient.y << " " << i.second->Ambient.z << " " << i.second->Ambient.z << std::endl;
            GeometryFile << "-" << std::endl;

            // Vertices
            // Position , Normal , TextureCoordinates
            for (auto& Vertices : i.second->Vertices)
            {
                GeometryFile << Vertices.Position.x << " " << Vertices.Position.y << " " << Vertices.Position.z << std::endl;
                GeometryFile << Vertices.Normal.x << " " << Vertices.Normal.y << " " << Vertices.Normal.z << std::endl;
                GeometryFile << Vertices.TextureCoodinates.x << " " << Vertices.TextureCoodinates.y << std::endl;
            }
            GeometryFile << "--" << std::endl;

            for (auto& Indices : i.second->Indices)
            {
                GeometryFile << Indices << std::endl;
            }
            GeometryFile << "---" << std::endl;

        }
        GeometryFile.close();
#endif
    }

    void GeometryCompiler::ReleaseFile(std::string& in)
    {
        if (in == "1")
        {
            if (Geometry.empty())
            {
                std::cout << "No Geometry Loaded" << std::endl << std::endl;
                return;
            }

            WriteToFile(Geometry);
            std::cout << "Geometry File Produced" << std::endl << std::endl;
        }
    }

    void GeometryCompiler::Write()
    {

    }

    void GeometryCompiler::CalculateOffsets()
    {
    }

    void GeometryCompiler::ReadFile()
    {
        GeometryFileWrite.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
            std::ios::in |
            std::ios::binary);

        std::string eachline;

#ifndef NEW_IMPLEMENTATION

        //if (GeometryFileWrite)
        //{
        //    Test B;
        //    int offset = 0;

        //    for (int i = 0; i < B.offsets.size(); ++i)
        //    {
        //        int size = 0;
        //        char* data = reinterpret_cast<char*>(&B) + offset;
        //        void* v = reinterpret_cast<void*>(data);

        //        GeometryFileWrite.read(reinterpret_cast<char*>(&size), sizeof(size));

        //        // Read all the data
        //        GeometryFileWrite.read(reinterpret_cast<char*>(v), size);

        //        offset += size;
        //    }

        //    std::cout << "--------READING FILE--------" << std::endl;
        //    std::cout << "Bool: " << B.j << std::endl;
        //    std::cout << "float: " << B.x << std::endl;
        //    std::cout << "Int : " << B.y << std::endl;
        //    std::cout << "Vec3 : " << B.z.x << std::endl;
        //    std::cout << "String : " << B.mystring << std::endl;

        //}
        //GeometryFileWrite.close();

        //Mesh TestCreateModelFromCompiler;

        //int MeshNameSize = 0;
        ////TestCreateModelFromCompiler.MeshName = new char[MeshNameSize];
        //GeometryFileWrite.read(reinterpret_cast<char*>(&MeshNameSize), sizeof(int));
        //GeometryFileWrite.read(reinterpret_cast<char*>(&TestCreateModelFromCompiler.MeshName), MeshNameSize);
        //    
        //std::cout << "Test " << TestCreateModelFromCompiler.MeshName;

        //int NoTextureSize = 0;
        //GeometryFileWrite.read(reinterpret_cast<char*>(&NoTextureSize), sizeof(int));
        //GeometryFileWrite.read(reinterpret_cast<char*>(&TestCreateModelFromCompiler.NoTex), NoTextureSize);

        //std::cout << "Test Booleamn " << TestCreateModelFromCompiler.NoTex << std::endl;

        //int TestDiffuseSize = 0;
        //glm::vec4 TestDiffuse{ 0,0,0,0 };
        //glm::vec4 y;
        //GeometryFileWrite.read(reinterpret_cast<char*>(&TestDiffuseSize), sizeof(int));
        //GeometryFileWrite.read(reinterpret_cast<char*>(&y), 16);

        int i = 0;

#else   
        // This getline is to getName
        while (!GeometryFile.eof())
        {
            std::getline(GeometryFileWrite, eachline);

            if (eachline == "")
            {
                break;
            }

            Mesh Model;

            // Name
            Model.MeshName = eachline;

            // NoTextures
            std::getline(GeometryFileWrite, eachline);
            Model.NoTex = std::atoi(eachline.c_str());

            // Diffuse
            std::getline(GeometryFileWrite, eachline);
            ProcessVec4(eachline, Model.Diffuse);

            // Specular
            std::getline(GeometryFileWrite, eachline);
            ProcessVec4(eachline, Model.Specular);

            // Ambient
            std::getline(GeometryFileWrite, eachline);
            ProcessVec4(eachline, Model.Ambient);

            std::getline(GeometryFileWrite, eachline);
            //if (eachline == "-")
                //std::cout << "Vertices Next" << std::endl;

            while (std::getline(GeometryFileWrite, eachline))
            {
                if (eachline == "--")
                {
                    //std::cout << "Indices Next" << std::endl;
                    break;
                }

                Vertex A;
                A.Position = ReturnVec3(eachline);

                std::getline(GeometryFileWrite, eachline);
                A.Normal = ReturnVec3(eachline);

                std::getline(GeometryFileWrite, eachline);
                A.TextureCoodinates = ReturnVec2(eachline);

                Model.Vertices.push_back(A);
            }

            while (std::getline(GeometryFileWrite, eachline))
            {
                if (eachline == "---")
                {
                    //std::cout << "Next Model" << std::endl;
                    break;
                }

                Model.Indices.push_back(std::atoi(eachline.c_str()));
            }

            Geometry.emplace(Model.MeshName, std::make_unique<Mesh>(Model));
        }
#endif
    }
}