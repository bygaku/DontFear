#pragma once
#include "collider_data.hpp"

/// @brief ボックス（OBB）判定情報
class ColliderInfoBox : public ColliderInfo 
{
public:
    /// @brief デフォルトコンストラクタ（削除）
    ColliderInfoBox() = delete;

    /// @brief コンストラクタ
    ColliderInfoBox(bool is_trigger, uint32_t category = Category::kAll,
        uint32_t mask = Category::kAll);

    /// @brief パラメータ付きコンストラクタ
    ColliderInfoBox(const VECTOR& local_center, const VECTOR& half_size, bool is_trigger,
        uint32_t category = Category::kAll, uint32_t mask = Category::kAll);

    /// @brief デストラクタ
    ~ColliderInfoBox() override = default;

    /// @brief AABBを更新
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief 詳細な衝突判定
    bool TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const override;

    /// @brief ボックスのパラメータを設定
    void SetBox(const VECTOR& local_center, const VECTOR& half_size);

    /// @brief ローカル中心を取得
    const VECTOR& GetLocalCenter() const { return local_center_; }

    /// @brief ワールド中心を取得
    const VECTOR& GetWorldCenter() const { return world_center_; }

    /// @brief 半分のサイズを取得
    const VECTOR& GetHalfSize() const { return half_size_; }

    /// @brief ワールド回転を取得
    const MATRIX& GetWorldRotation() const { return world_rotation_; }

    /// @brief ボックスの8頂点を取得（ワールド座標）
    void GetWorldVertices(VECTOR vertices[8]) const;

    /// @brief ボックスの6面の法線を取得（ワールド座標）
    void GetWorldNormals(VECTOR normals[6]) const;

    /// @brief 点がボックス内にあるか判定
    bool ContainsPoint(const VECTOR& point) const;

    /// @brief ボックス表面上の最近点を取得
    VECTOR GetClosestPoint(const VECTOR& point) const;

    /// @brief サポート写像（GJK用）
    VECTOR GetSupportPoint(const VECTOR& direction) const;

private:
    VECTOR local_center_;    /// @brief ローカル座標での中心
    VECTOR half_size_;       /// @brief 各軸の半分のサイズ
    VECTOR world_center_;    /// @brief ワールド座標での中心（キャッシュ）
    MATRIX world_rotation_;  /// @brief ワールド回転（キャッシュ）
};