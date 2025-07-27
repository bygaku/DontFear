#pragma once
#include "collidable_object.hpp"
#include "component/health.hpp"
#include "component/modeler.hpp"
#include "component/animator.hpp"

/// @brief �L�����N�^�[�I�u�W�F�N�g
class CharaObject abstract : public CollidableObject
{
public:
    /// @remark �֎~�R���X�g���N�^
    CharaObject() = delete;

    /// @brief �C���X�^���X��������
    CharaObject(const std::string& name)
        : CollidableObject{ name, Weight::Normal, ColliderInfo::Type::Capsule, false }
        , health_(std::make_shared<Health>(*this, 1))
        , model_(nullptr)
        , animator_(nullptr)
    {
    }

    /// @brief �C���X�^���X�����
    virtual ~CharaObject() = default;

    /// @brief �I�u�W�F�N�g��������
    virtual void Initialize() noexcept abstract;

    /// @brief �I�u�W�F�N�g���X�V
    virtual void Update()	  noexcept abstract;

    /// @brief �I�u�W�F�N�g��`��
    virtual void Draw() const noexcept abstract;

    /// @brief �I�u�W�F�N�g�̍ŏI�X�V
    /// HACK: ���t���[���Ă΂Ȃ��݌v�ɂ���
    virtual void LastUpdate() noexcept abstract;

    /// @brief �Փ˔���
    virtual void OnCollide(const CollidableObject& object) noexcept abstract;

protected:
    /// @remark �̗͑���
    void SetHealth(uint32_t amount) noexcept { health_->UpgradeMaxHealth(amount); }
    void OnDamage (uint32_t amount) noexcept { health_->TakeDamage      (amount); }
    void OnHeal	  (uint32_t amount) noexcept { health_->TakeHeal        (amount); }

protected:
    std::shared_ptr<Health>   health_;      /// @brief �̗͊Ǘ��R���|�[�l���g
    std::shared_ptr<Modeler>  model_;       /// @brief ���f���Ǘ��R���|�[�l���g
    std::shared_ptr<Animator> animator_;    /// @brief �A�j���[�V�����Ǘ��R���|�[�l���g

};
