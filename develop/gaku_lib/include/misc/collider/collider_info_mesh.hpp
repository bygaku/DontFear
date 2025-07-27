#pragma once
#include <vector>
#include "collider_data.hpp"

/// @brief ���b�V��������
class ColliderInfoMesh : public ColliderInfo 
{
public:
    /// @brief �O�p�`�f�[�^
    struct Triangle {
        VECTOR   v0_, v1_, v2_;     /// @brief ���_
        VECTOR   normal_;           /// @brief �@��
        AABB     aabb_;             /// @brief ���̎O�p�`��AABB
        uint32_t material_id_;      /// @brief �}�e���A��ID
    };

    /// @brief BVH�iBounding Volume Hierarchy�j�m�[�h
    struct BVHNode {
        AABB bounds_;
        int left_child_;    // -1�Ȃ�t�m�[�h
        int right_child_;
        int triangle_index_; // �t�m�[�h�̏ꍇ�̎O�p�`�C���f�b�N�X
    };

public:
    /// @brief �֎~�R���X�g���N�^
    ColliderInfoMesh() = delete;

    /// @brief �R���X�g���N�^
    ColliderInfoMesh(bool is_trigger, uint32_t category = Category::kAll, uint32_t mask = Category::kAll);

    /// @brief �f�X�g���N�^
    ~ColliderInfoMesh() override = default;

    /// @brief AABB���X�V
    void UpdateAABB(const VECTOR& position, const MATRIX& rotation) override;

    /// @brief �ڍׂȏՓ˔���
    bool TestCollision(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const override;

    /// @brief DxLib�̃��f�����烁�b�V���f�[�^���\�z
    void BuildFromModel(int model_handle);

    /// @brief ���_�ƃC���f�b�N�X���烁�b�V���f�[�^���\�z
    void BuildFromVertices(const std::vector<VECTOR>& vertices,
        const std::vector<int>& indices);

    /// @brief �O�p�`�f�[�^�𒼐ڐݒ�
    void SetTriangles(const std::vector<Triangle>& triangles);

    /// @brief �O�p�`���X�g���擾
    const std::vector<Triangle>& GetTriangles() const { return triangles_; }

    /// @brief BVH���\�z�i�������j
    void BuildBVH();

    /// @brief BVH���g�p���邩�ݒ�
    void SetUseBVH(bool use) { use_bvh_ = use; }
    bool GetUseBVH() const { return use_bvh_; }

    /// @brief �œK�����x����ݒ�
    void SetOptimizationLevel(int level) { optimization_level_ = level; }

    /// @brief ���C�L���X�g�i���b�V����p�j
    bool Raycast(const VECTOR& origin, const VECTOR& direction, float max_distance,
        VECTOR& hit_point, VECTOR& hit_normal, int& hit_triangle_index) const;

    /// @brief ���Ƃ̌�������O�p�`���擾
    std::vector<int> GetTrianglesInSphere(const VECTOR& center, float radius) const;

    /// @brief ���b�V���̓��v�����擾
    void GetStatistics(int& triangle_count, float& surface_area, VECTOR& centroid) const;

private:
    /// @brief BVH���g�����Փ˔���
    bool TestCollisionWithBVH(const ColliderInfo* other, const VECTOR& pos_a,
        const VECTOR& pos_b, VECTOR& hit_point,
        VECTOR& hit_normal, float& penetration) const;

    /// @brief BVH�m�[�h�Ƃ̌�������
    bool TestBVHNode(int node_index, const AABB& test_aabb,
        std::vector<int>& potential_triangles) const;

    /// @brief �O�p�`�̖@�����v�Z
    static VECTOR CalculateTriangleNormal(const VECTOR& v0, const VECTOR& v1, const VECTOR& v2);

    /// @brief �O�p�`��AABB���v�Z
    static AABB CalculateTriangleAABB(const VECTOR& v0, const VECTOR& v1, const VECTOR& v2);

private:
    std::vector<Triangle> triangles_;        /// @brief �O�p�`���X�g
    std::vector<BVHNode> bvh_nodes_;        /// @brief BVH�m�[�h
    bool use_bvh_;                          /// @brief BVH���g�p���邩
    int optimization_level_;                 /// @brief �œK�����x���i0-3�j

    // �L���b�V��
    VECTOR world_position_;                  /// @brief ���[���h�ʒu
    MATRIX world_rotation_;                  /// @brief ���[���h��]
};