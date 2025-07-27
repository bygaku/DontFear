#pragma once
#include "misc/primitive/aabb.hpp"

/// @brief �Փ˔���̃f�[�^
class ColliderInfo abstract
{
public:
    /// @brief ������
    enum class Type {
        Segment,
        Sphere,
        Capsule,
        Box, 
        Mesh,
        Terrain
    };

    /// @brief ����̃J�e�S��
    /// @remark �K�v�ȑg�ݍ��킹�������I�ɏ���
    enum Category : uint32_t {
        kNone = 0,
        kPlayer = 1 << 0,
        kEnemy = 1 << 1,
        kTerrain = 1 << 2,
        kStatic = 1 << 3,
        kDynamic = 1 << 4,
        kTrigger = 1 << 5,
        kAll = 0xFFFFFFFF
    };

public:
    /// @remark �֎~�R���X�g���N�^
    ColliderInfo() = delete;

    /// @brief �p�����[�^�[��ݒ肵�ăC���X�^���X��������
    ColliderInfo(Type type, bool is_trigger, uint32_t category = Category::kAll, uint32_t mask = Category::kAll)
        : type_(type)
        , is_trigger_(is_trigger)
        , category_(category)
        , collision_mask_(mask)
        , needs_update_(true)
    {
        type_       = type;
        is_trigger_ = is_trigger;
    }

    /// @brief �C���X�^���X�����
    virtual ~ColliderInfo() = default;

    /// @brief AABB���X�V
    /// @brief �h���N���X�Ŏ���
    virtual void UpdateAABB(const VECTOR& position, const MATRIX& rotation) abstract;

    /// @brief �ڍׂȏՓ˔���
    /// @brief �h���N���X�Ŏ���
    virtual bool TestCollision(const ColliderInfo* other, 
                               const VECTOR& pos_a,
                               const VECTOR& pos_b, 
                               VECTOR& hit_point,
                               VECTOR& hit_normal,
                               float& penetration)                    const abstract;

    /// @brief �����蔻���ʎ擾
    Type GetType()              const noexcept { return type_;           }

    /// @brief �g���K�[������                                              
    bool IsTrigger()            const noexcept { return is_trigger_;     }

    /// @brief AABB���擾                                                 
    const AABB& GetAABB()       const noexcept { return aabb_;           }

    /// @brief �J�e�S�����擾                                              
    uint32_t GetCategory()      const noexcept { return category_;       }

    /// @brief �Փ˃}�X�N���擾
    uint32_t GetCollisionMask() const noexcept { return collision_mask_; }

    /// @brief �Փˉ\������
    bool CanCollideWith(const ColliderInfo* other) const noexcept
    {
        return (category_ & other->collision_mask_) &&
               (collision_mask_ & other->category_);
    }

    /// @brief �X�V���K�v���ǂ���
    bool NeedsUpdate()          const noexcept { return needs_update_;   }

    /// @brief �X�V���K�v���ݒ�
    void SetNeedsUpdate(bool needs)   noexcept { needs_update_ = needs;  }

protected:
    Type     type_;            /// @brief �����蔻����
    bool     is_trigger_;      /// @brief �g���K�[
    AABB     aabb_;            /// @brief �o�E���f�B���O�{�b�N�X
    uint32_t category_;        /// @brief ���̃R���C�_�[�̃J�e�S��
    uint32_t collision_mask_;  /// @brief �Փ˂���J�e�S���̃}�X�N
    bool     needs_update_;    /// @brief AABB�̍X�V���K�v��

};
