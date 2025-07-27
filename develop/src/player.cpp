#include <DxLib.h>
#include "player.hpp"
#include "component/physics.hpp"
#include "misc/collider/collider_info_capsule.hpp"

Player::Player()
    : CharaObject("Player")
    , jump_initial_power_(8.0f)    // �����͍T���߂�
    , jump_hold_power_(25.0f)      // �z�[���h���̒ǉ���
    , max_jump_time_(0.3f)         // �ő�0.3�b�ԃW�����v�͂�ǉ�
    , current_jump_time_(0.0f)
    , can_jump_(false)
    , is_jumping_(false)
    , jump_input_(false)
    , jump_input_prev_(false)
    , color_(GetColor(0, 255, 0))
{
    tag_ = "Player";
}

void Player::Initialize() noexcept
{
    // �ʒu��ݒ�
    rigidbody_.SetPosition(VGet(0, 2, 0));

    // ���W�b�h�{�f�B�̐ݒ�
    rigidbody_.SetBodyType(Rigidbody::BodyType::kDynamic);
    rigidbody_.SetMass(50.0f);
    rigidbody_.SetUseGravity(true);

    Rigidbody::Constraints constraints;
    constraints.freeze_position_z_ = true;
    constraints.freeze_rotation_x_ = true;
    constraints.freeze_rotation_y_ = true;
    constraints.freeze_rotation_z_ = true;
    rigidbody_.SetConstraints(constraints);

    // �J�v�Z���R���C�_�[�̐ݒ�
    auto capsule = std::dynamic_pointer_cast<ColliderInfoCapsule>(GetColliderInfo());
    if (capsule) {
        // �v���C���[�̃J�v�Z���i����1.0�A���a0.3�j
        capsule->SetCapsule(
            VGet(0, 0.3f, 0),   // ��i���a���グ��j
            VGet(0, 0.7f, 0),   // ���_
            0.3f                // ���a
        );
    }

    // �A�N�e�B�u��
    Activate();
}

void Player::Update() noexcept
{
    if (!IsActive()) return;

    // ���͏���
    UpdateInput();

    // �W�����v����
    HandleJump();
}

void Player::Draw() const noexcept
{
    if (!IsActive()) return;

    VECTOR pos = rigidbody_.GetPosition();

    // �J�v�Z���ŕ`��i�ȈՔŁj
    DrawCapsule3D(
        VAdd(pos, VGet(0, 0.3f, 0)),  // ��
        VAdd(pos, VGet(0, 0.7f, 0)),  // ���_
        0.3f,                          // ���a
        8,
        color_,
        GetColor(100, 255, 100),
        TRUE
    );

    // ���`��i�O�ʁj
    DrawSphere3D(VAdd(pos, VGet(0, 0.8f, -0.2f)), 0.1f, 8,
        GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);

    // �W�����v���̓G�t�F�N�g��ǉ�
    if (is_jumping_) {
    }
}

void Player::LastUpdate() noexcept
{
    // ����͓��ɏ����Ȃ�
}

void Player::OnCollide(const CollidableObject& object) noexcept
{
    // ���Ƃ̏Փ�
    if (object.GetTag() == "Stage" || object.GetTag() == "Floor") {
        // �n�ʂɒ��n
        can_jump_ = true;
        rigidbody_.SetGrounded(true);

        // �������̑��x�����Z�b�g
        VECTOR vel = rigidbody_.GetVelocity();
        if (vel.y < 0) {
            vel.y = 0;
            rigidbody_.SetVelocity(vel);
        }
    }
}

void Player::UpdateInput()
{
    // A�{�^���i�L�[�{�[�h�ł�Z�L�[�j�ŃW�����v
    int pad_input = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    jump_input_ = (pad_input & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_Z);
}

void Player::HandleJump()
{
    float delta_time = 0.016f;  // ��60FPS�z��

    // �W�����v�J�n�i�{�^���������ꂽ�u�ԁj
    if (jump_input_ && !jump_input_prev_ && can_jump_) {
        // ������^����
        VECTOR vel = rigidbody_.GetVelocity();
        vel.y = jump_initial_power_;
        rigidbody_.SetVelocity(vel);

        can_jump_ = false;
        is_jumping_ = true;
        current_jump_time_ = 0.0f;
        rigidbody_.SetGrounded(false);

        // �W�����v�J�n���̐F�ύX
        color_ = GetColor(0, 255, 255);
    }

    // �W�����v���̒ǉ��́i�{�^�������������Ă���ԁj
    if (is_jumping_ && jump_input_ && current_jump_time_ < max_jump_time_) {
        // ������̒ǉ��͂�������
        VECTOR vel = rigidbody_.GetVelocity();
        vel.y += jump_hold_power_ * delta_time;
        rigidbody_.SetVelocity(vel);

        current_jump_time_ += delta_time;

        // �W�����v�̍����ɉ����ĐF��ς���
        float jump_ratio = current_jump_time_ / max_jump_time_;
        int green = static_cast<int>(255 * (1.0f - jump_ratio * 0.5f));
        color_ = GetColor(0, green, 255);
    }

    // �{�^���𗣂�����W�����v�I��
    if (!jump_input_ && is_jumping_) {
        is_jumping_ = false;

        // �㏸���Ȃ�㏸����߂�
        VECTOR vel = rigidbody_.GetVelocity();
        if (vel.y > 0) {
            vel.y *= 0.5f;  // �㏸���x�𔼌�
            rigidbody_.SetVelocity(vel);
        }
    }

    // ���~���̓W�����v�I��
    if (is_jumping_ && rigidbody_.GetVelocity().y < 0) {
        is_jumping_ = false;
    }

    // �F�̍X�V
    if (!rigidbody_.IsGrounded()) {
        if (!is_jumping_) {
            color_ = GetColor(0, 150, 150);  // ������
        }
    }
    else {
        color_ = GetColor(0, 255, 0);  // �ڒn��
    }

    // �O�t���[���̓��͂�ۑ�
    jump_input_prev_ = jump_input_;
}