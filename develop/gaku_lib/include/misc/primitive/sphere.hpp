#pragma once
#include "point.hpp"

/// @brief Primitive: 3D��
class Sphere : public Primitive
{
public:
	/// @brief �e�v�f�� 0.0 ��ݒ肵�ăC���X�^���X��������
	Sphere()
		: position_(0.0f, 0.0f, 0.0f)
		, radius_(0.0f)
	{
	}

	/// @brief �e�v�f�� 0.0 ��ݒ肵�ăC���X�^���X��������
	Sphere(const Point& position, float radius)
		: position_(position)
		, radius_(radius)
	{
	}

	/// @brief �C���X�^���X�����
	~Sphere() override = default;

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
		DrawSphere3D(position_, radius_, 16, Primitive::GetColor(), 0xffffff, filling);
	}

public:
	Point position_;	/// @brief ���W
	float radius_;		/// @brief ���a

};