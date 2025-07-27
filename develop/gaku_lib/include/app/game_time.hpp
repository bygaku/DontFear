#pragma once
#include <memory>

class GameTime final
{
public:	
    /// @brief �C���X�^���X��������
    GameTime()  noexcept;

    /// @brief �C���X�^���X�����
    ~GameTime();

    /// @brief �t���[���J�E���g�� +1
    /// @remark ���t���[�������O�ɌĂяo��
    void IncrementFrame()                             noexcept;

    /// @brief �Œ�t���[���̍X�V�񐔂��X�V���ĕԂ�
    int UpdateFixedFrameCount()                       noexcept;

    /// @brief ���݂�1�t���[���������� (seconds)
    [[nodiscard]] double GetCurrentFrameTime()  const noexcept;

    /// @brief fps_target_ �Ƃ��Ďw�肳��Ă��鎞�Ԃ܂ő҂�
    void SleepForNextFrame()                    const noexcept;

    /// @brief fps �������w��
    void SetFpsTarget(int target)                     noexcept;

    /// @brief �^�C���X�P�[����ݒ�
    static void SetTimeScale(const float& scale)      noexcept;

    /// @brief �^�C���X�P�[�����擾�F�ʏ�{���� 1.f
    static const float& GetTimeScale()                noexcept { return time_scale_; }

    /// @brief 2�O��1�O�̃t���[���Ԃ̎��ԍ� (seconds)
    /// @brief time_scaler_ �ɂ�郊�X�P�[���ɒ���
    static const float& GetDeltaTime()                noexcept { return delta_time_; }

    /// @brief �������ԂɊ�Â��� delta_time_���擾
    /// @brief time_scaler_ �̉e�����󂯂Ȃ�
    static const float& GetUnscaledDeltaTime()        noexcept { return unscaled_delta_time_; }

    /// @brief �N������̎��Ԃ��擾
    /// @brief time_scaler_ �̉e�����󂯂Ȃ�
    const float& GetBootTimer()                       noexcept;

    /// @brief �J�E���g�A�b�v�^�C�}�[���擾
    /// @brief time_scaler_ �̉e�����󂯂Ȃ�
    const float& GetUnscaledTimer()                   noexcept;

    /// @brief �J�E���g�A�b�v�^�C�}�[���擾
    const float& GetTimer()                           noexcept;

private:
#pragma region private���\�b�h
    /// @brief ���݂̃t���[���̎���
    [[nodiscard]] const LONGLONG& GetTime()                 const noexcept { return time_; }

    /// @brief �Ō�� fps ���X�V�������� (nanoseconds)
    [[nodiscard]] const LONGLONG& GetLastCheckedTime()      const noexcept { return fps_check_time_; }

    /// @brief �N������̎��� (seconds)
    [[nodiscard]] const float&    GetTimeFromBoot()         const noexcept { return time_since_start_up_; }

    /// @brief ���݂� frames per second
    [[nodiscard]] const int&      GetFps()                  const noexcept { return fps_; };

    /// @brief �Ō�� fps �X�V����̃t���[���J�E���g
    [[nodiscard]] const int&      GetFpsCounter()           const noexcept { return fps_counter_; };

    /// @brief ���݂� fps ����
    [[nodiscard]] const int&      GetFpsTarget()            const noexcept { return fps_target_; };

    /// @brief ���݂� fps �����ɂ��t���[���Ԃ̕b�� (seconds)
    [[nodiscard]] const double&   GetSecondsPerFrame()      const noexcept { return seconds_per_frame_; };

    /// @brief �Ō�̌Œ�Ԋu�X�V�̉�
    [[nodiscard]] const int&      GetLastFixedFrameCount()  const noexcept { return last_fixed_frame_count_; };

    /// @brief �N�����Ă���̃t���[����
    [[nodiscard]] const UINT&     GetFrames()               const noexcept { return frames_; };
#pragma endregion

private:
    static bool  instantiated_;
    static float delta_time_;
    static float unscaled_delta_time_;
    static float time_scale_;

private:
    LONGLONG time_;
    LONGLONG fps_check_time_;
    LONGLONG fixed_update_check_time_;
    float    time_since_start_up_;
             
    UINT     frames_;
    int      fps_;
    int      fps_target_;
    int      fps_counter_;
    double   seconds_per_frame_;
    int      last_fixed_frame_count_;

    float    boot_timer_;
    float    timer_;
    float    unscaled_timer_;
};