#include "pch.h"
#include "GeometryCompiler.h"
#include "Compiler/Texture/TextureCompiler.h"
#include "Compiler/Prefabs/PrefabsCompiler.h"

using namespace EclipseCompiler;

int main()
{
    CompilerManager Manager;

    std::unique_ptr<TextureCompiler> cTextureCompiler = std::make_unique<TextureCompiler>();
    std::unique_ptr<GeometryCompiler> cGeometryCompiler = std::make_unique<GeometryCompiler>();
    std::unique_ptr<PrefabsCompiler> cPrefabsCompiler = std::make_unique<PrefabsCompiler>();

    Manager.Register(*cGeometryCompiler);
    Manager.Register(*cTextureCompiler);
    Manager.Register(*cPrefabsCompiler);

    Manager.Initialise();
    cPrefabsCompiler->GetPrefabs(cGeometryCompiler->Prefabs);

    while (1)
    {
        std::string Check;
        std::cout << "Please Select one of the below : " << std::endl;
        std::cout << "[1] : Geometry " << std::endl;
        std::cout << "[2] : Texture " << std::endl;
        std::cout << "[3] : Prefabs " << std::endl;
        std::cout << "[Geo] : Test Read Geometry " << std::endl;
        std::cout << "[Tex] : Test Read Textures " << std::endl;
        std::cout << "[Pre] : Test Read Prefabs " << std::endl;
        std::cout << "[4] : Exit " << std::endl << std::endl << std::endl;
        std::cin >> Check;

        Manager.ProduceFile(Check);
        Manager.ReadFile(Check);

        if (Check == "4")
        {
            break;
        }
    }

    return 0;
}

