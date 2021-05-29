#include "pch.h"
#include "EntryPoint.h"

std::unique_ptr<Eclipse::Engine> engine;

int main()
{
	engine = std::make_unique<Eclipse::Engine>();
	Eclipse::Log::Init();
	ENGINE_CORE_INFO("Logger Test: Hello World!");
	engine->Init();
	engine->Run();

	return 0;
}
