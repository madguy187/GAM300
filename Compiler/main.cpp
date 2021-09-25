#include "pch.h"
#include "GeometryCompiler.h"

using namespace EclipseCompiler;

int main()
{
    std::unique_ptr<GeometryCompiler> cGeometryCompiler = std::make_unique<GeometryCompiler>();

    cGeometryCompiler->Init();

    while (1)
    {
        std::string Check;
        std::cout << "Please Select one of the below : " << std::endl;
        std::cout << "[1] : Geometry " << std::endl;
        std::cout << "[2] : Texture " << std::endl;
        std::cout << "[3] : Exit " << std::endl << std::endl << std::endl;
        std::cin >> Check;

        cGeometryCompiler->ReleaseFile(Check);

        if (Check == "3")
        {
            break;
        }
    }

    return 0;
}

