#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <functional>
#include "object/collidable_object.hpp"
#include "misc/primitive/aabb.hpp"

/// @brief �����؂ɂ���ԕ����V�X�e��
class SpatialOctree {
public:
    static constexpr int kMaxDepth = 6;          /// @brief �ő�[�x
    static constexpr int kMaxObjectsPerNode = 8; /// @brief �m�[�h������̍ő�I�u�W�F�N�g��

private:
    struct OctreeNode {
        AABB bounds_;                                    /// @brief ���̃m�[�h�̋��E
        std::vector<CollidableObject*> objects_;         /// @brief ���̃m�[�h�ɑ�����I�u�W�F�N�g
        std::unique_ptr<OctreeNode>    children_[8];     /// @brief �q�m�[�h
        int depth_;                                      /// @brief ���݂̐[�x
        bool is_leaf_;                                   /// @brief ���[�m�[�h���ǂ���

        OctreeNode(const AABB& bounds, int depth)
            : bounds_(bounds), depth_(depth), is_leaf_(true) {
        }

        /// @brief �q�m�[�h���쐬
        void Subdivide();

        /// @brief �I�u�W�F�N�g��}��
        void Insert(CollidableObject* object, const AABB& object_aabb);

        /// @brief ���ݓI�ȏՓ˃y�A���擾
        void GetPotentialPairs(CollidableObject* object, const AABB& object_aabb,
            std::vector<CollidableObject*>& potential_colliders);

        /// @brief �m�[�h���N���A
        void Clear();

        /// @brief �q�m�[�h�̃C���f�b�N�X���擾
        int GetChildIndex(const AABB& aabb) const;
    };

public:
    /// @brief �R���X�g���N�^
    SpatialOctree(const VECTOR& world_min, const VECTOR& world_max);

    /// @brief �f�X�g���N�^
    ~SpatialOctree() = default;

    /// @brief �����؂��č\�z
    void Build(const std::vector<CollidableObject*>& objects);

    /// @brief �I�u�W�F�N�g�̐��ݓI�ȏՓˑ�����擾
    std::vector<CollidableObject*> GetPotentialColliders(CollidableObject* object);

    /// @brief �S�Ă̐��ݓI�ȏՓ˃y�A���擾
    std::vector<std::pair<CollidableObject*, CollidableObject*>> GetAllPotentialPairs();

    /// @brief �f�o�b�O�`��
    void DebugDraw() const;

private:
    std::unique_ptr<OctreeNode> root_;   /// @brief ���[�g�m�[�h
    AABB world_bounds_;                  /// @brief ���[���h���E

    /// @brief �m�[�h�̃f�o�b�O�`��i�ċA�j
    void DebugDrawNode(const OctreeNode* node, int color_intensity) const;

};