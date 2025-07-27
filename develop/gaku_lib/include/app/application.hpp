#pragma once
#include <memory>
#include <unordered_map>
#include <string>

///< ツール・ライブラリ管理
#include "audio/adx_manager.hpp"

///< コンフィグ・マネージャ
#include "setting/window_config.hpp"
#include "app/game_time.hpp"
#include "app/input_manager.hpp"

/// @brief アプリケーション全体の管理
class Application final
{
public:
    /// @brief インスタンスを初期化
    Application()  noexcept;

    /// @brief インスタンスを解放
    ~Application() noexcept;

    /// @brief アプリケーションの初期化
    bool Initialize()      noexcept;
                              
    /// @brief アプリケーションの終了
    void Terminate()       noexcept;

    /// @brief アプリケーションの更新
    void Run()			   noexcept;

    /// @brief 更新を続けるか
    bool IsRunning() const noexcept;
private:
#ifdef _DEBUG
    /// @brief デバッグ表示
    void Debug()     const noexcept;
#endif
    /// @brief DXLIBの初期化
    bool InitializeDx();

    /// @brief ADXの初期化
    bool InitializeAdx();

private:
    bool         is_initialized_;
    static bool  instantiated_;

private:
    std::unique_ptr<audio::AdxManager> adx_manager_;
    std::unique_ptr<GameTime>          game_time_;

};
