#pragma once
#include <memory>
#include <cstddef>
#include <cri_adx2le.h>

namespace audio {

    /// @brief �ő�{�C�X��
    struct AdxConfig 
    {
        static constexpr int MAX_VOICE         = 24;
        static constexpr int MAX_VIRTUAL_VOICE = 64;
        static constexpr int MAX_CRIFS_LOADER  = 64;
    };

    /// @brief CRI_ADX2�Ǘ�
    class AdxManager 
    {
    public:
        /// @brief �C���X�^���X��������
        AdxManager()
            : player_handle_(nullptr)
            , voice_pool_(nullptr)
            , is_initialized_(false)
        {
        };

        /// @brief �C���X�^���X�����
        ~AdxManager();

        /// @remark �R�s�[�֎~�A���[�u�\
        AdxManager(const AdxManager&)                = delete;
        AdxManager& operator=(const AdxManager&)     = delete;
        AdxManager(AdxManager&&)            noexcept = default;
        AdxManager& operator=(AdxManager&&) noexcept = default;

        /// @brief ADX2_SYSTEM�̏�����
        bool Initialize()      noexcept;

        /// @brief �v���C���[�̍쐬
        bool CreatePlayer()    noexcept;

        /// @brief �{�C�X�v�[���̍쐬
        bool CreateVoicePool() noexcept;

        /// @brief ��������Ԃ̊m�F
        [[nodiscard]] bool IsInitialized()          const noexcept { return is_initialized_; }

        /// @brief �v���C���[�n���h���̎擾
        [[nodiscard]] CriAtomExPlayerHn GetPlayer() const noexcept { return player_handle_;  }

    private:
        /// @brief �R�[���o�b�N��ݒ�
        void SetupCallbacks() noexcept;

        /// @brief �I���Ăяo��
        void Terminate()      noexcept;

        /// @brief �G���[�R�[���o�b�N
        static void ErrorCallback(const CriChar8* error_id, CriUint32 p1, CriUint32 p2, CriUint32* p_array);

        /// @brief �������m��
        static void* AllocFunc(void* obj, CriUint32 size);

        /// @brief ���������
        static void  FreeFunc(void* obj, void* ptr);

    private:
        CriAtomExPlayerHn    player_handle_;    /// @brief �v���C���[�n���h��
        CriAtomExVoicePoolHn voice_pool_;       /// @brief �{�C�X�v�[��

    private:
        bool is_initialized_;

    };

} // namespace audio