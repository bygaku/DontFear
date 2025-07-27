#pragma once
#include "point.hpp"

/// @brief Primitive: 3D����
class Line : public Primitive
{
public:
    /// @brief �e�v�f�� 0.0 ��ݒ肵�ăC���X�^���X��������
    Line()
        : position_(VGet(0.f, 0.f, 0.f))
        , direction_(VGet(0.f, 0.f, 0.f))
    {
    }

    /// @brief �e�p�����[�^���w�肵�ăC���X�^���X��������
    /// @param position : ������̈�_
    /// @param dir      : �����x�N�g��
    Line(const Point& position, const VECTOR& dir)
        : position_(position)
        , direction_(dir)
    {
    }

    /// @brief �C���X�^���X�����
    ~Line() override = default;

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

    /// @brief ������̈�_��ݒ�
    void SetPoint(const VECTOR& position) noexcept
    {
        position_ = position;
    }

    /// @brief �����̌�����ݒ�
    void SetDirection(const VECTOR& dir)  noexcept
    {
        direction_ = dir;
    }

public:
    Point  position_;       /// @brief ������̈�_
    VECTOR direction_;      /// @brief �����x�N�g��

};