#include <string>
#include "misc/assert_dx.hpp"
#include "misc/collider/collision_dispatcher.hpp"
#include "misc/collider/collision_functions.hpp"

CollisionDispatcher::CollisionDispatcher()
{
    using Type = ColliderInfo::Type;

    //! �S�Ă̑g�ݍ��킹��o�^
    //! Sphere vs Sphere
    RegisterCollisionFunc(Type::Sphere, Type::Sphere,
        collision_functions::TestSphereSphere);

    //! Capsule vs Capsule
    RegisterCollisionFunc(Type::Capsule, Type::Capsule,
        collision_functions::TestCapsuleCapsule);

    //! Capsule vs Sphere
    RegisterCollisionFunc(Type::Capsule, Type::Sphere,
        collision_functions::TestCapsuleSphere);

    //! Capsule vs Box
    RegisterCollisionFunc(Type::Capsule, Type::Box,
        collision_functions::TestCapsuleBox);

    //! Box vs Box
    RegisterCollisionFunc(Type::Box, Type::Box,
        collision_functions::TestBoxBox);

    //! Sphere vs Box
    RegisterCollisionFunc(Type::Sphere, Type::Box,
        collision_functions::TestSphereBox);

    /*
    //! Any vs Terrain (�S�^�C�v�ƒn�`�̑g�ݍ��킹)
    RegisterCollisionFunc(Type::Sphere, Type::Terrain,
        collision_functions::TestAnyTerrain);

    RegisterCollisionFunc(Type::Capsule, Type::Terrain,
        collision_functions::TestAnyTerrain);

    RegisterCollisionFunc(Type::Box, Type::Terrain,
        collision_functions::TestAnyTerrain);

    //! Any vs Mesh
    RegisterCollisionFunc(Type::Sphere, Type::Mesh,
        collision_functions::TestAnyMesh);

    RegisterCollisionFunc(Type::Capsule, Type::Mesh,
        collision_functions::TestAnyMesh);

    RegisterCollisionFunc(Type::Box, Type::Mesh,
        collision_functions::TestAnyMesh);
    */
}

bool CollisionDispatcher::TestCollision(const ColliderInfo* a, const ColliderInfo* b,
                                        const VECTOR&   pos_a, const VECTOR&   pos_b,
                                        VECTOR&     hit_point, VECTOR&    hit_normal,
                                        float& penetration)
{
    //! �֐�������
    uint64_t key = MakeKey(a->GetType(), b->GetType());
    auto it      = collision_funcs_.find(key);

    if (it != collision_funcs_.end()) {
        //! �o�^���ꂽ�֐����Ăяo��
        return it->second(a, b, pos_a, pos_b, hit_point, hit_normal, penetration);
    }

    //! ���o�^�̑g�ݍ��킹
#ifdef _DEBUG
    std::string warning = "Warning: Unregistered collision type pair:";
    ASSERT(it == collision_funcs_.end(), warning);
#endif

    return false;
}

void CollisionDispatcher::RegisterCollisionFunc(ColliderInfo::Type type_a, ColliderInfo::Type type_b, COLLISION_FUNC func)
{
    uint64_t key          = MakeKey(type_a, type_b);
    collision_funcs_[key] = func;
}