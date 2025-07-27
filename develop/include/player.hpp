#pragma once
#include "object/character_object.hpp"

/// @brief �v���C���[
class Player : public CharaObject
{
public:
    /// @brief �R���X�g���N�^
    Player();

    /// @brief �f�X�g���N�^
    ~Player() override = default;

    /// @brief ������
    void Initialize() noexcept override;

    /// @brief �X�V
    void Update() noexcept override;

    /// @brief �`��
    void Draw() const noexcept override;

    /// @brief �ŏI�X�V
    void LastUpdate() noexcept override;

    /// @brief �Փ˔���
    void OnCollide(const CollidableObject& object) noexcept override;

private:
    /// @brief �W�����v����
    void HandleJump();

    /// @brief ���͏���
    void UpdateInput();

public:
    // �f�o�b�O�\���p�i�{����private�����ȈՎ����̂��߁j
    bool is_jumping_;            /// @brief �W�����v����
    float current_jump_time_;    /// @brief ���݂̃W�����v����
    float max_jump_time_;        /// @brief �ő�W�����v����

private:
    // �W�����v�p�����[�^
    float jump_initial_power_;    /// @brief �W�����v����
    float jump_hold_power_;       /// @brief �z�[���h���̒ǉ���
    float max_jump_time_;         /// @brief �ő�W�����v����
    float current_jump_time_;     /// @brief ���݂̃W�����v����
    bool can_jump_;              /// @brief �W�����v�\��
    bool is_jumping_;            /// @brief �W�����v����
    bool jump_input_;            /// @brief �W�����v���́i���݁j
    bool jump_input_prev_;       /// @brief �W�����v���́i�O�t���[���j

    // �`��p
    unsigned int color_;         /// @brief �v���C���[�̐F
};