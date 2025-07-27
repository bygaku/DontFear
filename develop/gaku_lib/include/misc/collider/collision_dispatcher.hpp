#pragma once
#include <DxLib.h>
#include <unordered_map>
#include <functional>
#include "collider_data.hpp"

/// @brief 衝突判定を統一的に処理するディスパッチャー
class CollisionDispatcher
{
public:
    /// @brief 衝突判定関数の型
    using COLLISION_FUNC = std::function<bool(
        const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR&   pos_a, const VECTOR&   pos_b,
        VECTOR&     hit_point, VECTOR&    hit_normal,
        float& penetration
        )>;

    /// @brief インスタンスを取得
    static CollisionDispatcher& GetInstance()
    {
        static CollisionDispatcher instance;
        return instance;
    }

    /// @brief 衝突判定を実行
    bool TestCollision(const ColliderInfo* a, const ColliderInfo* b,
                       const VECTOR&   pos_a, const VECTOR&   pos_b,
                       VECTOR&     hit_point, VECTOR&    hit_normal,
                       float& penetration);

    /// @brief 衝突判定関数を登録
    /// @param type_a 衝突判定の種別
    /// @param type_b 衝突判定の種別
    /// @param func   衝突判定関数の型：bool
    void RegisterCollisionFunc(ColliderInfo::Type type_a, ColliderInfo::Type type_b, COLLISION_FUNC func);

private:
    CollisionDispatcher();
    ~CollisionDispatcher() = default;

    /// @brief タイプペアからキーを生成
    uint64_t MakeKey(ColliderInfo::Type a, ColliderInfo::Type b) const
    {
        //! 順序を正規化（小さい方を先に）
        if (static_cast<int>(a) > static_cast<int>(b)) {
            std::swap(a, b);
        }

        return (static_cast<uint64_t>(a) << 32) | static_cast<uint64_t>(b);
    }

    /// @brief 衝突判定関数のマップ
    std::unordered_map<uint64_t, COLLISION_FUNC> collision_funcs_;

};