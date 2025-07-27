#pragma once
#include "collider_data.hpp"
#include "misc/primitive/capsule.hpp"

/// @brief カプセル判定情報
class ColliderInfoCapsule : public ColliderInfo
{
public:
	/// @remark 禁止コンストラクタ
	ColliderInfoCapsule() = delete;

	/// @brief パラメーターを設定してインスタンスを初期化
	ColliderInfoCapsule(bool is_trigger);

	/// @brief インスタンスを解放
	~ColliderInfoCapsule() override = default;

    /// @brief AABBを更新
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief 詳細な衝突判定
    bool TestCollision(const ColliderInfo* other,
                       const VECTOR& pos_a,
                       const VECTOR& pos_b,
                       VECTOR& hit_point,
                       VECTOR& hit_normal, 
                       float& penetration) const override;

    /// @brief カプセルのパラメータを設定
    void SetCapsule(const VECTOR& local_start, const VECTOR& local_end, float radius);

    /// @brief ワールド座標でのカプセル始点を取得
    VECTOR GetWorldStart()          const noexcept { return capsule_.world_start_; }

    /// @brief ワールド座標でのカプセル終点を取得
    VECTOR GetWorldEnd()            const noexcept { return capsule_.world_end_;   }

    /// @brief 半径を取得
    float GetRadius()               const noexcept { return capsule_.radius_;      }

    /// @brief ローカル座標での始点を取得
    const VECTOR& GetLocalStart()   const noexcept { return capsule_.local_start_; }

    /// @brief ローカル座標での終点を取得
    const VECTOR& GetLocalEnd()     const noexcept { return capsule_.local_end_;   }

    /// @brief カプセルの高さを取得（球の中心間の距離）
    float GetHeight()               const noexcept;

    /// @brief カプセルの全長を取得（端から端まで）
    float GetTotalHeight()          const noexcept;

public:
	Capsule capsule_;

};