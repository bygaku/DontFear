#include <DxLib.h>
#include <algorithm>
#include "misc/collider/collider_info_box.hpp"
#include "misc/collider/collision_dispatcher.hpp"

ColliderInfoBox::ColliderInfoBox(bool is_trigger, uint32_t category, uint32_t mask)
    : ColliderInfo(Type::Box, is_trigger, category, mask)
    , local_center_(VGet(0, 0, 0))
    , half_size_(VGet(0.5f, 0.5f, 0.5f))
    , world_center_(VGet(0, 0, 0))
    , world_rotation_(MGetIdent())
{
}

ColliderInfoBox::ColliderInfoBox(const VECTOR& local_center, const VECTOR& half_size,
    bool is_trigger, uint32_t category, uint32_t mask)
    : ColliderInfo(Type::Box, is_trigger, category, mask)
    , local_center_(local_center)
    , half_size_(half_size)
    , world_center_(local_center)
    , world_rotation_(MGetIdent())
{
}

void ColliderInfoBox::UpdateAABB(const VECTOR& position, const MATRIX& rotation)
{
    world_center_ = VAdd(position, VTransform(local_center_, rotation));
    world_rotation_ = rotation;
    aabb_ = AABB::FromOBB(world_center_, half_size_, rotation);
    SetNeedsUpdate(false);
}

bool ColliderInfoBox::TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
    const VECTOR& pos_b, VECTOR& hit_point,
    VECTOR& hit_normal, float& penetration) const
{
    //! CollisionDispatcher に処理を委譲
    return CollisionDispatcher::GetInstance().TestCollision(
        this, other, pos_a, pos_b, hit_point, hit_normal, penetration
    );
}

void ColliderInfoBox::SetBox(const VECTOR& local_center, const VECTOR& half_size)
{
    local_center_ = local_center;
    half_size_ = half_size;
    SetNeedsUpdate(true);
}

void ColliderInfoBox::GetWorldVertices(VECTOR vertices[8]) const
{
    const VECTOR& h = half_size_;
    VECTOR local_vertices[8] = {
        VGet(-h.x, -h.y, -h.z), VGet(h.x, -h.y, -h.z),
        VGet(h.x,  h.y, -h.z), VGet(-h.x,  h.y, -h.z),
        VGet(-h.x, -h.y,  h.z), VGet(h.x, -h.y,  h.z),
        VGet(h.x,  h.y,  h.z), VGet(-h.x,  h.y,  h.z)
    };

    for (int i = 0; i < 8; i++) {
        vertices[i] = VAdd(world_center_, VTransform(local_vertices[i], world_rotation_));
    }
}

void ColliderInfoBox::GetWorldNormals(VECTOR normals[6]) const
{
    VECTOR local_normals[6] = {
        VGet(1,  0,  0),  // +X面
        VGet(-1,  0,  0),  // -X面
        VGet(0,  1,  0),  // +Y面
        VGet(0, -1,  0),  // -Y面
        VGet(0,  0,  1),  // +Z面
        VGet(0,  0, -1)   // -Z面
    };

    for (int i = 0; i < 6; i++) {
        normals[i] = VTransform(local_normals[i], world_rotation_);
    }
}

bool ColliderInfoBox::ContainsPoint(const VECTOR& point) const
{
    //! ワールド座標の点をローカル座標に変換
    MATRIX inv_rotation = MTranspose(world_rotation_);
    VECTOR local_point  = VTransform(VSub(point, world_center_), inv_rotation);

    //! ローカル座標でボックス内判定
    return fabsf(local_point.x) <= half_size_.x &&
        fabsf(local_point.y) <= half_size_.y &&
        fabsf(local_point.z) <= half_size_.z;
}

VECTOR ColliderInfoBox::GetClosestPoint(const VECTOR& point) const
{
    //! ワールド座標の点をローカル座標に変換
    MATRIX inv_rotation = MTranspose(world_rotation_);
    VECTOR local_point = VTransform(VSub(point, world_center_), inv_rotation);

    //! ローカル座標で最近点を計算
    VECTOR local_closest = VGet(
        std::clamp(local_point.x, -half_size_.x, half_size_.x),
        std::clamp(local_point.y, -half_size_.y, half_size_.y),
        std::clamp(local_point.z, -half_size_.z, half_size_.z)
    );

    //! ワールド座標に変換して返す
    return VAdd(world_center_, VTransform(local_closest, world_rotation_));
}

VECTOR ColliderInfoBox::GetSupportPoint(const VECTOR& direction) const
{
    //! 方向ベクトルをローカル座標に変換
    MATRIX inv_rotation = MTranspose(world_rotation_);
    VECTOR local_dir = VTransform(direction, inv_rotation);

    //! 各軸で最も遠い点を選択
    VECTOR local_support = VGet(
        local_dir.x > 0 ? half_size_.x : -half_size_.x,
        local_dir.y > 0 ? half_size_.y : -half_size_.y,
        local_dir.z > 0 ? half_size_.z : -half_size_.z
    );

    //! ワールド座標に変換して返す
    return VAdd(world_center_, VTransform(local_support, world_rotation_));
}