#pragma once
#include "object/character_object.hpp"

/// @brief プレイヤー
class Player : public CharaObject
{
public:
    /// @brief コンストラクタ
    Player();

    /// @brief デストラクタ
    ~Player() override = default;

    /// @brief 初期化
    void Initialize() noexcept override;

    /// @brief 更新
    void Update() noexcept override;

    /// @brief 描画
    void Draw() const noexcept override;

    /// @brief 最終更新
    void LastUpdate() noexcept override;

    /// @brief 衝突判定
    void OnCollide(const CollidableObject& object) noexcept override;

private:
    /// @brief ジャンプ処理
    void HandleJump();

    /// @brief 入力処理
    void UpdateInput();

public:
    // デバッグ表示用（本来はprivateだが簡易実装のため）
    bool is_jumping_;            /// @brief ジャンプ中か
    float current_jump_time_;    /// @brief 現在のジャンプ時間
    float max_jump_time_;        /// @brief 最大ジャンプ時間

private:
    // ジャンプパラメータ
    float jump_initial_power_;    /// @brief ジャンプ初速
    float jump_hold_power_;       /// @brief ホールド時の追加力
    float max_jump_time_;         /// @brief 最大ジャンプ時間
    float current_jump_time_;     /// @brief 現在のジャンプ時間
    bool can_jump_;              /// @brief ジャンプ可能か
    bool is_jumping_;            /// @brief ジャンプ中か
    bool jump_input_;            /// @brief ジャンプ入力（現在）
    bool jump_input_prev_;       /// @brief ジャンプ入力（前フレーム）

    // 描画用
    unsigned int color_;         /// @brief プレイヤーの色
};