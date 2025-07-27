#include <DxLib.h>
#include <algorithm>
#include <queue>
#include "misc/collider/collider_info_mesh.hpp"
#include "misc/collider/collision_dispatcher.hpp"
#include "misc/collider/collision_algorithms.hpp"

ColliderInfoMesh::ColliderInfoMesh(bool is_trigger, uint32_t category, uint32_t mask)
    : ColliderInfo(Type::Mesh, is_trigger, category, mask)
    , use_bvh_(false)
    , optimization_level_(1)
    , world_position_(VGet(0, 0, 0))
    , world_rotation_(MGetIdent())
{
}

void ColliderInfoMesh::UpdateAABB(const VECTOR& position, const MATRIX& rotation)
{
    world_position_ = position;
    world_rotation_ = rotation;

    // 全体のAABBを再計算
    if (triangles_.empty()) {
        aabb_ = AABB();
        SetNeedsUpdate(false);
        return;
    }

    // 最初の三角形で初期化
    VECTOR v = VAdd(position, VTransform(triangles_[0].v0_, rotation));
    aabb_ = AABB(v, v);

    // 全ての頂点を含むようにAABBを拡張
    for (const auto& tri : triangles_) {
        VECTOR v0 = VAdd(position, VTransform(tri.v0_, rotation));
        VECTOR v1 = VAdd(position, VTransform(tri.v1_, rotation));
        VECTOR v2 = VAdd(position, VTransform(tri.v2_, rotation));

        aabb_.Merge(AABB(v0, v0));
        aabb_.Merge(AABB(v1, v1));
        aabb_.Merge(AABB(v2, v2));
    }

    SetNeedsUpdate(false);
}

bool ColliderInfoMesh::TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
    const VECTOR& pos_b, VECTOR& hit_point,
    VECTOR& hit_normal, float& penetration) const
{
    // BVHを使う場合
    if (use_bvh_ && !bvh_nodes_.empty()) {
        return TestCollisionWithBVH(other, pos_a, pos_b, hit_point, hit_normal, penetration);
    }

    // CollisionDispatcherに処理を委譲
    return CollisionDispatcher::GetInstance().TestCollision(
        this, other, pos_a, pos_b, hit_point, hit_normal, penetration
    );
}

void ColliderInfoMesh::BuildFromModel(int model_handle)
{
    triangles_.clear();

    // モデルの参照メッシュ数を取得
    int mesh_num = MV1GetMeshNum(model_handle);

    for (int mesh_idx = 0; mesh_idx < mesh_num; mesh_idx++) {
        // メッシュの三角形数を取得
        int tri_num = MV1GetMeshTriangleNum(model_handle, mesh_idx);

        for (int tri_idx = 0; tri_idx < tri_num; tri_idx++) {
            Triangle tri;

            // 三角形の頂点を取得
            auto tri_ = MV1GetReferenceMesh(model_handle, mesh_idx, tri_idx, 0);
            
            //TODO: tri.v0_ = MV1GetReferenceMesh(model_handle, mesh_idx, tri_idx, 0).Vertexs[tri_num].Position;
            //TODO: tri.v1_ = MV1GetReferenceMesh(model_handle, mesh_idx, tri_idx, 1);
            //TODO: tri.v2_ = MV1GetReferenceMesh(model_handle, mesh_idx, tri_idx, 2);

            // 法線を計算
            tri.normal_ = CalculateTriangleNormal(tri.v0_, tri.v1_, tri.v2_);

            // AABBを計算
            tri.aabb_ = CalculateTriangleAABB(tri.v0_, tri.v1_, tri.v2_);

            // マテリアルID
            tri.material_id_ = static_cast<uint32_t>(mesh_idx);

            triangles_.push_back(tri);
        }
    }

    SetNeedsUpdate(true);

    // 自動的にBVHを構築（大きなメッシュの場合）
    if (triangles_.size() > 100 && optimization_level_ > 0) {
        BuildBVH();
    }
}

void ColliderInfoMesh::BuildFromVertices(const std::vector<VECTOR>& vertices,
    const std::vector<int>& indices)
{
    triangles_.clear();

    // インデックスは3の倍数である必要がある
    if (indices.size() % 3 != 0) {
        return;
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        Triangle tri;

        tri.v0_ = vertices[indices[i]];
        tri.v1_ = vertices[indices[i + 1]];
        tri.v2_ = vertices[indices[i + 2]];

        tri.normal_ = CalculateTriangleNormal(tri.v0_, tri.v1_, tri.v2_);
        tri.aabb_ = CalculateTriangleAABB(tri.v0_, tri.v1_, tri.v2_);
        tri.material_id_ = 0;

        triangles_.push_back(tri);
    }

    SetNeedsUpdate(true);
}

