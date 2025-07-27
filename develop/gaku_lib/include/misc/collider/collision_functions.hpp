#pragma once
#include <DxLib.h>
#include "collider_data.hpp"
#include "collision_algorithms.hpp"

// 前方宣言
class ColliderInfoCapsule;
class ColliderInfoSphere;
class ColliderInfoBox;
class ColliderInfoMesh;
class ColliderInfoTerrain;

/// @brief 衝突判定関数群
namespace collision_functions {
    // ========== Sphere vs Sphere ==========
    bool TestSphereSphere(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== Capsule vs Capsule ==========
    bool TestCapsuleCapsule(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== Capsule vs Sphere ==========
    bool TestCapsuleSphere(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== Capsule vs Box ==========
    bool TestCapsuleBox(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== Box vs Box ==========
    bool TestBoxBox(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== Sphere vs Box ==========
    bool TestSphereBox(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== Any vs Mesh ==========
    bool TestAnyMesh(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== Any vs Terrain ==========
    bool TestAnyTerrain(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration);

    // ========== ヘルパー関数 ==========

    /// @brief AABBチェック（事前判定）
    inline bool CheckAABB(const ColliderInfo* a, const ColliderInfo* b) {
        return a->GetAABB().Intersects(b->GetAABB());
    }

    /// @brief カテゴリチェック
    inline bool CheckCategory(const ColliderInfo* a, const ColliderInfo* b) {
        return a->CanCollideWith(b);
    }

    /// @brief 前処理（AABBとカテゴリチェック）
    inline bool PreCheck(const ColliderInfo* a, const ColliderInfo* b) {
        return CheckCategory(a, b) && CheckAABB(a, b);
    }

    /// @brief 結果を反転（AとBを入れ替えた場合の補正）
    inline void SwapResult(VECTOR& hit_normal) {
        hit_normal = VScale(hit_normal, -1.0f);
    }

} // namespace collision_functions