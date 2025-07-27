#pragma once
#include "collider_data.hpp"

/// @brief �{�b�N�X�iOBB�j������
class ColliderInfoBox : public ColliderInfo 
{
public:
    /// @brief �f�t�H���g�R���X�g���N�^�i�폜�j
    ColliderInfoBox() = delete;

    /// @brief �R���X�g���N�^
    ColliderInfoBox(bool is_trigger, uint32_t category = Category::kAll,
        uint32_t mask = Category::kAll);

    /// @brief �p�����[�^�t���R���X�g���N�^
    ColliderInfoBox(const VECTOR& local_center, const VECTOR& half_size, bool is_trigger,
        uint32_t category = Category::kAll, uint32_t mask = Category::kAll);

    /// @brief �f�X�g���N�^
    ~ColliderInfoBox() override = default;

    /// @brief AABB���X�V
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief �ڍׂȏՓ˔���
    bool TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const override;

    /// @brief �{�b�N�X�̃p�����[�^��ݒ�
    void SetBox(const VECTOR& local_center, const VECTOR& half_size);

    /// @brief ���[�J�����S���擾
    const VECTOR& GetLocalCenter() const { return local_center_; }

    /// @brief ���[���h���S���擾
    const VECTOR& GetWorldCenter() const { return world_center_; }

    /// @brief �����̃T�C�Y���擾
    const VECTOR& GetHalfSize() const { return half_size_; }

    /// @brief ���[���h��]���擾
    const MATRIX& GetWorldRotation() const { return world_rotation_; }

    /// @brief �{�b�N�X��8���_���擾�i���[���h���W�j
    void GetWorldVertices(VECTOR vertices[8]) const;

    /// @brief �{�b�N�X��6�ʂ̖@�����擾�i���[���h���W�j
    void GetWorldNormals(VECTOR normals[6]) const;

    /// @brief �_���{�b�N�X���ɂ��邩����
    bool ContainsPoint(const VECTOR& point) const;

    /// @brief �{�b�N�X�\�ʏ�̍ŋߓ_���擾
    VECTOR GetClosestPoint(const VECTOR& point) const;

    /// @brief �T�|�[�g�ʑ��iGJK�p�j
    VECTOR GetSupportPoint(const VECTOR& direction) const;

private:
    VECTOR local_center_;    /// @brief ���[�J�����W�ł̒��S
    VECTOR half_size_;       /// @brief �e���̔����̃T�C�Y
    VECTOR world_center_;    /// @brief ���[���h���W�ł̒��S�i�L���b�V���j
    MATRIX world_rotation_;  /// @brief ���[���h��]�i�L���b�V���j
};