void ColliderInfoMesh::SetTriangles(const std::vector<Triangle>& triangles)
{
    triangles_ = triangles;
    SetNeedsUpdate(true);
}

void ColliderInfoMesh::BuildBVH()
{
    /*
    if (triangles_.empty()) return;

    bvh_nodes_.clear();
    use_bvh_ = true;

    // 簡易的なBVH構築（中央分割法）
    struct BuildNode {
        std::vector<int> triangle_indices;
        AABB bounds;
    };

    std::queue<std::pair<BuildNode, int>> build_queue;

    // ルートノード
    BuildNode root;
    root.bounds = aabb_;
    for (int i = 0; i < static_cast<int>(triangles_.size()); i++) {
        root.triangle_indices.push_back(i);
    }

    BVHNode root_node;
    root_node.bounds_ = root.bounds;
    root_node.left_child_ = -1;
    root_node.right_child_ = -1;
    root_node.triangle_index_ = -1;
    bvh_nodes_.push_back(root_node);

    build_queue.push({ root, 0 });

    // BVH構築
    while (!build_queue.empty()) {
        auto&& [current, node_index] = build_queue.front();
        build_queue.pop();

        // 葉ノードの条件
        if (current.triangle_indices.size() <= 4) {
            // 葉ノードとして設定
            if (!current.triangle_indices.empty()) {
                bvh_nodes_[node_index].triangle_index_ = current.triangle_indices[0];
            }
            continue;
        }

        // 最も長い軸で分割
        VECTOR size = current.bounds.GetSize();
        int split_axis = 0;
        if (size.y > size.x) split_axis = 1;
        if (size.z > size.y && size.z > size.x) split_axis = 2;

        // 中央値で分割
        float split_pos = current.bounds.GetCenter().v[split_axis];

        BuildNode left, right;
        left.bounds = AABB(current.bounds.max_, current.bounds.min_);
        right.bounds = AABB(current.bounds.max_, current.bounds.min_);

        // 三角形を分配
        for (int idx : current.triangle_indices) {
            VECTOR center = VScale(VAdd(VAdd(triangles_[idx].v0_, triangles_[idx].v1_),
                triangles_[idx].v2_), 1.0f / 3.0f);

            if (center.v[split_axis] < split_pos) {
                left.triangle_indices.push_back(idx);
                left.bounds.Merge(triangles_[idx].aabb_);
            }
            else {
                right.triangle_indices.push_back(idx);
                right.bounds.Merge(triangles_[idx].aabb_);
            }
        }

        // 子ノードを作成
        if (!left.triangle_indices.empty() && !right.triangle_indices.empty()) {
            BVHNode left_node, right_node;
            left_node.bounds_ = left.bounds;
            left_node.left_child_ = -1;
            left_node.right_child_ = -1;
            left_node.triangle_index_ = -1;

            right_node.bounds_ = right.bounds;
            right_node.left_child_ = -1;
            right_node.right_child_ = -1;
            right_node.triangle_index_ = -1;

            int left_index = static_cast<int>(bvh_nodes_.size());
            bvh_nodes_.push_back(left_node);
            int right_index = static_cast<int>(bvh_nodes_.size());
            bvh_nodes_.push_back(right_node);

            bvh_nodes_[node_index].left_child_ = left_index;
            bvh_nodes_[node_index].right_child_ = right_index;

            build_queue.push({ left, left_index });
            build_queue.push({ right, right_index });
        }
    }
    */
}

bool ColliderInfoMesh::Raycast(const VECTOR& origin, const VECTOR& direction, float max_distance,
    VECTOR& hit_point, VECTOR& hit_normal, int& hit_triangle_index) const
{
    bool hit = false;
    float min_distance = max_distance;

    // 各三角形との交差判定
    for (int i = 0; i < static_cast<int>(triangles_.size()); i++) {
        const Triangle& tri = triangles_[i];

        // ワールド座標に変換
        VECTOR v0 = VAdd(world_position_, VTransform(tri.v0_, world_rotation_));
        VECTOR v1 = VAdd(world_position_, VTransform(tri.v1_, world_rotation_));
        VECTOR v2 = VAdd(world_position_, VTransform(tri.v2_, world_rotation_));

        // レイと三角形の交差判定（Möller-Trumboreアルゴリズム）
        VECTOR edge1 = VSub(v1, v0);
        VECTOR edge2 = VSub(v2, v0);
        VECTOR h = VCross(direction, edge2);
        float a = VDot(edge1, h);

        if (fabsf(a) < 0.00001f) continue;

        float f = 1.0f / a;
        VECTOR s = VSub(origin, v0);
        float u = f * VDot(s, h);

        if (u < 0.0f || u > 1.0f) continue;

        VECTOR q = VCross(s, edge1);
        float v = f * VDot(direction, q);

        if (v < 0.0f || u + v > 1.0f) continue;

        float t = f * VDot(edge2, q);

        if (t > 0.00001f && t < min_distance) {
            min_distance = t;
            hit_point = VAdd(origin, VScale(direction, t));
            hit_normal = VTransform(tri.normal_, world_rotation_);
            hit_triangle_index = i;
            hit = true;
        }
    }

    return hit;
}

