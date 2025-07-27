#pragma once
#include "collider_data.hpp"
#include "misc/primitive/capsule.hpp"

/// @brief �J�v�Z��������
class ColliderInfoCapsule : public ColliderInfo
{
public:
	/// @remark �֎~�R���X�g���N�^
	ColliderInfoCapsule() = delete;

	/// @brief �p�����[�^�[��ݒ肵�ăC���X�^���X��������
	ColliderInfoCapsule(bool is_trigger);

	/// @brief �C���X�^���X�����
	~ColliderInfoCapsule() override = default;

    /// @brief AABB���X�V
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief �ڍׂȏՓ˔���
    bool TestCollision(const ColliderInfo* other,
                       const VECTOR& pos_a,
                       const VECTOR& pos_b,
                       VECTOR& hit_point,
                       VECTOR& hit_normal, 
                       float& penetration) const override;

    /// @brief �J�v�Z���̃p�����[�^��ݒ�
    void SetCapsule(const VECTOR& local_start, const VECTOR& local_end, float radius);

    /// @brief ���[���h���W�ł̃J�v�Z���n�_���擾
    VECTOR GetWorldStart()          const noexcept { return capsule_.world_start_; }

    /// @brief ���[���h���W�ł̃J�v�Z���I�_���擾
    VECTOR GetWorldEnd()            const noexcept { return capsule_.world_end_;   }

    /// @brief ���a���擾
    float GetRadius()               const noexcept { return capsule_.radius_;      }

    /// @brief ���[�J�����W�ł̎n�_���擾
    const VECTOR& GetLocalStart()   const noexcept { return capsule_.local_start_; }

    /// @brief ���[�J�����W�ł̏I�_���擾
    const VECTOR& GetLocalEnd()     const noexcept { return capsule_.local_end_;   }

    /// @brief �J�v�Z���̍������擾�i���̒��S�Ԃ̋����j
    float GetHeight()               const noexcept;

    /// @brief �J�v�Z���̑S�����擾�i�[����[�܂Łj
    float GetTotalHeight()          const noexcept;

public:
	Capsule capsule_;

};