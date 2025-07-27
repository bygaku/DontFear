#pragma once
#include <array>
#include <memory>
#include <unordered_map>
#include <string>
#include <optional>
#include "setting/input_data.hpp"
#include "misc/math/math.hpp"

class InputManager final
{
public:
    static InputManager& GetInstance() noexcept;
    static void          Create()	   noexcept;
    static void          Destroy()	   noexcept;

    void Update() noexcept;
    void Debug(int x = 10, int y = 10) const noexcept;

    /// @remark �L�[�{�[�h�֘A
    [[nodiscard]] InputState GetKeyState  (KeyCode key) const noexcept;
    [[nodiscard]] bool       IsKeyPressed (KeyCode key) const noexcept;
    [[nodiscard]] bool       IsKeyHeld    (KeyCode key) const noexcept;
    [[nodiscard]] bool       IsKeyReleased(KeyCode key) const noexcept;

    /// @remark �}�E�X�֘A
    [[nodiscard]] InputState          GetMouseButtonState  (MouseButton button) const noexcept;
    [[nodiscard]] bool                IsMouseButtonPressed (MouseButton button) const noexcept;
    [[nodiscard]] bool                IsMouseButtonHeld    (MouseButton button) const noexcept;
    [[nodiscard]] bool                IsMouseButtonReleased(MouseButton button) const noexcept;
    [[nodiscard]] dx_math::Vector2D   GetMousePosition()                        const noexcept;
    [[nodiscard]] dx_math::Vector2D   GetMouseDelta()                           const noexcept;     /// @brief �O�t���[������̌������擾
    [[nodiscard]] dx_math::Vector2D   GetMouseDeltaFromCenter()                 const noexcept;     /// @brief ��ʒ��S���W����̌������擾
    [[nodiscard]] int                 GetMouseWheelDelta()                      const noexcept;     
    void                              ResetMousePoint()                         const noexcept;     /// @brief �}�E�X���W�𖈃t���[����ʒ��S�ɖ߂�

    /// @remark �Q�[���p�b�h�֘A
    [[nodiscard]] bool       IsPadConnected     (int player_index = 0)                   const noexcept;
    [[nodiscard]] InputState GetPadButtonState  (PadButton button, int player_index = 0) const noexcept;
    [[nodiscard]] bool       IsPadButtonPressed (PadButton button, int player_index = 0) const noexcept;
    [[nodiscard]] bool       IsPadButtonHeld    (PadButton button, int player_index = 0) const noexcept;
    [[nodiscard]] bool       IsPadButtonReleased(PadButton button, int player_index = 0) const noexcept;
    void                     SortConnectedPads()                                               noexcept;

    /// @remark �X�e�B�b�N���́i-1.0f�`1.0f�͈̔́j
    [[nodiscard]] float             GetPadStickValue(PadStick stick, int player_index = 0) const noexcept;
    [[nodiscard]] dx_math::Vector2D GetPadLStick(int player_index = 0)                     const noexcept;
    [[nodiscard]] dx_math::Vector2D GetPadRStick(int player_index = 0)                     const noexcept;

    /// @remark �g���K�[���́i0.0f�`1.0f�͈̔́j
    [[nodiscard]] float GetPadLTrigger(int player_index = 0) const noexcept;
    [[nodiscard]] float GetPadRTrigger(int player_index = 0) const noexcept;

    /// @remark �o�C�u���[�V��������
    void SetPadVibration (int player_index, float left_motor, float right_motor, float duration = 0.0f) noexcept;
    void StopPadVibration(int player_index)                                                             noexcept;

private:
    static InputManager* instance_;

    InputManager();
    ~InputManager() = default;

    [[nodiscard]] InputState CalculateInputState   (bool current, bool prev) const noexcept;
    [[nodiscard]] float      NormalizeStickValue   (short value)             const noexcept;
    [[nodiscard]] float      NormalizeTriggerValue (UCHAR value)             const noexcept;
    void                     UpdatePadVibration    (float delta_time)              noexcept;

private:
    /// @remark �֎~
    InputManager(const InputManager&)            = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&)                 = delete;
    InputManager& operator=(InputManager&&)      = delete;

private:
    static constexpr int    kKeyAll              =    256;  /// @brief �L�[�{�[�h��
    static constexpr int    kMouseButtonAll      =      3;  /// @brief �}�E�X�{�^����
    static constexpr int    kMaxGamepads         =      4;  /// @brief �ő哯���ڑ�
    static constexpr double kStickDeadZone       =    0.2;  /// @brief �f�b�h�]�[��

    /// @remark �L�[�{�[�h�֘A
    std::array<bool, kKeyAll> current_key_state_ {};
    std::array<bool, kKeyAll> prev_key_state_    {};

    /// @remark �}�E�X�֘A
    std::array<bool, kMouseButtonAll> current_mouse_state_{};
    std::array<bool, kMouseButtonAll> prev_mouse_state_   {};
    dx_math::Vector2D                 mouse_position_     {};
    dx_math::Vector2D                 prev_mouse_position_{};
    int                               mouse_wheel_delta_ = 0;

    /// @remark �p�b�h�֘A 
    struct PadState
    {
        XINPUT_STATE     current_state_           {};
        XINPUT_STATE     prev_state_              {};
        bool             connected_          = false;
        float            vibration_duration_ =  0.0f;
    };

    std::array<PadState, kMaxGamepads> gamepads_ {};

};