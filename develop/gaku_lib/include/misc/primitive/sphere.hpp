#pragma once
#include "point.hpp"

/// @brief Primitive: 3D球
class Sphere : public Primitive
{
public:
	/// @brief 各要素に 0.0 を設定してインスタンスを初期化
	Sphere()
		: position_(0.0f, 0.0f, 0.0f)
		, radius_(0.0f)
	{
	}

	/// @brief 各要素に 0.0 を設定してインスタンスを初期化
	Sphere(const Point& position, float radius)
		: position_(position)
		, radius_(radius)
	{
	}

	/// @brief インスタンスを解放
	~Sphere() override = default;

	/// @brief 図形を初期化
	/// @param color_code : [ デフォルト値...白 ]
	void Initialize(uint32_t color_code = 0xffffff) noexcept override
	{
		Primitive::SetColor(color_code);
	}

	/// @brief 図形のデバッグ描画
	/// @param filling : メッシュを埋めるか [ デフォルト値...埋めない ]
	void Debug(bool filling = false)		  const noexcept override
	{
		DrawSphere3D(position_, radius_, 16, Primitive::GetColor(), 0xffffff, filling);
	}

public:
	Point position_;	/// @brief 座標
	float radius_;		/// @brief 半径

};