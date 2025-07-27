#pragma once
#include <DxLib.h>
#include <unordered_map>
#include <functional>
#include "collider_data.hpp"

/// @brief �Փ˔���𓝈�I�ɏ�������f�B�X�p�b�`���[
class CollisionDispatcher
{
public:
    /// @brief �Փ˔���֐��̌^
    using COLLISION_FUNC = std::function<bool(
        const ColliderInfo* a, const ColliderInfo* b,
        const VECTOR&   pos_a, const VECTOR&   pos_b,
        VECTOR&     hit_point, VECTOR&    hit_normal,
        float& penetration
        )>;

    /// @brief �C���X�^���X���擾
    static CollisionDispatcher& GetInstance()
    {
        static CollisionDispatcher instance;
        return instance;
    }

    /// @brief �Փ˔�������s
    bool TestCollision(const ColliderInfo* a, const ColliderInfo* b,
                       const VECTOR&   pos_a, const VECTOR&   pos_b,
                       VECTOR&     hit_point, VECTOR&    hit_normal,
                       float& penetration);

    /// @brief �Փ˔���֐���o�^
    /// @param type_a �Փ˔���̎��
    /// @param type_b �Փ˔���̎��
    /// @param func   �Փ˔���֐��̌^�Fbool
    void RegisterCollisionFunc(ColliderInfo::Type type_a, ColliderInfo::Type type_b, COLLISION_FUNC func);

private:
    CollisionDispatcher();
    ~CollisionDispatcher() = default;

    /// @brief �^�C�v�y�A����L�[�𐶐�
    uint64_t MakeKey(ColliderInfo::Type a, ColliderInfo::Type b) const
    {
        //! �����𐳋K���i�����������Ɂj
        if (static_cast<int>(a) > static_cast<int>(b)) {
            std::swap(a, b);
        }

        return (static_cast<uint64_t>(a) << 32) | static_cast<uint64_t>(b);
    }

    /// @brief �Փ˔���֐��̃}�b�v
    std::unordered_map<uint64_t, COLLISION_FUNC> collision_funcs_;

};