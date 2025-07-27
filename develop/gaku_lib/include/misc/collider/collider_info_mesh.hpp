#pragma once
#include <vector>
#include "collider_data.hpp"

/// @brief メッシュ判定情報
class ColliderInfoMesh : public ColliderInfo 
{
public:
    /// @brief 三角形データ
    struct Triangle {
        VECTOR   v0_, v1_, v2_;     /// @brief 頂点
        VECTOR   normal_;           /// @brief 法線
        AABB     aabb_;             /// @brief この三角形のAABB
        uint32_t material_id_;      /// @brief マテリアルID
    };

    /// @brief BVH（Bounding Volume Hierarchy）ノード
    struct BVHNode {
        AABB bounds_;
        int left_child_;    // -1なら葉ノード
        int right_child_;
        int triangle_index_; // 葉ノードの場合の三角形インデックス
    };

public:
    /// @brief 禁止コンストラクタ
    ColliderInfoMesh() = delete;

    /// @brief コンストラクタ
    ColliderInfoMesh(bool is_trigger, uint32_t category = Category::kAll, uint32_t mask = Category::kAll);

    /// @brief デストラクタ
    ~ColliderInfoMesh() override = default;

    /// @brief AABBを更新
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief 詳細な衝突判定
    bool TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const override;

    /// @brief DxLibのモデルからメッシュデータを構築
    void BuildFromModel(int model_handle);

    /// @brief 頂点とインデックスからメッシュデータを構築
    void BuildFromVertices(const std::vector<VECTOR>& vertices,
        const std::vector<int>& indices);

    /// @brief 三角形データを直接設定
    void SetTriangles(const std::vector<Triangle>& triangles);

    /// @brief 三角形リストを取得
    const std::vector<Triangle>& GetTriangles() const { return triangles_; }

    /// @brief BVHを構築（高速化）
    void BuildBVH();

    /// @brief BVHを使用するか設定
    void SetUseBVH(bool use) { use_bvh_ = use; }
    bool GetUseBVH() const { return use_bvh_; }

    /// @brief 最適化レベルを設定
    void SetOptimizationLevel(int level) { optimization_level_ = level; }

    /// @brief レイキャスト（メッシュ専用）
    bool Raycast(const VECTOR& origin, const VECTOR& direction, float max_distance,
        VECTOR& hit_point, VECTOR& hit_normal, int& hit_triangle_index) const;

    /// @brief 球との交差する三角形を取得
    std::vector<int> GetTrianglesInSphere(const VECTOR& center, float radius) const;

    /// @brief メッシュの統計情報を取得
    void GetStatistics(int& triangle_count, float& surface_area, VECTOR& centroid) const;

private:
    /// @brief BVHを使った衝突判定
    bool TestCollisionWithBVH(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const;

    /// @brief BVHノードとの交差判定
    bool TestBVHNode(int node_index, const AABB& test_aabb,
        std::vector<int>& potential_triangles) const;

    /// @brief 三角形の法線を計算
    static VECTOR CalculateTriangleNormal(const VECTOR& v0, const VECTOR& v1, const VECTOR& v2);

    /// @brief 三角形のAABBを計算
    static AABB CalculateTriangleAABB(const VECTOR& v0, const VECTOR& v1, const VECTOR& v2);

private:
    std::vector<Triangle> triangles_;        /// @brief 三角形リスト
    std::vector<BVHNode> bvh_nodes_;        /// @brief BVHノード
    bool use_bvh_;                          /// @brief BVHを使用するか
    int optimization_level_;                 /// @brief 最適化レベル（0-3）

    // キャッシュ
    VECTOR world_position_;                  /// @brief ワールド位置
    MATRIX world_rotation_;                  /// @brief ワールド回転
};