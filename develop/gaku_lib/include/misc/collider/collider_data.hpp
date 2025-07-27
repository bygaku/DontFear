#pragma once
#include "misc/primitive/aabb.hpp"

/// @brief 衝突判定のデータ
class ColliderInfo abstract
{
public:
    /// @brief 判定種別
    enum class Type {
        Segment,
        Sphere,
        Capsule,
        Box, 
        Mesh,
        Terrain
    };

    /// @brief 判定のカテゴリ
    /// @remark 必要な組み合わせを効率的に処理
    enum Category : uint32_t {
        kNone = 0,
        kPlayer = 1 << 0,
        kEnemy = 1 << 1,
        kTerrain = 1 << 2,
        kStatic = 1 << 3,
        kDynamic = 1 << 4,
        kTrigger = 1 << 5,
        kAll = 0xFFFFFFFF
    };

public:
    /// @remark 禁止コンストラクタ
    ColliderInfo() = delete;

    /// @brief パラメーターを設定してインスタンスを初期化
    ColliderInfo(Type type, bool is_trigger, uint32_t category = Category::kAll, uint32_t mask = Category::kAll)
        : type_(type)
        , is_trigger_(is_trigger)
        , category_(category)
        , collision_mask_(mask)
        , needs_update_(true)
    {
        type_       = type;
        is_trigger_ = is_trigger;
    }

    /// @brief インスタンスを解放
    virtual ~ColliderInfo() = default;

    /// @brief AABBを更新
    /// @brief 派生クラスで実装
    virtual void UpdateAABB(const VECTOR& position, const MATRIX& rotation) abstract;

    /// @brief 詳細な衝突判定
    /// @brief 派生クラスで実装
    virtual bool TestCollision(const ColliderInfo* other, 
                               const VECTOR& pos_a,
                               const VECTOR& pos_b, 
                               VECTOR& hit_point,
                               VECTOR& hit_normal,
                               float& penetration)                    const abstract;

    /// @brief 当たり判定種別取得
    Type GetType()              const noexcept { return type_;           }

    /// @brief トリガーか判定                                              
    bool IsTrigger()            const noexcept { return is_trigger_;     }

    /// @brief AABBを取得                                                 
    const AABB& GetAABB()       const noexcept { return aabb_;           }

    /// @brief カテゴリを取得                                              
    uint32_t GetCategory()      const noexcept { return category_;       }

    /// @brief 衝突マスクを取得
    uint32_t GetCollisionMask() const noexcept { return collision_mask_; }

    /// @brief 衝突可能か判定
    bool CanCollideWith(const ColliderInfo* other) const noexcept
    {
        return (category_ & other->collision_mask_) &&
               (collision_mask_ & other->category_);
    }

    /// @brief 更新が必要かどうか
    bool NeedsUpdate()          const noexcept { return needs_update_;   }

    /// @brief 更新が必要か設定
    void SetNeedsUpdate(bool needs)   noexcept { needs_update_ = needs;  }

protected:
    Type     type_;            /// @brief 当たり判定種類
    bool     is_trigger_;      /// @brief トリガー
    AABB     aabb_;            /// @brief バウンディングボックス
    uint32_t category_;        /// @brief このコライダーのカテゴリ
    uint32_t collision_mask_;  /// @brief 衝突するカテゴリのマスク
    bool     needs_update_;    /// @brief AABBの更新が必要か

};