std::vector<int> ColliderInfoMesh::GetTrianglesInSphere(const VECTOR& center, float radius) const
{
    std::vector<int> result;
    AABB sphere_aabb = AABB::FromSphere(center, radius);

    for (int i = 0; i < static_cast<int>(triangles_.size()); i++) {
        // ワールド座標でのAABB
        AABB world_tri_aabb = triangles_[i].aabb_;
        world_tri_aabb.Translate(world_position_);

        if (sphere_aabb.Intersects(world_tri_aabb)) {
            // より詳細な判定が必要な場合はここで実装
            result.push_back(i);
        }
    }

    return result;
}

void ColliderInfoMesh::GetStatistics(int& triangle_count, float& surface_area, VECTOR& centroid) const
{
    triangle_count = static_cast<int>(triangles_.size());
    surface_area = 0.0f;
    centroid = VGet(0, 0, 0);

    for (const auto& tri : triangles_) {
        // 三角形の面積
        VECTOR edge1 = VSub(tri.v1_, tri.v0_);
        VECTOR edge2 = VSub(tri.v2_, tri.v0_);
        float area = VSize(VCross(edge1, edge2)) * 0.5f;
        surface_area += area;

        // 重心
        VECTOR tri_center = VScale(VAdd(VAdd(tri.v0_, tri.v1_), tri.v2_), 1.0f / 3.0f);
        centroid = VAdd(centroid, VScale(tri_center, area));
    }

    if (surface_area > 0.0001f) {
        centroid = VScale(centroid, 1.0f / surface_area);
    }
}

bool ColliderInfoMesh::TestCollisionWithBVH(const ColliderInfo* other, const VECTOR& pos_a,
    const VECTOR& pos_b, VECTOR& hit_point,
    VECTOR& hit_normal, float& penetration) const
{
    // BVHを使った高速化判定
    // 実装は複雑なので、ここでは通常の判定にフォールバック
    return CollisionDispatcher::GetInstance().TestCollision(
        this, other, pos_a, pos_b, hit_point, hit_normal, penetration
    );
}

bool ColliderInfoMesh::TestBVHNode(int node_index, const AABB& test_aabb,
    std::vector<int>& potential_triangles) const
{
    if (node_index < 0 || node_index >= static_cast<int>(bvh_nodes_.size())) {
        return false;
    }

    const BVHNode& node = bvh_nodes_[node_index];

    if (!test_aabb.Intersects(node.bounds_)) {
        return false;
    }

    // 葉ノード
    if (node.left_child_ == -1 && node.right_child_ == -1) {
        if (node.triangle_index_ >= 0) {
            potential_triangles.push_back(node.triangle_index_);
        }
        return true;
    }

    // 子ノードを探索
    bool hit = false;
    if (node.left_child_ >= 0) {
        hit |= TestBVHNode(node.left_child_, test_aabb, potential_triangles);
    }
    if (node.right_child_ >= 0) {
        hit |= TestBVHNode(node.right_child_, test_aabb, potential_triangles);
    }

    return hit;
}

VECTOR ColliderInfoMesh::CalculateTriangleNormal(const VECTOR& v0, const VECTOR& v1, const VECTOR& v2)
{
    VECTOR edge1 = VSub(v1, v0);
    VECTOR edge2 = VSub(v2, v0);
    VECTOR normal = VCross(edge1, edge2);
    float len = VSize(normal);

    if (len > 0.00001f) {
        return VScale(normal, 1.0f / len);
    }

    return VGet(0, 1, 0);  // デフォルト法線
}

AABB ColliderInfoMesh::CalculateTriangleAABB(const VECTOR& v0, const VECTOR& v1, const VECTOR& v2)
{
    AABB result(v0, v0);
    result.Merge(AABB(v1, v1));
    result.Merge(AABB(v2, v2));
    return result;
}
