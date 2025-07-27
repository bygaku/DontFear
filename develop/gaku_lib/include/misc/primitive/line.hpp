#pragma once
#include "point.hpp"

/// @brief Primitive: 3D直線
class Line : public Primitive
{
public:
    /// @brief 各要素に 0.0 を設定してインスタンスを初期化
    Line()
        : position_(VGet(0.f, 0.f, 0.f))
        , direction_(VGet(0.f, 0.f, 0.f))
    {
    }

    /// @brief 各パラメータを指定してインスタンスを初期化
    /// @param position : 直線上の一点
    /// @param dir      : 向きベクトル
    Line(const Point& position, const VECTOR& dir)
        : position_(position)
        , direction_(dir)
    {
    }

    /// @brief インスタンスを解放
    ~Line() override = default;

    /// @brief 図形を初期化
    /// @param color_code : [ デフォルト値...白 ]
    void Initialize(uint32_t color_code = 0xffffff) noexcept override
    {
        Primitive::SetColor(color_code);
    }

    /// @brief 図形のデバッグ描画
    /// @param filling : メッシュを埋めるか [ デフォルト値...埋めない ]
    void Debug(bool filling = false)          const noexcept override
    {
    }

    /// @brief 直線上の一点を設定
    void SetPoint(const VECTOR& position) noexcept
    {
        position_ = position;
    }

    /// @brief 直線の向きを設定
    void SetDirection(const VECTOR& dir)  noexcept
    {
        direction_ = dir;
    }

public:
    Point  position_;       /// @brief 直線上の一点
    VECTOR direction_;      /// @brief 向きベクトル

};