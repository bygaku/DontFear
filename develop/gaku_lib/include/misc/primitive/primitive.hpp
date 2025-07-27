#pragma once
#include <DxLib.h>
#include <cstdint>
#include "../math/math.hpp"

using namespace dx_math;

/// @brief �}�`�C���^�[�t�F�C�X
class Primitive abstract
{
public:
    /// @brief �C���X�^���X��������
    Primitive()
        : color_(0xffffff)
    {
    }

    /// @brief �C���X�^���X�����
    virtual ~Primitive() = default;

    /// @brief �}�`��������
    /// @param color_code : [ �f�t�H���g�l...�� ]
    virtual void Initialize(uint32_t color_code = 0xffffff) noexcept abstract;

    /// @brief �}�`�̃f�o�b�O�`��
    /// @param filling : ���b�V���𖄂߂邩 [ �f�t�H���g�l...���߂Ȃ� ]
    virtual void Debug(bool filling = false)          const noexcept abstract;

    /// @brief �F��ݒ�
    /// @param color_code Default::0xffffff
    void SetColor(uint32_t color_code) noexcept
    {
        color_ = color_code;
    }

protected:
    /// @brief �F��ݒ�
    /// @param color_code Default::0xffffff
    uint32_t GetColor() const noexcept
    {
        return color_;
    }

private:
    uint32_t    color_;

};
