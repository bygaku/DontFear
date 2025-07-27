#pragma once
#include "segment.hpp"

/// @brief Primitive: 3Dカプセル
class Capsule : public Primitive
{
public:
    /// @brief 各要素に 0.0 を設定してインスタンスを初期化
    Capsule()
        : radius_(0.f)
        , local_start_(VGet(0.f, 0.f, 0.f))
        , local_end_(VGet(0.f, 0.f, 0.f))
        , world_start_(local_start_)
        , world_end_(local_end_)
    {
    }

    /// @brief 各パラメータを指定してインスタンスを初期化
    /// @param seg : 線分Primitive
    /// @param rad : 半径
    Capsule(const Segment& seg, float rad)
        : radius_(rad)
        , local_start_(seg.position_)
        , local_end_(seg.GetEndPoint())
        , world_start_(local_start_)
        , world_end_(local_end_)
    {
    }

    /// @brief 各パラメータを指定してインスタンスを初期化
    /// @param local_start : カプセルの始点
    /// @param local_end   : カプセルの終点
    /// @param rad         : 半径
    Capsule(const Point& local_start, const Point& local_end, float rad)
        : radius_(rad)
        , local_start_(local_start)
        , local_end_(local_end)
        , world_start_(local_start)
        , world_end_(local_end)
    {
    }

    /// @brief インスタンスを解放
    ~Capsule() override = default;

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
        DrawCapsule3D(local_start_, local_end_, radius_, 16, Primitive::GetColor(), 0xffffff, filling);
    }

public:
    //! ローカル座標系でのカプセル定義
    VECTOR local_start_;    /// @brief ローカル座標での始点
    VECTOR local_end_;      /// @brief ローカル座標での終点
    float  radius_;         /// @brief 半径

    //! ワールド座標系でのカプセル（キャッシュ）
    VECTOR world_start_;    /// @brief ワールド座標での始点
    VECTOR world_end_;      /// @brief ワールド座標での終点

};
