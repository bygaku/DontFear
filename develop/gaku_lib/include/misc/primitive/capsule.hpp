#pragma once
#include "segment.hpp"

/// @brief Primitive: 3D�J�v�Z��
class Capsule : public Primitive
{
public:
    /// @brief �e�v�f�� 0.0 ��ݒ肵�ăC���X�^���X��������
    Capsule()
        : radius_(0.f)
        , local_start_(VGet(0.f, 0.f, 0.f))
        , local_end_(VGet(0.f, 0.f, 0.f))
        , world_start_(local_start_)
        , world_end_(local_end_)
    {
    }

    /// @brief �e�p�����[�^���w�肵�ăC���X�^���X��������
    /// @param seg : ����Primitive
    /// @param rad : ���a
    Capsule(const Segment& seg, float rad)
        : radius_(rad)
        , local_start_(seg.position_)
        , local_end_(seg.GetEndPoint())
        , world_start_(local_start_)
        , world_end_(local_end_)
    {
    }

    /// @brief �e�p�����[�^���w�肵�ăC���X�^���X��������
    /// @param local_start : �J�v�Z���̎n�_
    /// @param local_end   : �J�v�Z���̏I�_
    /// @param rad         : ���a
    Capsule(const Point& local_start, const Point& local_end, float rad)
        : radius_(rad)
        , local_start_(local_start)
        , local_end_(local_end)
        , world_start_(local_start)
        , world_end_(local_end)
    {
    }

    /// @brief �C���X�^���X�����
    ~Capsule() override = default;

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
        DrawCapsule3D(local_start_, local_end_, radius_, 16, Primitive::GetColor(), 0xffffff, filling);
    }

public:
    //! ���[�J�����W�n�ł̃J�v�Z����`
    VECTOR local_start_;    /// @brief ���[�J�����W�ł̎n�_
    VECTOR local_end_;      /// @brief ���[�J�����W�ł̏I�_
    float  radius_;         /// @brief ���a

    //! ���[���h���W�n�ł̃J�v�Z���i�L���b�V���j
    VECTOR world_start_;    /// @brief ���[���h���W�ł̎n�_
    VECTOR world_end_;      /// @brief ���[���h���W�ł̏I�_

};
