#include <DxLib.h>
#include <algorithm>
#include <cmath>
#include "misc/collider/collider_info_segment.hpp"
#include "misc/collider/collision_dispatcher.hpp"

ColliderInfoSegment::ColliderInfoSegment(bool is_trigger, uint32_t category, uint32_t mask)
    : ColliderInfo(Type::Segment, is_trigger, category, mask)
    , local_start_(VGet(0, 0, 0))
    , local_end_(VGet(0, 1, 0))
    , thickness_(0.0f)
    , world_start_(local_start_)
    , world_end_(local_end_)
{
}

ColliderInfoSegment::ColliderInfoSegment(const VECTOR& local_start, const VECTOR& local_end,
    float thickness, bool is_trigger,
    uint32_t category, uint32_t mask)
    : ColliderInfo(Type::Segment, is_trigger, category, mask)
    , local_start_(local_start)
    , local_end_(local_end)
    , thickness_(thickness)
    , world_start_(local_start)
    , world_end_(local_end)
{
}

void ColliderInfoSegment::UpdateAABB(const VECTOR& position, const MATRIX& rotation)
{
    // ローカル座標をワールド座標に変換
    world_start_ = VAdd(position, VTransform(local_start_, rotation));
    world_end_ = VAdd(position, VTransform(local_end_, rotation));

    // AABBを計算
    VECTOR min_point = VGet(
        std::min(world_start_.x, world_end_.x) - thickness_,
        std::min(world_start_.y, world_end_.y) - thickness_,
        std::min(world_start_.z, world_end_.z) - thickness_
    );

    VECTOR max_point = VGet(
        std::max(world_start_.x, world_end_.x) + thickness_,
        std::max(world_start_.y, world_end_.y) + thickness_,
        std::max(world_start_.z, world_end_.z) + thickness_
    );

    aabb_ = AABB(min_point, max_point);

    // 更新フラグをクリア
    SetNeedsUpdate(false);
}

bool ColliderInfoSegment::TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
    const VECTOR& pos_b, VECTOR& hit_point,
    VECTOR& hit_normal, float& penetration) const
{
    // CollisionDispatcherに処理を委譲
    return CollisionDispatcher::GetInstance().TestCollision(
        this, other, pos_a, pos_b, hit_point, hit_normal, penetration
    );
}

void ColliderInfoSegment::SetSegment(const VECTOR& local_start, const VECTOR& local_end, float thickness)
{
    local_start_ = local_start;
    local_end_ = local_end;
    thickness_ = thickness;
    SetNeedsUpdate(true);
}

float ColliderInfoSegment::GetLength() const
{
    return VSize(VSub(local_end_, local_start_));
}

VECTOR ColliderInfoSegment::GetDirection() const
{
    VECTOR dir = VSub(world_end_, world_start_);
    float len = VSize(dir);

    if (len < 0.0001f) {
        return VGet(0, 1, 0);  // デフォルト方向
    }

    return VScale(dir, 1.0f / len);
}

float ColliderInfoSegment::GetDistanceToPoint(const VECTOR& point) const
{
    VECTOR closest = GetClosestPoint(point);
    return VSize(VSub(point, closest));
}

VECTOR ColliderInfoSegment::GetClosestPoint(const VECTOR& point) const
{
    VECTOR seg_vec = VSub(world_end_, world_start_);
    float seg_len_sq = VSquareSize(seg_vec);

    if (seg_len_sq < 0.0001f) {
        // 始点と終点が同じ場合
        return world_start_;
    }

    // 線分上のパラメータtを計算
    float t = VDot(VSub(point, world_start_), seg_vec) / seg_len_sq;
    t = std::clamp(t, 0.0f, 1.0f);

    // 最近点を計算
    return VAdd(world_start_, VScale(seg_vec, t));
}

bool ColliderInfoSegment::Raycast(const VECTOR& target_point, float& t) const
{
    // この線分が原点から target_point への線分と交差するかチェック
    // （主にレーザーや視線判定で使用）

    VECTOR seg_dir = VSub(world_end_, world_start_);
    VECTOR to_target = VSub(target_point, world_start_);

    float seg_len = VSize(seg_dir);
    if (seg_len < 0.0001f) {
        return false;
    }

    seg_dir = VScale(seg_dir, 1.0f / seg_len);

    // 線分上への投影
    float proj = VDot(to_target, seg_dir);

    if (proj < 0.0f || proj > seg_len) {
        return false;
    }

    // パラメータtを設定
    t = proj / seg_len;

    // 距離チェック（太さを考慮）
    VECTOR closest = VAdd(world_start_, VScale(seg_dir, proj));
    float dist = VSize(VSub(target_point, closest));

    return dist <= thickness_;
}