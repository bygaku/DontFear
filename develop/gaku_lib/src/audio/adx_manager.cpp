#include <cstdlib>
#include <cri_le_xpt.h>
#include <cri_le_atom_ex.h>
#include <cri_le_atom_wasapi.h>
#include "audio/adx_manager.hpp"

namespace audio {

    AdxManager::~AdxManager()
    {
        // 終了
        Terminate();
    }

    bool AdxManager::Initialize() noexcept
    {
        if (is_initialized_) return true;

        try {
            SetupCallbacks();

            // ライブラリ設定
            CriAtomExConfig_WASAPI lib_config{};
            CriFsConfig             fs_config{};

            criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
            criFs_SetDefaultConfig(&fs_config);

            lib_config.atom_ex.max_virtual_voices = AdxConfig::MAX_VIRTUAL_VOICE;
            fs_config.num_loaders                 = AdxConfig::MAX_CRIFS_LOADER;
            lib_config.atom_ex.fs_config          = &fs_config;

            // ADX2初期化
            criAtomEx_Initialize_WASAPI(&lib_config, nullptr, 0);

            is_initialized_ = true;
            return true;
        }
        catch (...) {
            return false;
        }
    }

    bool AdxManager::CreatePlayer() noexcept
    {
        if (!is_initialized_ || player_handle_ != nullptr) {
            return is_initialized_;
        }

        try {
            player_handle_ = criAtomExPlayer_Create(nullptr, nullptr, 0);
            return player_handle_ != nullptr;
        }
        catch (...) {
            return false;
        }
    }

    bool AdxManager::CreateVoicePool() noexcept 
    {
        if (!is_initialized_ || voice_pool_ != nullptr) {
            return is_initialized_;
        }

        try {
            CriAtomExStandardVoicePoolConfig pool_config{};

            criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&pool_config);
            pool_config.num_voices = AdxConfig::MAX_VOICE;

            voice_pool_ = criAtomExVoicePool_AllocateStandardVoicePool(&pool_config, nullptr, 0);
            return voice_pool_ != nullptr;
        }
        catch (...) {
            return false;
        }
    }

    void AdxManager::SetupCallbacks() noexcept
    {
        criErr_SetCallback(ErrorCallback);
        criAtomEx_SetUserAllocator(AllocFunc, FreeFunc, nullptr);
    }

    void AdxManager::Terminate() noexcept
    {
        if (player_handle_ != nullptr) {
            criAtomExPlayer_Destroy(player_handle_);
            player_handle_ = nullptr;
        }

        if (voice_pool_ != nullptr) {
            criAtomExVoicePool_Free(voice_pool_);
            voice_pool_ = nullptr;
        }

        if (is_initialized_) {
            criAtomEx_UnregisterAcf();
            criAtomEx_Finalize_WASAPI();
            is_initialized_ = false;
        }
    }

    void AdxManager::ErrorCallback(const CriChar8* error_id, CriUint32 p1, CriUint32 p2, CriUint32* p_array) 
    {
        /// @brief ログ出力
        const CriChar8* error_message = criErr_ConvertIdToMessage(error_id, p1, p2);
        /// @remark 未使用警告回避
        (void)error_message;
        (void)p_array;
    }

    void* AdxManager::AllocFunc(void* obj, CriUint32 size)
    {
        try {
            return std::malloc(size);
        }
        catch (...) {
            return nullptr;
        }
    }

    void  AdxManager::FreeFunc(void* obj, void* ptr)
    {
        std::free(ptr);
    }

} // namespace audio