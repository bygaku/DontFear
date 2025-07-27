#pragma once
#include <memory>
#include <unordered_map>
#include <string>

///< �c�[���E���C�u�����Ǘ�
#include "audio/adx_manager.hpp"

///< �R���t�B�O�E�}�l�[�W��
#include "setting/window_config.hpp"
#include "app/game_time.hpp"
#include "app/input_manager.hpp"

/// @brief �A�v���P�[�V�����S�̂̊Ǘ�
class Application final
{
public:
    /// @brief �C���X�^���X��������
    Application()  noexcept;

    /// @brief �C���X�^���X�����
    ~Application() noexcept;

    /// @brief �A�v���P�[�V�����̏�����
    bool Initialize()      noexcept;
                              
    /// @brief �A�v���P�[�V�����̏I��
    void Terminate()       noexcept;

    /// @brief �A�v���P�[�V�����̍X�V
    void Run()			   noexcept;

    /// @brief �X�V�𑱂��邩
    bool IsRunning() const noexcept;
private:
#ifdef _DEBUG
    /// @brief �f�o�b�O�\��
    void Debug()     const noexcept;
#endif
    /// @brief DXLIB�̏�����
    bool InitializeDx();

    /// @brief ADX�̏�����
    bool InitializeAdx();

private:
    bool         is_initialized_;
    static bool  instantiated_;

private:
    std::unique_ptr<audio::AdxManager> adx_manager_;
    std::unique_ptr<GameTime>          game_time_;

};
