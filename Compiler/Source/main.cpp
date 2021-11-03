#include "pch.h"
#include "GeometryCompiler.h"
#include "TextureCompiler.h"
#include "PrefabsCompiler.h"
#include "AnimationCompiler.h"

using namespace EclipseCompiler;

int main()
//int WinMain()
{
    CompilerManager Manager;

    std::unique_ptr<TextureCompiler> cTextureCompiler = std::make_unique<TextureCompiler>();
    std::unique_ptr<GeometryCompiler> cGeometryCompiler = std::make_unique<GeometryCompiler>();
    std::unique_ptr<PrefabsCompiler> cPrefabsCompiler = std::make_unique<PrefabsCompiler>();
    std::unique_ptr<AnimationCompiler> cAnimationCompiler = std::make_unique<AnimationCompiler>();

    Manager.Register(*cGeometryCompiler);
    Manager.Register(*cTextureCompiler);
    Manager.Register(*cPrefabsCompiler);
    Manager.Register(*cAnimationCompiler);

    Manager.Initialise();
    cPrefabsCompiler->GetPrefabs(cGeometryCompiler->Prefabs);

    // We will produce our own resource files which will be named .eclipse
    Manager.ProduceFile();
    std::cout << "Compiler Finished Execution" << std::endl;

    return 0;
}

