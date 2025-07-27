#include <DxLib.h>
#include "misc/collider/collider_info_sphere.hpp"
#include "misc/collider/collision_dispatcher.hpp"

ColliderInfoSphere::ColliderInfoSphere(bool is_trigger)
    : ColliderInfo{ Type::Sphere, is_trigger }
    , world_center_(VGet(0.f, 0.f, 0.f))
{
}

void ColliderInfoSphere::UpdateAABB(const VECTOR& position, const MATRIX& rotation)
{
    // ローカル中心をワールド座標に変換
    world_center_ = VAdd(position, VTransform(sphere_.position_, rotation));

    //! AABBを計算（球なので単純）
    aabb_ = AABB::FromSphere(world_center_, sphere_.radius_);

    // 更新フラグをクリア
    SetNeedsUpdate(false);
}

bool ColliderInfoSphere::TestCollision(const ColliderInfo* other, 
                                       const VECTOR& pos_a,
                                       const VECTOR& pos_b, 
                                       VECTOR& hit_point,
                                       VECTOR& hit_normal, 
                                       float& penetration) const
{
    //! CollisionDispatcher に処理を委譲
    return CollisionDispatcher::GetInstance().TestCollision(
        this, other, pos_a, pos_b, hit_point, hit_normal, penetration
    );
}

void ColliderInfoSphere::SetSphere(const VECTOR& local_center, float radius)
{
    sphere_.position_ = local_center;
    sphere_.radius_   = radius;
    SetNeedsUpdate(true);
}

float ColliderInfoSphere::GetVolume() const
{
    //! 球の体積: (4/3) * π * r³
    return (4.0f / 3.0f) * DX_PI_F * sphere_.radius_ * sphere_.radius_ * sphere_.radius_;
}

float ColliderInfoSphere::GetSurfaceArea() const
{
    //! 球の表面積: 4 * π * r²
    return 4.0f * DX_PI_F * sphere_.radius_ * sphere_.radius_;
}

bool ColliderInfoSphere::ContainsPoint(const VECTOR& point) const
{
    //! 点が球内にあるかチェック
    float dist_sq = VSquareSize(VSub(point, world_center_));
    return dist_sq <= sphere_.radius_ * sphere_.radius_;
}

VECTOR ColliderInfoSphere::GetClosestPoint(const VECTOR& point) const
{
    //! 球表面上の最近点を取得
    VECTOR to_point = VSub(point, world_center_);
    float  dist     = VSize(to_point);

    //! 中心と一致する場合は上方向の点を返す
    if (dist < 0.0001f) {
        return VAdd(world_center_, VGet(0.f, sphere_.radius_, 0.f));
    }

    //! 正規化して半径分伸ばす
    return VAdd(world_center_, VScale(VNorm(to_point), sphere_.radius_));
}

VECTOR ColliderInfoSphere::GetSupportPoint(const VECTOR& direction) const
{
    //! サポート写像（指定方向の最遠点）
    if (VSize(direction) < 0.0001f) {
        return world_center_;
    }

    return VAdd(world_center_, VScale(VNorm(direction), sphere_.radius_));
}