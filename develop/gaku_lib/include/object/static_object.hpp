#pragma once
#include "collidable_object.hpp"
#include "component/modeler.hpp"

/// @brief �Î~�I�u�W�F�N�g
class StaticObject abstract : public CollidableObject
{
public:
    /// @remark �֎~�R���X�g���N�^
    StaticObject() = delete;

    /// @brief �C���X�^���X��������
    StaticObject(const std::string& name)
        : CollidableObject{ name, Weight::Static, ColliderInfo::Type::Mesh, false }
        , model_(nullptr)
    {
    }

    /// @brief �C���X�^���X�����
    virtual ~StaticObject() = default;

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

protected: // TODO: Subclass Sandbox

protected: // TODO: Add Component
    std::shared_ptr<Modeler>  model_;

};
