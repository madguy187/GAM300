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

    // We will produce our own resource files which will be named .eclipse
    Manager.ProduceFile();

    while (1)
    {
        std::string Test;
        std::cin >> Test;

        if (Test == "1")
        {
            return 0;
        }
    }

    return 0;
}

