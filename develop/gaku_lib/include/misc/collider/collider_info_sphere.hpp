#pragma once
#include "collider_data.hpp"
#include "misc/primitive/sphere.hpp"

/// @brief 線分判定情報
class ColliderInfoSphere : public ColliderInfo
{
public:
	/// @remark 禁止コンストラクタ
	ColliderInfoSphere() = delete;

	/// @brief パラメーターを設定してインスタンスを初期化
	ColliderInfoSphere(bool is_trigger);

	/// @brief インスタンスを解放
	~ColliderInfoSphere() override = default;

    /// @brief AABBを更新
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief 詳細な衝突判定
    bool TestCollision(const ColliderInfo* other,
                       const VECTOR& pos_a,
                       const VECTOR& pos_b,
                       VECTOR& hit_point,
                       VECTOR& hit_normal,
                       float& penetration) const override;

    /// @brief 球のパラメータを設定
    void SetSphere(const VECTOR& local_center, float radius);

    /// @brief ローカル中心を取得
    const VECTOR& GetLocalCenter() const { return sphere_.position_; }

    /// @brief ワールド中心を取得
    const VECTOR& GetWorldCenter() const { return world_center_;     }

    /// @brief 半径を取得
    float GetRadius()              const { return sphere_.radius_;   }

    /// @brief 体積を取得
    float GetVolume() const;

    /// @brief 表面積を取得
    float GetSurfaceArea() const;

    /// @brief 点が球内にあるか判定
    bool ContainsPoint(const VECTOR& point) const;

    /// @brief 球表面上の最近点を取得
    VECTOR GetClosestPoint(const VECTOR& point) const;

    /// @brief サポート写像（GJK用）
    VECTOR GetSupportPoint(const VECTOR& direction) const;

public:
	Sphere sphere_;
    VECTOR world_center_;

};