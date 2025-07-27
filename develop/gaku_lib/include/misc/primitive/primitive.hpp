#pragma once
#include <DxLib.h>
#include <cstdint>
#include "../math/math.hpp"

using namespace dx_math;

/// @brief 図形インターフェイス
class Primitive abstract
{
public:
    /// @brief インスタンスを初期化
    Primitive()
        : color_(0xffffff)
    {
    }

    /// @brief インスタンスを解放
    virtual ~Primitive() = default;

    /// @brief 図形を初期化
    /// @param color_code : [ デフォルト値...白 ]
    virtual void Initialize(uint32_t color_code = 0xffffff) noexcept abstract;

    /// @brief 図形のデバッグ描画
    /// @param filling : メッシュを埋めるか [ デフォルト値...埋めない ]
    virtual void Debug(bool filling = false)          const noexcept abstract;

    /// @brief 色を設定
    /// @param color_code Default::0xffffff
    void SetColor(uint32_t color_code) noexcept
    {
        color_ = color_code;
    }

protected:
    /// @brief 色を設定
    /// @param color_code Default::0xffffff
    uint32_t GetColor() const noexcept
    {
        return color_;
    }

private:
    uint32_t    color_;

};
