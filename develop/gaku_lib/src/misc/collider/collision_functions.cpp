#include "misc/collider/collision_functions.hpp"
#include "misc/collider/collider_info_all.hpp"

namespace collision_functions {

    // ========== Sphere vs Sphere ==========
    bool TestSphereSphere(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration)
    {
        if (!PreCheck(a, b)) return false;

        auto sphere_a = static_cast<const ColliderInfoSphere*>(a);
        auto sphere_b = static_cast<const ColliderInfoSphere*>(b);

        VECTOR center_a = VAdd(pos_a, sphere_a->GetLocalCenter());
        VECTOR center_b = VAdd(pos_b, sphere_b->GetLocalCenter());

        return collision_algorithms::TestSphereSphere(
            center_a, sphere_a->GetRadius(),
            center_b, sphere_b->GetRadius(),
            hit_point, hit_normal, penetration
        );
    }

    // ========== Capsule vs Capsule ==========
    bool TestCapsuleCapsule(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration)
    {
        if (!PreCheck(a, b)) return false;

        auto cap_a = static_cast<const ColliderInfoCapsule*>(a);
        auto cap_b = static_cast<const ColliderInfoCapsule*>(b);

        VECTOR a_start = VAdd(pos_a, cap_a->GetLocalStart());
        VECTOR a_end = VAdd(pos_a, cap_a->GetLocalEnd());
        VECTOR b_start = VAdd(pos_b, cap_b->GetLocalStart());
        VECTOR b_end = VAdd(pos_b, cap_b->GetLocalEnd());

        return collision_algorithms::TestCapsuleCapsule(
            a_start, a_end, cap_a->GetRadius(),
            b_start, b_end, cap_b->GetRadius(),
            hit_point, hit_normal, penetration
        );
    }

    // ========== Capsule vs Sphere ==========
    bool TestCapsuleSphere(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration)
    {
        if (!PreCheck(a, b)) return false;

        const ColliderInfo* first       = a;
        const ColliderInfo* second      = b;
        VECTOR              first_pos   = pos_a;
        VECTOR              second_pos  = pos_b;

        //! Capsuleが先に来ることを保証
        bool swapped = false;
        if (first->GetType() != ColliderInfo::Type::Capsule) {
            std::swap(first, second);
            std::swap(first_pos, second_pos);
            swapped = true;
        }

        auto capsule = static_cast<const ColliderInfoCapsule*>(first);
        auto sphere  = static_cast<const ColliderInfoSphere*>(second);

        VECTOR cap_start     = VAdd(first_pos,  capsule->GetLocalStart());
        VECTOR cap_end       = VAdd(first_pos,  capsule->GetLocalEnd());
        VECTOR sphere_center = VAdd(second_pos, sphere->GetLocalCenter());

        bool result = collision_algorithms::TestCapsuleSphere(
            cap_start, cap_end, capsule->GetRadius(),
            sphere_center, sphere->GetRadius(),
            hit_point, hit_normal, penetration
        );

        if (result && swapped) {
            SwapResult(hit_normal);
        }

        return result;
    }

    // ========== Capsule vs Box ==========
    bool TestCapsuleBox(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration)
    {
        if (!PreCheck(a, b)) return false;

        const ColliderInfo* first       = a;
        const ColliderInfo* second      = b;
        VECTOR              first_pos   = pos_a;
        VECTOR              second_pos  = pos_b;

        //! Capsuleが先に来ることを保証
        bool swapped = false;
        if (first->GetType() != ColliderInfo::Type::Capsule) {
            std::swap(first, second);
            std::swap(first_pos, second_pos);
            swapped = true;
        }

        auto capsule = static_cast<const ColliderInfoCapsule*>(first);
        auto box     = static_cast<const ColliderInfoBox*>(second);

        VECTOR cap_start = VAdd(first_pos, capsule->GetLocalStart());
        VECTOR cap_end   = VAdd(first_pos, capsule->GetLocalEnd());

        //! OBB（Oriented Bounding Box）との判定
        /// @note: ここでは簡易版を実装（より正確にはSATアルゴリズムを使用）
        VECTOR box_center       = VAdd(second_pos, box->GetLocalCenter());
        const VECTOR& half_size = box->GetHalfSize();

        //! カプセルの中心
        VECTOR cap_center = VScale(VAdd(cap_start, cap_end), 0.5f);

        //! ボックスのローカル座標系でのカプセル中心
        VECTOR local_center = VSub(cap_center, box_center);
        MATRIX inv_rotation = MTranspose(box->GetWorldRotation());
        local_center        = VTransform(local_center, inv_rotation);

        //! 最近点を計算
        VECTOR closest_local = VGet(
            std::clamp(local_center.x, -half_size.x, half_size.x),
            std::clamp(local_center.y, -half_size.y, half_size.y),
            std::clamp(local_center.z, -half_size.z, half_size.z)
        );

        //! ワールド座標に戻す
        VECTOR closest_world = VAdd(box_center,
            VTransform(closest_local, box->GetWorldRotation()));

        //! カプセル線分上の最近点
        VECTOR closest_on_cap = collision_algorithms::ClosestPointOnSegment(
            closest_world, cap_start, cap_end
        );

        VECTOR diff = VSub(closest_world, closest_on_cap);
        float dist_sq = VSquareSize(diff);

        if (dist_sq <= capsule->GetRadius() * capsule->GetRadius()) {
            float dist = sqrtf(dist_sq);
            if (dist > 0.00001f) {
                hit_normal = VScale(diff, 1.0f / dist);
                penetration = capsule->GetRadius() - dist;
                hit_point = VAdd(closest_on_cap, VScale(hit_normal, capsule->GetRadius()));
            }
            else {
                hit_normal = VGet(0, 1, 0);
                penetration = capsule->GetRadius();
                hit_point = closest_on_cap;
            }

            if (swapped) {
                SwapResult(hit_normal);
            }
            return true;
        }

        return false;
    }

