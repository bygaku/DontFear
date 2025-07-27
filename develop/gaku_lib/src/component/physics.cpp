#include <DxLib.h>
#include "misc/assert_dx.hpp"
#include "misc/math/math.hpp"
#include "app/game_time.hpp"
#include "component/physics.hpp"
#include "misc/collider/collider_info_all.hpp"

using namespace dx_math;

Physics* Physics::instance_ = nullptr;

Physics& Physics::GetInstance()
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");
    return *instance_;
}

void Physics::Create()
{
    ASSERT(DxLib_IsInit() != 0, "DxLib isn't initialize.");
    ASSERT(instance_ == nullptr, "instance has been created.");
    instance_ = new Physics();
}

void Physics::Destroy()
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");
    delete instance_;
}

Physics::Physics()
	: future_pos_(VGet(0.f, 0.f, 0.f))
{
}

Physics::~Physics()
{
}

void Physics::Entry(CollidableObject* collidable)
{	
	//! collidable が配列内に見つかれば back を行う
    bool found = std::find(instance_->collidables_.begin(), instance_->collidables_.end(), collidable) != instance_->collidables_.end();
    if (found) {
    	ASSERT(found, "the specified object is already registered.");
    }
    
    instance_->collidables_.emplace_back(collidable);
}

void Physics::Exit(CollidableObject* collidable)
{
	//! collidable が配列内に見つかれば erase を行う
    auto count = std::erase_if(instance_->collidables_, 
                              [collidable](CollidableObject* target) { 
                              return target == collidable; }
    );

    if (count <= 0) {
        ASSERT(count == 0, "the specified object isn't registered.");
    }
}

void Physics::Update() noexcept
{
    /* HACK: EARLY RETURN;
    *  if (GameTime::GetTimeScaler() < 0.01f) return;
    */

    //! 移動処理
    for (auto& object : instance_->collidables_) {
        auto   rd = object->GetRigidbody();
        VECTOR position = rd.GetPosition();
        VECTOR velocity = rd.GetVelocity();

        //! 重力を Velocity に加える
        if (rd.OnGravity()) {
            velocity.y += kGravity;

            //! 最大重力加速度処理
            velocity.y = std::clamp(velocity.y, 0.f, kMaxGravityAccel);
        }

        rd.SetVelocity(velocity);
		rd.SaveFuturePosition(position + velocity);
    }

    //! 衝突チェック
    auto on_collide_info = instance_->CheckCollide();

    //! 座標修正
    instance_->FixPosition();

    //! 衝突通知
    for (auto& object : on_collide_info) {
        object.owner->OnCollide(*object.collider);
    }
}

std::vector<Physics::OnCollideInfo> Physics::CheckCollide() const noexcept
{
	std::vector<OnCollideInfo> on_collide_info;
	bool	do_check = true;
	int		count	 =	  0;	/// @brief チェック回数

	//! 衝突通知、座標修正
	while (do_check) {
		do_check = false;
		++count;

		/// HACK: Playerからの距離を確認, 最適化すべし
		//! オブジェクト総当たり
		for (auto& obj_a : collidables_) {
			for (auto& obj_b : collidables_) {
				//! 別オブジェクトである
				if (obj_a != obj_b) {
					//! どちらも静止オブジェクトの場合, 判定しない
					if (obj_a->GetWeight() == CollidableObject::Weight::Static &&
						obj_b->GetWeight() == CollidableObject::Weight::Static) {
						continue;
					}

					//! 衝突処理
					if (IsCollide(obj_a, obj_b)) {
						auto weight_a = obj_a->GetWeight();
						auto weight_b = obj_b->GetWeight();

						CollidableObject* higher = obj_a;		/// @brief 重さ：高
						CollidableObject* lower  = obj_b;		/// @brief 重さ：低

						bool are_trigger = obj_a->GetColliderInfo()->IsTrigger() || obj_b->GetColliderInfo()->IsTrigger();
						
						//! トリガーでなければ次目標位置修正
						if (!are_trigger) {
							if (weight_a < weight_b) {
								higher  = obj_b;
								lower	= obj_a;
							}

							FixFuturePosition(higher, lower);
						}

						/// @note 何度も呼ばれる可能性を考慮, 排他遅延処理用.
						bool has_higher_info = false;
						bool has_lower_info	 = false;

						//! 衝突通知情報の更新
						for (const auto& object : on_collide_info) {
							//! 既に通知リストに含まれている
							if (object.owner == higher) has_higher_info = true;
							if (object.owner == lower)  has_lower_info  = true;
						}

						if (!has_higher_info) {
							on_collide_info.push_back({ higher, lower });
						}
						if (!has_lower_info)  {
							on_collide_info.push_back({ lower, higher });
						}

						//! 一度でもヒット+補正したら衝突判定と補正やりなおし
						if (!are_trigger) do_check = true;// 片方がトリガーならヒットとりなおさない
						break;
					}
				}
			}
		
			if (do_check) break;
		}

		//! これ以上は判定を行わない
		if (count > kMaxHitCheck && do_check) {
#if _DEBUG
			printfDx("Loop exceeded the maximum number of evaluations.\n");
#endif
			break;
		}
	}

	return on_collide_info;
}

bool Physics::IsCollide(const CollidableObject* obj_a, const CollidableObject* obj_b) const noexcept
{
    return false;
}

void Physics::FixFuturePosition(CollidableObject* higher, CollidableObject* lower) const noexcept
{
	//! 当たり判定の種別ごとに修正方法を変更
	auto higher_type = higher->GetColliderInfo()->GetType();
	auto lower_type  =  lower->GetColliderInfo()->GetType();
	auto higher_rd	 = higher->GetRigidbody();
	auto lower_rd	 =  lower->GetRigidbody();

	//! カプセル同士の座標修正
	if (higher_type == ColliderInfo::Type::Capsule && lower_type == ColliderInfo::Type::Capsule) {
		VECTOR higher_to_lower_dir	= lower_rd.GetFuturePosition() - higher_rd.GetFuturePosition();
		VECTOR higher_to_lower_norm = VNorm(higher_to_lower_dir);

		auto higher_collider_info = dynamic_cast<ColliderInfoCapsule*>(higher->GetColliderInfo().get());
		auto lower_collider_info  = dynamic_cast<ColliderInfoCapsule*>(lower->GetColliderInfo().get());

		//! 押し戻し処理をかく
		
	}
	

}

void Physics::FixPosition() noexcept
{
    for (auto& object : collidables_) {
        auto rd = object->GetRigidbody();

        //! 補正しなおす
        VECTOR to_fixed = rd.GetFuturePosition() - rd.GetPosition();
		if (GameTime::GetTimeScale() != 0.f) {
			to_fixed	= to_fixed * (1.f / GameTime::GetTimeScale());
		}

        //! 移動量更新
        rd.SetVelocity(to_fixed);

        //! 座標更新
        rd.SetPosition(rd.GetFuturePosition());
    }
}
