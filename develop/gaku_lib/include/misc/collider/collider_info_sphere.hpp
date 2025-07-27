#pragma once
#include "collider_data.hpp"
#include "misc/primitive/sphere.hpp"

/// @brief ����������
class ColliderInfoSphere : public ColliderInfo
{
public:
	/// @remark �֎~�R���X�g���N�^
	ColliderInfoSphere() = delete;

	/// @brief �p�����[�^�[��ݒ肵�ăC���X�^���X��������
	ColliderInfoSphere(bool is_trigger);

	/// @brief �C���X�^���X�����
	~ColliderInfoSphere() override = default;

    /// @brief AABB���X�V
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief �ڍׂȏՓ˔���
    bool TestCollision(const ColliderInfo* other,
                       const VECTOR& pos_a,
                       const VECTOR& pos_b,
                       VECTOR& hit_point,
                       VECTOR& hit_normal,
                       float& penetration) const override;

    /// @brief ���̃p�����[�^��ݒ�
    void SetSphere(const VECTOR& local_center, float radius);

    /// @brief ���[�J�����S���擾
    const VECTOR& GetLocalCenter() const { return sphere_.position_; }

    /// @brief ���[���h���S���擾
    const VECTOR& GetWorldCenter() const { return world_center_;     }

    /// @brief ���a���擾
    float GetRadius()              const { return sphere_.radius_;   }

    /// @brief �̐ς��擾
    float GetVolume() const;

    /// @brief �\�ʐς��擾
    float GetSurfaceArea() const;

    /// @brief �_�������ɂ��邩����
    bool ContainsPoint(const VECTOR& point) const;

    /// @brief ���\�ʏ�̍ŋߓ_���擾
    VECTOR GetClosestPoint(const VECTOR& point) const;

    /// @brief �T�|�[�g�ʑ��iGJK�p�j
    VECTOR GetSupportPoint(const VECTOR& direction) const;

public:
	Sphere sphere_;
    VECTOR world_center_;

};