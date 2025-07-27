#pragma once
#include "line.hpp"

/// @brief Primitive: 3D線分
class Segment : public Line
{
public:
	/// @brief 各要素に 0.0 を設定してインスタンスを初期化
	Segment()
		: Line { /* nothing */}
	{
	}

	/// @brief 各要素を指定してインスタンスを初期化
	/// @param start_position : 始点の座標
	/// @param end_position   : 終点の座標
	Segment(const Point& start_position, const Point& end_position)
		: Line { start_position, end_position - start_position }
	{
	}

	/// @brief インスタンスを解放
	~Segment() override = default;

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
		DrawLine3D(position_, GetEndPoint(), Primitive::GetColor());
	}

	/// @brief 終点を取得
	VECTOR GetEndPoint() const noexcept
	{
		return position_ + direction_;
	}
};
