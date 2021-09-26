#include "pch.h"
#include "GeometryCompiler.h"
#include "TextureCompiler.h"

using namespace EclipseCompiler;

int main()
{
    CompilerManager Manager;

    std::unique_ptr<GeometryCompiler> cGeometryCompiler = std::make_unique<GeometryCompiler>();
    std::unique_ptr<TextureCompiler> cTextureCompiler = std::make_unique<TextureCompiler>();

    Manager.Register(*cGeometryCompiler);
    Manager.Register(*cTextureCompiler);

    Manager.Initialise();

    while (1)
    {
        std::string Check;
        std::cout << "Please Select one of the below : " << std::endl;
        std::cout << "[1] : Geometry " << std::endl;
        std::cout << "[2] : Texture " << std::endl;
        std::cout << "[3] : Exit " << std::endl << std::endl << std::endl;
        std::cin >> Check;

        Manager.ProduceFile(Check);

        if (Check == "3")
        {
            break;
        }
    }

    return 0;
}

