#pragma once
#include "line.hpp"

/// @brief Primitive: 3D����
class Segment : public Line
{
public:
	/// @brief �e�v�f�� 0.0 ��ݒ肵�ăC���X�^���X��������
	Segment()
		: Line { /* nothing */}
	{
	}

	/// @brief �e�v�f���w�肵�ăC���X�^���X��������
	/// @param start_position : �n�_�̍��W
	/// @param end_position   : �I�_�̍��W
	Segment(const Point& start_position, const Point& end_position)
		: Line { start_position, end_position - start_position }
	{
	}

	/// @brief �C���X�^���X�����
	~Segment() override = default;

	/// @brief �}�`��������
	/// @param color_code : [ �f�t�H���g�l...�� ]
	void Initialize(uint32_t color_code = 0xffffff) noexcept override
	{
		Primitive::SetColor(color_code);
	}

	/// @brief �}�`�̃f�o�b�O�`��
	/// @param filling : ���b�V���𖄂߂邩 [ �f�t�H���g�l...���߂Ȃ� ]
	void Debug(bool filling = false)		  const noexcept override
	{
		DrawLine3D(position_, GetEndPoint(), Primitive::GetColor());
	}

	/// @brief �I�_���擾
	VECTOR GetEndPoint() const noexcept
	{
		return position_ + direction_;
	}
};
