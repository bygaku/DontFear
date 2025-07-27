#pragma once
#include "collider_data.hpp"
#include "misc/primitive/segment.hpp"

/// @brief 線分判定情報
class ColliderInfoSegment : public ColliderInfo 
{
public:
    /// @remark 禁止コンストラクタ
    ColliderInfoSegment() = delete;

	/// @brief パラメーターを設定してインスタンスを初期化
    ColliderInfoSegment(bool is_trigger, uint32_t category = Category::kAll,
        uint32_t mask = Category::kAll);

    /// @brief 線分パラメータ付きコンストラクタ
    ColliderInfoSegment(const VECTOR& local_start, const VECTOR& local_end,
        float thickness, bool is_trigger,
        uint32_t category = Category::kAll,
        uint32_t mask = Category::kAll);

    /// @brief インスタンスを解放
    ~ColliderInfoSegment() override = default;

    /// @brief AABBを更新
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief 詳細な衝突判定
    bool TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const override;

    /// @brief 線分のパラメータを設定
    void SetSegment(const VECTOR& local_start, const VECTOR& local_end, float thickness = 0.0f);

    /// @brief ローカル座標での始点を取得
    const VECTOR& GetLocalStart() const { return local_start_; }

    /// @brief ローカル座標での終点を取得
    const VECTOR& GetLocalEnd() const { return local_end_; }

    /// @brief ワールド座標での始点を取得
    const VECTOR& GetWorldStart() const { return world_start_; }

    /// @brief ワールド座標での終点を取得
    const VECTOR& GetWorldEnd() const { return world_end_; }

    /// @brief 太さを取得（0の場合は無限に細い線）
    float GetThickness() const { return thickness_; }

    /// @brief 線分の長さを取得
    float GetLength() const;

    /// @brief 線分の方向ベクトル（正規化）を取得
    VECTOR GetDirection() const;

    /// @brief 点との最短距離を取得
    float GetDistanceToPoint(const VECTOR& point) const;

    /// @brief 線分上の最近点を取得
    VECTOR GetClosestPoint(const VECTOR& point) const;

    /// @brief レイキャストのような使用のためのヘルパー
    bool Raycast(const VECTOR& target_point, float& t) const;

private:
    // ローカル座標系での線分定義
    VECTOR local_start_;    /// @brief ローカル座標での始点
    VECTOR local_end_;      /// @brief ローカル座標での終点
    float thickness_;       /// @brief 線分の太さ（0なら無限に細い）

    // ワールド座標系での線分（キャッシュ）
    VECTOR world_start_;    /// @brief ワールド座標での始点
    VECTOR world_end_;      /// @brief ワールド座標での終点
};