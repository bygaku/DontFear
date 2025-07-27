#include <DxLib.h>
#include "player.hpp"
#include "component/physics.hpp"
#include "misc/collider/collider_info_capsule.hpp"

Player::Player()
    : CharaObject("Player")
    , jump_initial_power_(8.0f)    // 初速は控えめに
    , jump_hold_power_(25.0f)      // ホールド時の追加力
    , max_jump_time_(0.3f)         // 最大0.3秒間ジャンプ力を追加
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
    // 位置を設定
    rigidbody_.SetPosition(VGet(0, 2, 0));

    // リジッドボディの設定
    rigidbody_.SetBodyType(Rigidbody::BodyType::kDynamic);
    rigidbody_.SetMass(50.0f);
    rigidbody_.SetUseGravity(true);

    Rigidbody::Constraints constraints;
    constraints.freeze_position_z_ = true;
    constraints.freeze_rotation_x_ = true;
    constraints.freeze_rotation_y_ = true;
    constraints.freeze_rotation_z_ = true;
    rigidbody_.SetConstraints(constraints);

    // カプセルコライダーの設定
    auto capsule = std::dynamic_pointer_cast<ColliderInfoCapsule>(GetColliderInfo());
    if (capsule) {
        // プレイヤーのカプセル（高さ1.0、半径0.3）
        capsule->SetCapsule(
            VGet(0, 0.3f, 0),   // 底（半径分上げる）
            VGet(0, 0.7f, 0),   // 頂点
            0.3f                // 半径
        );
    }

    // アクティブ化
    Activate();
}

void Player::Update() noexcept
{
    if (!IsActive()) return;

    // 入力処理
    UpdateInput();

    // ジャンプ処理
    HandleJump();
}

void Player::Draw() const noexcept
{
    if (!IsActive()) return;

    VECTOR pos = rigidbody_.GetPosition();

    // カプセルで描画（簡易版）
    DrawCapsule3D(
        VAdd(pos, VGet(0, 0.3f, 0)),  // 底
        VAdd(pos, VGet(0, 0.7f, 0)),  // 頂点
        0.3f,                          // 半径
        8,
        color_,
        GetColor(100, 255, 100),
        TRUE
    );

    // 顔を描画（前面）
    DrawSphere3D(VAdd(pos, VGet(0, 0.8f, -0.2f)), 0.1f, 8,
        GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);

    // ジャンプ中はエフェクトを追加
    if (is_jumping_) {
    }
}

void Player::LastUpdate() noexcept
{
    // 今回は特に処理なし
}

void Player::OnCollide(const CollidableObject& object) noexcept
{
    // 床との衝突
    if (object.GetTag() == "Stage" || object.GetTag() == "Floor") {
        // 地面に着地
        can_jump_ = true;
        rigidbody_.SetGrounded(true);

        // 下向きの速度をリセット
        VECTOR vel = rigidbody_.GetVelocity();
        if (vel.y < 0) {
            vel.y = 0;
            rigidbody_.SetVelocity(vel);
        }
    }
}

void Player::UpdateInput()
{
    // Aボタン（キーボードではZキー）でジャンプ
    int pad_input = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    jump_input_ = (pad_input & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_Z);
}

void Player::HandleJump()
{
    float delta_time = 0.016f;  // 約60FPS想定

    // ジャンプ開始（ボタンが押された瞬間）
    if (jump_input_ && !jump_input_prev_ && can_jump_) {
        // 初速を与える
        VECTOR vel = rigidbody_.GetVelocity();
        vel.y = jump_initial_power_;
        rigidbody_.SetVelocity(vel);

        can_jump_ = false;
        is_jumping_ = true;
        current_jump_time_ = 0.0f;
        rigidbody_.SetGrounded(false);

        // ジャンプ開始時の色変更
        color_ = GetColor(0, 255, 255);
    }

    // ジャンプ中の追加力（ボタンを押し続けている間）
    if (is_jumping_ && jump_input_ && current_jump_time_ < max_jump_time_) {
        // 上向きの追加力を加える
        VECTOR vel = rigidbody_.GetVelocity();
        vel.y += jump_hold_power_ * delta_time;
        rigidbody_.SetVelocity(vel);

        current_jump_time_ += delta_time;

        // ジャンプの高さに応じて色を変える
        float jump_ratio = current_jump_time_ / max_jump_time_;
        int green = static_cast<int>(255 * (1.0f - jump_ratio * 0.5f));
        color_ = GetColor(0, green, 255);
    }

    // ボタンを離したらジャンプ終了
    if (!jump_input_ && is_jumping_) {
        is_jumping_ = false;

        // 上昇中なら上昇を弱める
        VECTOR vel = rigidbody_.GetVelocity();
        if (vel.y > 0) {
            vel.y *= 0.5f;  // 上昇速度を半減
            rigidbody_.SetVelocity(vel);
        }
    }

    // 下降中はジャンプ終了
    if (is_jumping_ && rigidbody_.GetVelocity().y < 0) {
        is_jumping_ = false;
    }

    // 色の更新
    if (!rigidbody_.IsGrounded()) {
        if (!is_jumping_) {
            color_ = GetColor(0, 150, 150);  // 落下中
        }
    }
    else {
        color_ = GetColor(0, 255, 0);  // 接地中
    }

    // 前フレームの入力を保存
    jump_input_prev_ = jump_input_;
}