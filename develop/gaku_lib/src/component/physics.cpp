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
	//! collidable ���z����Ɍ������ back ���s��
    bool found = std::find(instance_->collidables_.begin(), instance_->collidables_.end(), collidable) != instance_->collidables_.end();
    if (found) {
    	ASSERT(found, "the specified object is already registered.");
    }
    
    instance_->collidables_.emplace_back(collidable);
}

void Physics::Exit(CollidableObject* collidable)
{
	//! collidable ���z����Ɍ������ erase ���s��
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

    //! �ړ�����
    for (auto& object : instance_->collidables_) {
        auto   rd = object->GetRigidbody();
        VECTOR position = rd.GetPosition();
        VECTOR velocity = rd.GetVelocity();

        //! �d�͂� Velocity �ɉ�����
        if (rd.OnGravity()) {
            velocity.y += kGravity;

            //! �ő�d�͉����x����
            velocity.y = std::clamp(velocity.y, 0.f, kMaxGravityAccel);
        }

        rd.SetVelocity(velocity);
		rd.SaveFuturePosition(position + velocity);
    }

    //! �Փ˃`�F�b�N
    auto on_collide_info = instance_->CheckCollide();

    //! ���W�C��
    instance_->FixPosition();

    //! �Փ˒ʒm
    for (auto& object : on_collide_info) {
        object.owner->OnCollide(*object.collider);
    }
}

std::vector<Physics::OnCollideInfo> Physics::CheckCollide() const noexcept
{
	std::vector<OnCollideInfo> on_collide_info;
	bool	do_check = true;
	int		count	 =	  0;	/// @brief �`�F�b�N��

	//! �Փ˒ʒm�A���W�C��
	while (do_check) {
		do_check = false;
		++count;

		/// HACK: Player����̋������m�F, �œK�����ׂ�
		//! �I�u�W�F�N�g��������
		for (auto& obj_a : collidables_) {
			for (auto& obj_b : collidables_) {
				//! �ʃI�u�W�F�N�g�ł���
				if (obj_a != obj_b) {
					//! �ǂ�����Î~�I�u�W�F�N�g�̏ꍇ, ���肵�Ȃ�
					if (obj_a->GetWeight() == CollidableObject::Weight::Static &&
						obj_b->GetWeight() == CollidableObject::Weight::Static) {
						continue;
					}

					//! �Փˏ���
					if (IsCollide(obj_a, obj_b)) {
						auto weight_a = obj_a->GetWeight();
						auto weight_b = obj_b->GetWeight();

						CollidableObject* higher = obj_a;		/// @brief �d���F��
						CollidableObject* lower  = obj_b;		/// @brief �d���F��

						bool are_trigger = obj_a->GetColliderInfo()->IsTrigger() || obj_b->GetColliderInfo()->IsTrigger();
						
						//! �g���K�[�łȂ���Ύ��ڕW�ʒu�C��
						if (!are_trigger) {
							if (weight_a < weight_b) {
								higher  = obj_b;
								lower	= obj_a;
							}

							FixFuturePosition(higher, lower);
						}

						/// @note ���x���Ă΂��\�����l��, �r���x�������p.
						bool has_higher_info = false;
						bool has_lower_info	 = false;

						//! �Փ˒ʒm���̍X�V
						for (const auto& object : on_collide_info) {
							//! ���ɒʒm���X�g�Ɋ܂܂�Ă���
							if (object.owner == higher) has_higher_info = true;
							if (object.owner == lower)  has_lower_info  = true;
						}

						if (!has_higher_info) {
							on_collide_info.push_back({ higher, lower });
						}
						if (!has_lower_info)  {
							on_collide_info.push_back({ lower, higher });
						}

						//! ��x�ł��q�b�g+�␳������Փ˔���ƕ␳���Ȃ���
						if (!are_trigger) do_check = true;// �Е����g���K�[�Ȃ�q�b�g�Ƃ�Ȃ����Ȃ�
						break;
					}
				}
			}
		
			if (do_check) break;
		}

		//! ����ȏ�͔�����s��Ȃ�
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
	//! �����蔻��̎�ʂ��ƂɏC�����@��ύX
	auto higher_type = higher->GetColliderInfo()->GetType();
	auto lower_type  =  lower->GetColliderInfo()->GetType();
	auto higher_rd	 = higher->GetRigidbody();
	auto lower_rd	 =  lower->GetRigidbody();

	//! �J�v�Z�����m�̍��W�C��
	if (higher_type == ColliderInfo::Type::Capsule && lower_type == ColliderInfo::Type::Capsule) {
		VECTOR higher_to_lower_dir	= lower_rd.GetFuturePosition() - higher_rd.GetFuturePosition();
		VECTOR higher_to_lower_norm = VNorm(higher_to_lower_dir);

		auto higher_collider_info = dynamic_cast<ColliderInfoCapsule*>(higher->GetColliderInfo().get());
		auto lower_collider_info  = dynamic_cast<ColliderInfoCapsule*>(lower->GetColliderInfo().get());

		//! �����߂�����������
		
	}
	

}

void Physics::FixPosition() noexcept
{
    for (auto& object : collidables_) {
        auto rd = object->GetRigidbody();

        //! �␳���Ȃ���
        VECTOR to_fixed = rd.GetFuturePosition() - rd.GetPosition();
		if (GameTime::GetTimeScale() != 0.f) {
			to_fixed	= to_fixed * (1.f / GameTime::GetTimeScale());
		}

        //! �ړ��ʍX�V
        rd.SetVelocity(to_fixed);

        //! ���W�X�V
        rd.SetPosition(rd.GetFuturePosition());
    }
}
