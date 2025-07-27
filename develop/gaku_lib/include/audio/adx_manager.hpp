#pragma once
#include <memory>
#include <cstddef>
#include <cri_adx2le.h>

namespace audio {

    /// @brief 最大ボイス数
    struct AdxConfig 
    {
        static constexpr int MAX_VOICE         = 24;
        static constexpr int MAX_VIRTUAL_VOICE = 64;
        static constexpr int MAX_CRIFS_LOADER  = 64;
    };

    /// @brief CRI_ADX2管理
    class AdxManager 
    {
    public:
        /// @brief インスタンスを初期化
        AdxManager()
            : player_handle_(nullptr)
            , voice_pool_(nullptr)
            , is_initialized_(false)
        {
        };

        /// @brief インスタンスを解放
        ~AdxManager();

        /// @remark コピー禁止、ムーブ可能
        AdxManager(const AdxManager&)                = delete;
        AdxManager& operator=(const AdxManager&)     = delete;
        AdxManager(AdxManager&&)            noexcept = default;
        AdxManager& operator=(AdxManager&&) noexcept = default;

        /// @brief ADX2_SYSTEMの初期化
        bool Initialize()      noexcept;

        /// @brief プレイヤーの作成
        bool CreatePlayer()    noexcept;

        /// @brief ボイスプールの作成
        bool CreateVoicePool() noexcept;

        /// @brief 初期化状態の確認
        [[nodiscard]] bool IsInitialized()          const noexcept { return is_initialized_; }

        /// @brief プレイヤーハンドルの取得
        [[nodiscard]] CriAtomExPlayerHn GetPlayer() const noexcept { return player_handle_;  }

    private:
        /// @brief コールバックを設定
        void SetupCallbacks() noexcept;

        /// @brief 終了呼び出し
        void Terminate()      noexcept;

        /// @brief エラーコールバック
        static void ErrorCallback(const CriChar8* error_id, CriUint32 p1, CriUint32 p2, CriUint32* p_array);

        /// @brief メモリ確保
        static void* AllocFunc(void* obj, CriUint32 size);

        /// @brief メモリ解放
        static void  FreeFunc(void* obj, void* ptr);

    private:
        CriAtomExPlayerHn    player_handle_;    /// @brief プレイヤーハンドル
        CriAtomExVoicePoolHn voice_pool_;       /// @brief ボイスプール

    private:
        bool is_initialized_;

    };

} // namespace audio