    // ========== Box vs Box ==========
    bool TestBoxBox(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration)
    {
        if (!PreCheck(a, b)) return false;

        auto box_a = static_cast<const ColliderInfoBox*>(a);
        auto box_b = static_cast<const ColliderInfoBox*>(b);

        // SAT（Separating Axis Theorem）による判定
        // ここでは簡易実装（完全な実装は複雑）

        // とりあえずAABB同士の判定で代用
        const AABB& aabb_a = box_a->GetAABB();
        const AABB& aabb_b = box_b->GetAABB();

        if (!aabb_a.Intersects(aabb_b)) {
            return false;
        }

        // 簡易的な貫通深度と法線の計算
        VECTOR center_a = aabb_a.GetCenter();
        VECTOR center_b = aabb_b.GetCenter();
        VECTOR diff = VSub(center_b, center_a);

        // 各軸での重なりを計算
        VECTOR half_a = aabb_a.GetHalfSize();
        VECTOR half_b = aabb_b.GetHalfSize();

        float overlap_x = (half_a.x + half_b.x) - fabsf(diff.x);
        float overlap_y = (half_a.y + half_b.y) - fabsf(diff.y);
        float overlap_z = (half_a.z + half_b.z) - fabsf(diff.z);

        // 最小の重なりを持つ軸を分離軸とする
        if (overlap_x < overlap_y && overlap_x < overlap_z) {
            penetration = overlap_x;
            hit_normal = diff.x > 0 ? VGet(1, 0, 0) : VGet(-1, 0, 0);
        }
        else if (overlap_y < overlap_z) {
            penetration = overlap_y;
            hit_normal = diff.y > 0 ? VGet(0, 1, 0) : VGet(0, -1, 0);
        }
        else {
            penetration = overlap_z;
            hit_normal = diff.z > 0 ? VGet(0, 0, 1) : VGet(0, 0, -1);
        }

        hit_point = VAdd(center_a, VScale(hit_normal, half_a.x));
        return true;
    }

    bool TestSphereBox(const ColliderInfo* a, const ColliderInfo* b, const VECTOR& pos_a, const VECTOR& pos_b, VECTOR& hit_point, VECTOR& hit_normal, float& penetration)
    {
        return false;
    }

    // ========== Any vs Terrain ==========
    bool TestAnyTerrain(const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR& pos_a, const VECTOR& pos_b,
        VECTOR& hit_point, VECTOR& hit_normal,
        float& penetration)
    {
        /*
        if (!PreCheck(a, b)) return false;

        // 地形が必ずbに来るようにする
        bool swapped = false;
        if (b->GetType() != ColliderInfo::Type::Terrain) {
            std::swap(a, b);
            std::swap(pos_a, pos_b);
            swapped = true;
        }

        auto terrain = static_cast<const ColliderInfoTerrain*>(b);

        // 地形は通常原点にあるため、pos_bは無視されることが多い
        bool result = false;

        switch (a->GetType()) {
        case ColliderInfo::Type::Capsule: {
            auto capsule = static_cast<const ColliderInfoCapsule*>(a);
            VECTOR start = VAdd(pos_a, capsule->GetLocalStart());
            VECTOR end = VAdd(pos_a, capsule->GetLocalEnd());
            result = terrain->TestCapsuleCollision(
                start, end, capsule->GetRadius(),
                hit_point, hit_normal, penetration
            );
            break;
        }

        case ColliderInfo::Type::Sphere: {
            auto sphere = static_cast<const ColliderInfoSphere*>(a);
            VECTOR center = VAdd(pos_a, sphere->GetLocalCenter());

            // 球を小さなカプセルとして扱う
            result = terrain->TestCapsuleCollision(
                center, center, sphere->GetRadius(),
                hit_point, hit_normal, penetration
            );
            break;
        }

                                       // 他のタイプは必要に応じて実装
        default:
            break;
        }

        if (result && swapped) {
            SwapResult(hit_normal);
        }

        return result;
        */
        return true;

    }

} // namespace collision_functions