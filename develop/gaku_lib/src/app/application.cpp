#include <DxLib.h>
#include "app/application.hpp"
#include "misc/json_loader.hpp"

bool Application::instantiated_ = false;

Application::Application() noexcept
    : is_initialized_(false)
{
    instantiated_ = true;
}

Application::~Application() noexcept
{
    instantiated_ = false;
}

bool Application::Initialize() noexcept
{
    if (is_initialized_)  return true;

    JsonLoader::Create();

    if (!InitializeDx())  return false;
    if (!InitializeAdx()) return false;

    game_time_ = std::make_unique<GameTime>();
    game_time_->SetFpsTarget(60);

    InputManager::Create();

    is_initialized_ = true;
    return true;
}

void Application::Terminate() noexcept
{
    if (!is_initialized_) return;

    InputManager::Destroy();

    if (game_time_) {
        game_time_.reset();
    }

    if (adx_manager_) {
        adx_manager_.reset();
    }


    DxLib_End();

    is_initialized_ = false;
}

bool Application::IsRunning() const noexcept
{
    return !ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE);
}

void Application::Run() noexcept
{
    game_time_->IncrementFrame();
    InputManager::GetInstance().Update();
    ClearDrawScreen();



    Debug();
    game_time_->SleepForNextFrame();
    ScreenFlip();
}

void Application::Debug() const noexcept
{
    printfDx("Game Update!!\n");
    printfDx("Application boot timer: %.2f\n", game_time_->GetBootTimer());
    InputManager::GetInstance().Debug();
}

bool Application::InitializeDx()
{
    auto config_ = WindowConfig{};

    try {
#ifdef _DEBUG
        SetOutApplicationLogValidFlag(TRUE);
#else
        SetOutApplicationLogValidFlag(FALSE);
#endif
        SetWindowText(config_.project_name.c_str());
        ChangeWindowMode(!config_.is_full_screen);
        SetGraphMode(config_.screen_width, config_.screen_height, config_.color_bit);

        if (config_.is_full_screen) {
            int default_width, default_height, default_color_bit;
            GetDefaultState(&default_width, &default_height, &default_color_bit);
            SetWindowSize(default_width, default_height);
        }

        SetUseDirect3DVersion(DX_DIRECT3D_11);
        SetUseDirectInputFlag(false);
        SetUseXInputFlag(true);

        if (DxLib_Init() == -1)  return false;

        SetDrawScreen(DX_SCREEN_BACK);

        SetUseZBuffer3D(true);
        SetWriteZBuffer3D(true);
        SetUseBackCulling(true);

        SetFogEnable(true);
        SetFogMode(DX_FOGMODE_LINEAR);
        SetFogColor(200, 200, 200);
        SetFogStartEnd(10.0f, 100.0f);
        SetFogDensity(0.1f);

        return true;
    }
    catch (...) {
        return false;
    }
}

bool Application::InitializeAdx()
{
    try {
        adx_manager_ = std::make_unique<audio::AdxManager>();

        if (!adx_manager_->Initialize()) {
            adx_manager_.reset();
            return false;
        }

        /// @remark プレイヤーとボイスプール作成
        adx_manager_->CreateVoicePool();
        adx_manager_->CreatePlayer();

        return true;
    }
    catch (...) {
        adx_manager_.reset();
        return false;
    }
}

