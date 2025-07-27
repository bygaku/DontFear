#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <functional>
#include "object/collidable_object.hpp"
#include "misc/primitive/aabb.hpp"

/// @brief 八分木による空間分割システム
class SpatialOctree {
public:
    static constexpr int kMaxDepth = 6;          /// @brief 最大深度
    static constexpr int kMaxObjectsPerNode = 8; /// @brief ノードあたりの最大オブジェクト数

private:
    struct OctreeNode {
        AABB bounds_;                                    /// @brief このノードの境界
        std::vector<CollidableObject*> objects_;         /// @brief このノードに属するオブジェクト
        std::unique_ptr<OctreeNode>    children_[8];     /// @brief 子ノード
        int depth_;                                      /// @brief 現在の深度
        bool is_leaf_;                                   /// @brief 末端ノードかどうか

        OctreeNode(const AABB& bounds, int depth)
            : bounds_(bounds), depth_(depth), is_leaf_(true) {
        }

        /// @brief 子ノードを作成
        void Subdivide();

        /// @brief オブジェクトを挿入
        void Insert(CollidableObject* object, const AABB& object_aabb);

        /// @brief 潜在的な衝突ペアを取得
        void GetPotentialPairs(CollidableObject* object, const AABB& object_aabb,
            std::vector<CollidableObject*>& potential_colliders);

        /// @brief ノードをクリア
        void Clear();

        /// @brief 子ノードのインデックスを取得
        int GetChildIndex(const AABB& aabb) const;
    };

public:
    /// @brief コンストラクタ
    SpatialOctree(const VECTOR& world_min, const VECTOR& world_max);

    /// @brief デストラクタ
    ~SpatialOctree() = default;

    /// @brief 八分木を再構築
    void Build(const std::vector<CollidableObject*>& objects);

    /// @brief オブジェクトの潜在的な衝突相手を取得
    std::vector<CollidableObject*> GetPotentialColliders(CollidableObject* object);

    /// @brief 全ての潜在的な衝突ペアを取得
    std::vector<std::pair<CollidableObject*, CollidableObject*>> GetAllPotentialPairs();

    /// @brief デバッグ描画
    void DebugDraw() const;

private:
    std::unique_ptr<OctreeNode> root_;   /// @brief ルートノード
    AABB world_bounds_;                  /// @brief ワールド境界

    /// @brief ノードのデバッグ描画（再帰）
    void DebugDrawNode(const OctreeNode* node, int color_intensity) const;

};