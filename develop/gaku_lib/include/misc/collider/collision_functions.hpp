#pragma once
#include <DxLib.h>
#include "collider_data.hpp"
#include "collision_algorithms.hpp"

// �O���錾
class ColliderInfoCapsule;
class ColliderInfoSphere;
class ColliderInfoBox;
class ColliderInfoMesh;
class ColliderInfoTerrain;

/// @brief �Փ˔���֐��Q
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

    // ========== �w���p�[�֐� ==========

    /// @brief AABB�`�F�b�N�i���O����j
    inline bool CheckAABB(const ColliderInfo* a, const ColliderInfo* b) {
        return a->GetAABB().Intersects(b->GetAABB());
    }

    /// @brief �J�e�S���`�F�b�N
    inline bool CheckCategory(const ColliderInfo* a, const ColliderInfo* b) {
        return a->CanCollideWith(b);
    }

    /// @brief �O�����iAABB�ƃJ�e�S���`�F�b�N�j
    inline bool PreCheck(const ColliderInfo* a, const ColliderInfo* b) {
        return CheckCategory(a, b) && CheckAABB(a, b);
    }

    /// @brief ���ʂ𔽓]�iA��B�����ւ����ꍇ�̕␳�j
    inline void SwapResult(VECTOR& hit_normal) {
        hit_normal = VScale(hit_normal, -1.0f);
    }

} // namespace collision_functions