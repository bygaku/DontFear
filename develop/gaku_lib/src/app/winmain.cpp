#include <DxLib.h>
#include "app/application.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    auto app = std::make_shared<Application>();

    if (!app->Initialize()) {
        return -1;
    }

    while (app->IsRunning()) {
        clsDx();
        app->Run();
    }

    app->Terminate();

    return 0;
}