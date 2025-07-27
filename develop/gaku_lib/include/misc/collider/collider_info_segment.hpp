#pragma once
#include "collider_data.hpp"
#include "misc/primitive/segment.hpp"

/// @brief ����������
class ColliderInfoSegment : public ColliderInfo 
{
public:
    /// @remark �֎~�R���X�g���N�^
    ColliderInfoSegment() = delete;

	/// @brief �p�����[�^�[��ݒ肵�ăC���X�^���X��������
    ColliderInfoSegment(bool is_trigger, uint32_t category = Category::kAll,
        uint32_t mask = Category::kAll);

    /// @brief �����p�����[�^�t���R���X�g���N�^
    ColliderInfoSegment(const VECTOR& local_start, const VECTOR& local_end,
        float thickness, bool is_trigger,
        uint32_t category = Category::kAll,
        uint32_t mask = Category::kAll);

    /// @brief �C���X�^���X�����
    ~ColliderInfoSegment() override = default;

    /// @brief AABB���X�V
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief �ڍׂȏՓ˔���
    bool TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const override;

    /// @brief �����̃p�����[�^��ݒ�
    void SetSegment(const VECTOR& local_start, const VECTOR& local_end, float thickness = 0.0f);

    /// @brief ���[�J�����W�ł̎n�_���擾
    const VECTOR& GetLocalStart() const { return local_start_; }

    /// @brief ���[�J�����W�ł̏I�_���擾
    const VECTOR& GetLocalEnd() const { return local_end_; }

    /// @brief ���[���h���W�ł̎n�_���擾
    const VECTOR& GetWorldStart() const { return world_start_; }

    /// @brief ���[���h���W�ł̏I�_���擾
    const VECTOR& GetWorldEnd() const { return world_end_; }

    /// @brief �������擾�i0�̏ꍇ�͖����ɍׂ����j
    float GetThickness() const { return thickness_; }

    /// @brief �����̒������擾
    float GetLength() const;

    /// @brief �����̕����x�N�g���i���K���j���擾
    VECTOR GetDirection() const;

    /// @brief �_�Ƃ̍ŒZ�������擾
    float GetDistanceToPoint(const VECTOR& point) const;

    /// @brief ������̍ŋߓ_���擾
    VECTOR GetClosestPoint(const VECTOR& point) const;

    /// @brief ���C�L���X�g�̂悤�Ȏg�p�̂��߂̃w���p�[
    bool Raycast(const VECTOR& target_point, float& t) const;

private:
    // ���[�J�����W�n�ł̐�����`
    VECTOR local_start_;    /// @brief ���[�J�����W�ł̎n�_
    VECTOR local_end_;      /// @brief ���[�J�����W�ł̏I�_
    float thickness_;       /// @brief �����̑����i0�Ȃ疳���ɍׂ��j

    // ���[���h���W�n�ł̐����i�L���b�V���j
    VECTOR world_start_;    /// @brief ���[���h���W�ł̎n�_
    VECTOR world_end_;      /// @brief ���[���h���W�ł̏I�_
};