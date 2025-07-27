#include <DxLib.h>
#include "misc/assert_dx.hpp"
#include "component/physics.hpp"
#include "object/collidable_object.hpp"
#include "misc/collider/collider_info_all.hpp"

CollidableObject::CollidableObject(const std::string& name, Weight weight, ColliderInfo::Type collider_type, bool is_trigger)
    : GameObject{ name }
    , weight_(weight)
    , collider_info_(nullptr)
{
    CreateColliderInfo(collider_type, is_trigger);
}

CollidableObject::~CollidableObject()
{    
    Physics::Exit(this);
}

void CollidableObject::Initialize() noexcept
{
    Physics::Entry(this);
}

void CollidableObject::AddThroughTag(std::string tag) noexcept
{
    bool found = std::find(through_tags_.begin(), through_tags_.end(), tag) != through_tags_.end();

    if (!found) {
        through_tags_.emplace_back(tag);
        return;
    }

    ASSERT(!found, "tag is already added list");
}

void CollidableObject::RemoveThroughTag(std::string tag) noexcept
{
    bool found = std::find(through_tags_.begin(), through_tags_.end(), tag) != through_tags_.end();

    if (found) {
        through_tags_.emplace_back(tag);
        return;
    }

    ASSERT(found, "tag isn't found list");
}

bool CollidableObject::IsThroughTarget(const CollidableObject* target) const noexcept
{
    return std::find(through_tags_.begin(), through_tags_.end(), target->GetTag()) != through_tags_.end();
}

std::shared_ptr<ColliderInfo> CollidableObject::CreateColliderInfo(ColliderInfo::Type collider_type, bool is_trigger)
{
    if (collider_info_ != nullptr) {
        return collider_info_;
    }

    switch (collider_type) {
    case ColliderInfo::Type::Segment:
        return collider_info_ = std::make_shared<ColliderInfoSegment>(is_trigger);
    case ColliderInfo::Type::Sphere:
        return collider_info_ = std::make_shared<ColliderInfoSphere>(is_trigger);
    case ColliderInfo::Type::Capsule:
        return collider_info_ = std::make_shared<ColliderInfoCapsule>(is_trigger);
    default:
        return nullptr;
    }
}

