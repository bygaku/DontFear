#include <DxLib.h>
#include <unordered_set>
#include "misc/collider/octree.hpp"

// ========== OctreeNode �̎��� ==========

void SpatialOctree::OctreeNode::Subdivide()
{
    if (!is_leaf_ || depth_ >= kMaxDepth) return;

    VECTOR center       = bounds_.GetCenter();
    VECTOR half_size    = bounds_.GetHalfSize();
    VECTOR quarter_size = VScale(half_size, 0.5f);

    // 8�̎q�m�[�h���쐬
    for (int i = 0; i < 8; i++) {
        VECTOR child_center = center;
        child_center.x += (i & 1) ? quarter_size.x : -quarter_size.x;
        child_center.y += (i & 2) ? quarter_size.y : -quarter_size.y;
        child_center.z += (i & 4) ? quarter_size.z : -quarter_size.z;

        AABB child_bounds(child_center, quarter_size);
        children_[i] = std::make_unique<OctreeNode>(child_bounds, depth_ + 1);
    }

    is_leaf_ = false;

    // �����̃I�u�W�F�N�g���q�m�[�h�ɍĔz�u
    std::vector<CollidableObject*> current_objects = std::move(objects_);
    objects_.clear();

    for (auto* obj : current_objects) {
        Insert(obj, obj->GetColliderInfo()->GetAABB());
    }
}

void SpatialOctree::OctreeNode::Insert(CollidableObject* object, const AABB& object_aabb)
{
    // ���̃m�[�h�̋��E�ƌ������Ȃ��ꍇ�͑}�����Ȃ�
    if (!bounds_.Intersects(object_aabb)) return;

    // �t�m�[�h�̏ꍇ
    if (is_leaf_) {
        objects_.push_back(object);

        // �I�u�W�F�N�g����臒l�𒴂����番��
        if (objects_.size() > kMaxObjectsPerNode && depth_ < kMaxDepth) {
            Subdivide();
        }
    }
    else {
        // �q�m�[�h�ɑ}��
        for (int i = 0; i < 8; i++) {
            if (children_[i]) {
                children_[i]->Insert(object, object_aabb);
            }
        }
    }
}

void SpatialOctree::OctreeNode::GetPotentialPairs(CollidableObject* object,
    const AABB& object_aabb,
    std::vector<CollidableObject*>& potential_colliders)
{
    // ���̃m�[�h�̋��E�ƌ������Ȃ��ꍇ�͉������Ȃ�
    if (!bounds_.Intersects(object_aabb)) return;

    // ���̃m�[�h�̃I�u�W�F�N�g��ǉ�
    for (auto* other : objects_) {
        if (other != object) {
            potential_colliders.push_back(other);
        }
    }

    // �q�m�[�h���T��
    if (!is_leaf_) {
        for (int i = 0; i < 8; i++) {
            if (children_[i]) {
                children_[i]->GetPotentialPairs(object, object_aabb, potential_colliders);
            }
        }
    }
}

void SpatialOctree::OctreeNode::Clear()
{
    objects_.clear();
    for (int i = 0; i < 8; i++) {
        children_[i].reset();
    }
    is_leaf_ = true;
}

int SpatialOctree::OctreeNode::GetChildIndex(const AABB& aabb) const
{
    VECTOR center = bounds_.GetCenter();
    VECTOR obj_center = aabb.GetCenter();

    int index = 0;
    if (obj_center.x > center.x) index |= 1;
    if (obj_center.y > center.y) index |= 2;
    if (obj_center.z > center.z) index |= 4;

    return index;
}

// ========== SpatialOctree �̎��� ==========

SpatialOctree::SpatialOctree(const VECTOR& world_min, const VECTOR& world_max)
    : world_bounds_(world_min, world_max)
{
    root_ = std::make_unique<OctreeNode>(world_bounds_, 0);
}

void SpatialOctree::Build(const std::vector<CollidableObject*>& objects)
{
    // �����̃c���[���N���A
    root_->Clear();

    // ���[���h���E���Čv�Z�i�I�v�V�����j
    if (!objects.empty()) {
        AABB new_bounds = objects[0]->GetColliderInfo()->GetAABB();
        for (size_t i = 1; i < objects.size(); i++) {
            new_bounds.Merge(objects[i]->GetColliderInfo()->GetAABB());
        }

        // �����]�T����������
        new_bounds.Expand(10.0f);
        world_bounds_ = new_bounds;
        root_ = std::make_unique<OctreeNode>(world_bounds_, 0);
    }

    // �I�u�W�F�N�g��}��
    for (auto* obj : objects) {
        root_->Insert(obj, obj->GetColliderInfo()->GetAABB());
    }
}

