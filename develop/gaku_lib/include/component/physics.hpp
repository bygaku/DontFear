#pragma once
#include <vector>
#include "object/collidable_object.hpp"

/// @brief 物理・衝突判定するオブジェクトを登録、物理移動・衝突を通知
class Physics final
{
public:
    /// @brief インスタンスを取得
    static Physics& GetInstance();

    /// @brief インスタンスを作成
    static void Create();
    
    /// @brief インスタンスを解放
    static void Destroy();

    /// @brief 衝突オブジェクトの登録
    static void Entry(CollidableObject* collidable);

    /// @brief 衝突オブジェクトの解除
    static void Exit(CollidableObject* collidable);

    /// @brief 登録オブジェクトの衝突通知、物理移動
    static void Update() noexcept;
    
public:
    static constexpr float kGravity         = -0.01f;   /// @brief 重力
    static constexpr float kMaxGravityAccel = -0.15f;   /// @brief 最大重力加速度

private:
    /// @brief インスタンスを初期化
    Physics();

    /// @brief インスタンスを解放
    ~Physics();

private:
    std::vector<CollidableObject*> collidables_;        /// @brief 登録された Object の配列
    std::vector<CollidableObject*> static_objects_;     /// @brief 静的オブジェクト
    std::vector<CollidableObject*> dynamic_objects_;    /// @brief 動的オブジェクト

private:
    static const int kBeforeFixInfoColor    = 0x0000ff;	/// @brief 補正前情報色
    static const int kAimInfoColor          = 0x0000aa;	/// @brief 補正前予定情報色
    static const int kAfterFixInfoColor     = 0xff00ff;	/// @brief 補正後情報色
    static const int kMaxHitCheck           = 2000;     /// @brief 最大判定回数


    // OnCollide の遅延通知のためのデータ
    struct OnCollideInfo final
    {
        CollidableObject* owner;
        CollidableObject* collider;

        /// @brief CollidableObject の OnCollide を呼び出す
        void OnCollide() const noexcept {
            owner->OnCollide(*collider); 
        }
    };

    /// @brief すべての CollidableObject を総当たりで確認
    std::vector<OnCollideInfo> CheckCollide()                                      const noexcept;

    /// @brief CollidableObject が当たっているか判定
    bool IsCollide(const CollidableObject* obj_a, const CollidableObject* obj_b)   const noexcept;

    /// @brief 次フレームの座標を補正
    void FixFuturePosition(CollidableObject* primary, CollidableObject* secondary) const noexcept;

    /// @brief 座標を補正
    void FixPosition() noexcept;

private:
    VECTOR future_pos_;     /// @brief 次フレームの座標 [ FIXされる ]

private:
    static Physics* instance_;

};
