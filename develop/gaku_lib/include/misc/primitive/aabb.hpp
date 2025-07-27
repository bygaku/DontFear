#pragma once
#include <algorithm>
#include "point.hpp"

/// @brief Primitive: AABB
class AABB : public Primitive
{
public:
#undef min
#undef max
    VECTOR min_;  /// @brief 最小座標
    VECTOR max_;  /// @brief 最大座標

    /// @brief 各要素に 0.0 を設定してインスタンスを初期化
    AABB() 
        : min_(VGet(0.f, 0.f, 0.f))
        , max_(VGet(0.f, 0.f, 0.f)) 
    {
    }

    /// @brief 各パラメータを指定してインスタンスを初期化
    /// @param min : 最小座標
    /// @param max : 最大座標
    AABB(const VECTOR& min, const VECTOR& max)
        : min_(min)
        , max_(max) 
    {
    }

    /// @brief インスタンスを解放
    ~AABB() override = default;

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

    /// @brief 中心座標を取得
    VECTOR GetCenter()   const { return VScale(VAdd(min_, max_), 0.5f); }

    /// @brief サイズを取得
    VECTOR GetSize()     const { return VSub(max_, min_); }

    /// @brief 半分のサイズを取得
    VECTOR GetHalfSize() const { return VScale(GetSize(), 0.5f); }

    /// @brief AABBを移動
    void Translate(const VECTOR& offset) 
    {
        min_ = min_ + offset;
        max_ = max_ + offset;
    }

    /// @brief AABBを拡張
    void Expand(float margin) 
    {
        VECTOR expand_vec = VGet(margin, margin, margin);
        min_ = min_ - expand_vec;
        max_ = max_ + expand_vec;
    }

    /// @brief 点がAABB内にあるか判定
    bool Contains(const VECTOR& point) const
    {
        return point.x >= min_.x && point.x <= max_.x &&
               point.y >= min_.y && point.y <= max_.y &&
               point.z >= min_.z && point.z <= max_.z;
    }

    /// @brief 他のAABBと交差しているか判定
    bool Intersects(const AABB& other) const
    {
        return min_.x <= other.max_.x && max_.x >= other.min_.x &&
               min_.y <= other.max_.y && max_.y >= other.min_.y &&
               min_.z <= other.max_.z && max_.z >= other.min_.z;
    }

    /// @brief 他のAABBとマージ
    void Merge(const AABB& other)
    {
        min_.x = std::min(min_.x, other.min_.x);
        min_.y = std::min(min_.y, other.min_.y);
        min_.z = std::min(min_.z, other.min_.z);
        max_.x = std::max(max_.x, other.max_.x);
        max_.y = std::max(max_.y, other.max_.y);
        max_.z = std::max(max_.z, other.max_.z);
    }

    /// @brief カプセルからAABBを生成
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

    /// @brief 球からAABBを生成
    static AABB FromSphere(const VECTOR& center, float radius)
    {
        VECTOR half_size = VGet(radius, radius, radius);
        return AABB(center, half_size);
    }

    /// @brief OBBからAABBを生成
    /// @remark 簡易版
    static AABB FromOBB(const VECTOR& center, const VECTOR& half_size, const MATRIX& rotation)
    {
        // 回転を考慮した最大範囲を計算
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