std::vector<CollidableObject*> SpatialOctree::GetPotentialColliders(CollidableObject* object)
{
    std::vector<CollidableObject*> potential_colliders;
    const AABB& object_aabb = object->GetColliderInfo()->GetAABB();

    root_->GetPotentialPairs(object, object_aabb, potential_colliders);

    // �d�����폜
    std::sort(potential_colliders.begin(), potential_colliders.end());
    potential_colliders.erase(
        std::unique(potential_colliders.begin(), potential_colliders.end()),
        potential_colliders.end()
    );

    return potential_colliders;
}

std::vector<std::pair<CollidableObject*, CollidableObject*>>
SpatialOctree::GetAllPotentialPairs()
{
    std::vector<std::pair<CollidableObject*, CollidableObject*>> pairs;
    std::unordered_set<uint64_t> processed_pairs;

    // �S�I�u�W�F�N�g�����W
    std::vector<CollidableObject*> all_objects;
    std::function<void(OctreeNode*)> collect_objects = [&](OctreeNode* node) {
        if (!node) return;

        all_objects.insert(all_objects.end(),
            node->objects_.begin(),
            node->objects_.end());

        if (!node->is_leaf_) {
            for (int i = 0; i < 8; i++) {
                if (node->children_[i]) {
                    collect_objects(node->children_[i].get());
                }
            }
        }
        };

    collect_objects(root_.get());

    // �d�����폜
    std::sort(all_objects.begin(), all_objects.end());
    all_objects.erase(
        std::unique(all_objects.begin(), all_objects.end()),
        all_objects.end()
    );

    // �e�I�u�W�F�N�g�ɂ��Đ��ݓI�ȏՓˑ����������
    for (auto* obj : all_objects) {
        auto potential = GetPotentialColliders(obj);

        for (auto* other : potential) {
            // �y�A��ID�𐶐��i�����𐳋K���j
            CollidableObject* first = obj < other ? obj : other;
            CollidableObject* second = obj < other ? other : obj;
            uint64_t pair_id = (reinterpret_cast<uint64_t>(first) << 32) |
                reinterpret_cast<uint64_t>(second);

            // �܂��������Ă��Ȃ��y�A�Ȃ�ǉ�
            if (processed_pairs.insert(pair_id).second) {
                // �����ÓI�Ȃ珜�O
                if (!(first->GetWeight() == CollidableObject::Weight::Static &&
                    second->GetWeight() == CollidableObject::Weight::Static)) {
                    pairs.emplace_back(first, second);
                }
            }
        }
    }

    return pairs;
}

void SpatialOctree::DebugDraw() const
{
    DebugDrawNode(root_.get(), 255);
}

void SpatialOctree::DebugDrawNode(const OctreeNode* node, int color_intensity) const
{
    if (!node) return;

    // �m�[�h�̋��E��`��
    const AABB& bounds = node->bounds_;

    // �[�x�ɉ����ĐF��ς���
    int r = node->is_leaf_ ? color_intensity : 0;
    int g = color_intensity - node->depth_ * 30;
    int b = node->depth_ * 40;
    unsigned int color = GetColor(r, g, b);

    // �I�u�W�F�N�g���܂܂�Ă���m�[�h�̂ݕ`��
    if (!node->objects_.empty() || !node->is_leaf_) {
        DrawCube3D(bounds.min_, bounds.max_, color, color, FALSE);
    }

    // �q�m�[�h���`��
    if (!node->is_leaf_) {
        for (int i = 0; i < 8; i++) {
            if (node->children_[i]) {
                DebugDrawNode(node->children_[i].get(), color_intensity - 20);
            }
        }
    }

    // �I�u�W�F�N�g����\���i�t�m�[�h�̂݁j
    if (node->is_leaf_ && !node->objects_.empty()) {
        VECTOR center = bounds.GetCenter();
        VECTOR screen_pos = ConvWorldPosToScreenPos(center);
        if (screen_pos.z > 0 && screen_pos.z < 1) {
            DrawFormatString(static_cast<int>(screen_pos.x),
                static_cast<int>(screen_pos.y),
                GetColor(255, 255, 255),
                "%d", node->objects_.size());
        }
    }
}