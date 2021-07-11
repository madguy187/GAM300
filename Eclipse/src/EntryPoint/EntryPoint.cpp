#include "pch.h"
#include "EntryPoint.h"

std::unique_ptr<Eclipse::Engine> engine;

//int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
int main()
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    /*CoInitialize(0);
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)hInstance;
    (void)nCmdShow;*/
    engine = std::make_unique<Eclipse::Engine>();
    Eclipse::Log::Init();
    engine->Init();
    engine->Run();
    /*CoUninitialize();*/

    return 0;
}
