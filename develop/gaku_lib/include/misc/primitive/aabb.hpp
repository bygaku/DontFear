#pragma once
#include <algorithm>
#include "point.hpp"

/// @brief Primitive: AABB
class AABB : public Primitive
{
public:
#undef min
#undef max
    VECTOR min_;  /// @brief �ŏ����W
    VECTOR max_;  /// @brief �ő���W

    /// @brief �e�v�f�� 0.0 ��ݒ肵�ăC���X�^���X��������
    AABB() 
        : min_(VGet(0.f, 0.f, 0.f))
        , max_(VGet(0.f, 0.f, 0.f)) 
    {
    }

    /// @brief �e�p�����[�^���w�肵�ăC���X�^���X��������
    /// @param min : �ŏ����W
    /// @param max : �ő���W
    AABB(const VECTOR& min, const VECTOR& max)
        : min_(min)
        , max_(max) 
    {
    }

    /// @brief �C���X�^���X�����
    ~AABB() override = default;

    /// @brief �}�`��������
    /// @param color_code : [ �f�t�H���g�l...�� ]
    void Initialize(uint32_t color_code = 0xffffff) noexcept override
    {
        Primitive::SetColor(color_code);
    }

    /// @brief �}�`�̃f�o�b�O�`��
    /// @param filling : ���b�V���𖄂߂邩 [ �f�t�H���g�l...���߂Ȃ� ]
    void Debug(bool filling = false)          const noexcept override
    {
    }

    /// @brief ���S���W���擾
    VECTOR GetCenter()   const { return VScale(VAdd(min_, max_), 0.5f); }

    /// @brief �T�C�Y���擾
    VECTOR GetSize()     const { return VSub(max_, min_); }

    /// @brief �����̃T�C�Y���擾
    VECTOR GetHalfSize() const { return VScale(GetSize(), 0.5f); }

    /// @brief AABB���ړ�
    void Translate(const VECTOR& offset) 
    {
        min_ = min_ + offset;
        max_ = max_ + offset;
    }

    /// @brief AABB���g��
    void Expand(float margin) 
    {
        VECTOR expand_vec = VGet(margin, margin, margin);
        min_ = min_ - expand_vec;
        max_ = max_ + expand_vec;
    }

    /// @brief �_��AABB���ɂ��邩����
    bool Contains(const VECTOR& point) const
    {
        return point.x >= min_.x && point.x <= max_.x &&
               point.y >= min_.y && point.y <= max_.y &&
               point.z >= min_.z && point.z <= max_.z;
    }

    /// @brief ����AABB�ƌ������Ă��邩����
    bool Intersects(const AABB& other) const
    {
        return min_.x <= other.max_.x && max_.x >= other.min_.x &&
               min_.y <= other.max_.y && max_.y >= other.min_.y &&
               min_.z <= other.max_.z && max_.z >= other.min_.z;
    }

    /// @brief ����AABB�ƃ}�[�W
    void Merge(const AABB& other)
    {
        min_.x = std::min(min_.x, other.min_.x);
        min_.y = std::min(min_.y, other.min_.y);
        min_.z = std::min(min_.z, other.min_.z);
        max_.x = std::max(max_.x, other.max_.x);
        max_.y = std::max(max_.y, other.max_.y);
        max_.z = std::max(max_.z, other.max_.z);
    }

    /// @brief �J�v�Z������AABB�𐶐�
    static AABB FromCapsule(const VECTOR& start, const VECTOR& end, float radius)
    {
        VECTOR min = VGet(
            std::min(start.x, end.x) - radius,
            std::min(start.y, end.y) - radius,
            std::min(start.z, end.z) - radius
        );
        
        VECTOR max = VGet(
            std::max(start.x, end.x) + radius,
            std::max(start.y, end.y) + radius,
            std::max(start.z, end.z) + radius
        );

        return AABB(min, max);
    }

    /// @brief ������AABB�𐶐�
    static AABB FromSphere(const VECTOR& center, float radius)
    {
        VECTOR half_size = VGet(radius, radius, radius);
        return AABB(center, half_size);
    }

    /// @brief OBB����AABB�𐶐�
    /// @remark �ȈՔ�
    static AABB FromOBB(const VECTOR& center, const VECTOR& half_size, const MATRIX& rotation)
    {
        // ��]���l�������ő�͈͂��v�Z
        float max_x = fabsf(rotation.m[0][0] * half_size.x) +
                      fabsf(rotation.m[0][1] * half_size.y) +
                      fabsf(rotation.m[0][2] * half_size.z);

        float max_y = fabsf(rotation.m[1][0] * half_size.x) +
                      fabsf(rotation.m[1][1] * half_size.y) +
                      fabsf(rotation.m[1][2] * half_size.z);

        float max_z = fabsf(rotation.m[2][0] * half_size.x) +
                      fabsf(rotation.m[2][1] * half_size.y) +
                      fabsf(rotation.m[2][2] * half_size.z);

        VECTOR max_half = VGet(max_x, max_y, max_z);

        return AABB(center, max_half);
    }
};