#include "pch.h"
#include "Compilers.h"
#include "CompilerSerializer.h"

using namespace EclipseCompiler;

std::unique_ptr<Compilers> AllCompilers;

int main()
{
    AllCompilers = std::make_unique<Compilers>();

    AllCompilers->Loader.LoadModels("..//Eclipse//src//Assets//ASSModels");

    for (auto& i : AllCompilers->Loader.ModelMap)
    {
        std::cout << i.first << std::endl;
    }

	std::string mystring{ "Hoopa" };
	Serializer sz;

	sz.StartElement("HelloWorld");
	for (size_t i = 0; i < 10; ++i)
	{
		sz.StartElement("A", true, i);
		sz.AddAttributeToElement("int", 0);
		sz.AddAttributeToElement("string", mystring);
		sz.AddAttributeToElement("double", 1.0);
		sz.AddAttributeToElement("float", 1.0f);
		sz.AddAttributeToElement("size_t", i);
		sz.AddAttributeToElement("char", 'A');
		sz.AddAttributeToElement("bool", true);
		sz.CloseElement();
	}
	sz.CloseElement();

	sz.StartElement("ByeWorld");
	sz.CloseElement();
	sz.SaveXML("TesatCompiler\\Test.xml");

    return 0;
}
