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

    std::cout << "======================================================" << std::endl << std::endl;
    // We will produce our own resource files which will be named .eclipse
    Manager.ProduceFile();
    std::cout << "======================================================" << std::endl;

    return 0;
}

