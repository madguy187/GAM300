#include "pch.h"
#include "EntryPoint.h"

std::unique_ptr<Eclipse::Engine> engine;

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    static char THIS_FILE[] = __FILE__;
#endif

    engine = std::make_unique<Eclipse::Engine>();
    Eclipse::Log::Init();
    engine->Init();
    engine->Run();

    return 0;
}
