#include "misc/collider/collider_info_capsule.hpp"

ColliderInfoCapsule::ColliderInfoCapsule(bool is_trigger)
    : ColliderInfo{ Type::Capsule, is_trigger }
{
}

void ColliderInfoCapsule::UpdateAABB(const VECTOR& position, const MATRIX& rotation)
{
}

bool ColliderInfoCapsule::TestCollision(const ColliderInfo* other, const VECTOR& pos_a, const VECTOR& pos_b, VECTOR& hit_point, VECTOR& hit_normal, float& penetration) const
{
    return false;
}

void ColliderInfoCapsule::SetCapsule(const VECTOR& local_start, const VECTOR& local_end, float radius)
{
}

float ColliderInfoCapsule::GetHeight() const noexcept
{
    return 0.0f;
}

float ColliderInfoCapsule::GetTotalHeight() const noexcept
{
    return 0.0f;
}
