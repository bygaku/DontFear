#include <DxLib.h>
#include "component/transform.hpp"
#include "component/rigidbody.hpp"

Rigidbody::Rigidbody()
    : position_ (VGet(0.f, 0.f, 0.f))
    , direction_(VGet(0.f, 0.f, 0.f))
    , velocity_ (VGet(0.f, 0.f, 0.f))
    , use_gravity_(true)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Initialize() noexcept
{
    position_  = VGet(0.f, 0.f, 0.f);
    direction_ = VGet(0.f, 0.f, 0.f);
    velocity_  = VGet(0.f, 0.f, 0.f);
}

void Rigidbody::SetPosition(const VECTOR& position) noexcept
{
    position_ = position;
}

void Rigidbody::SetVelocity(const VECTOR& velocity) noexcept
{
    velocity_ = velocity;

    if (VSquareSize(velocity_) > 0) {
        direction_ = VNorm(velocity_);
    }
}

void Rigidbody::SetUseGravity(bool gravity) noexcept
{
    use_gravity_ = gravity;
}

void Rigidbody::AddForce(const VECTOR& force_direction) noexcept
{

}

void Rigidbody::SaveFuturePosition(const VECTOR& future_pos) noexcept
{
    future_pos_ = future_pos;
}
