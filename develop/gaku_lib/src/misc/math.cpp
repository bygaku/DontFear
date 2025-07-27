#include <DxLib.h>
#include "misc/math/math.hpp"

namespace dx_math
{
#pragma region �x�N�g��
    VECTOR GetUnitVector(const VECTOR& vec)
    {
        return VSize(vec) != 0 ? VNorm(vec) : vec;
    }
#pragma endregion

#pragma region �@���x�N�g���Z�o
    VECTOR GetNormalVector(const VECTOR& vec1, const VECTOR& vec2)
    {
        // [�@���x�N�g�� = v1��v2�̊O��]���琬�����擾
        return GetUnitVector(VCross(vec1, vec2));
    }

    VECTOR GetNormalVector(const VECTOR& vec)
	{
        VECTOR normal_v{};

        if (vec != VGet(1, 0, 0))
        {
            normal_v = GetNormalVector(vec, VGet(1, 0, 0));
        }
        else if (vec != VGet(0, 1, 0))
        {
            normal_v = GetNormalVector(vec, VGet(0, 1, 0));
        }
        else if (vec != VGet(0, 0, 1))
        {
            normal_v = GetNormalVector(vec, VGet(0, 0, 1));
        }
        return GetUnitVector(normal_v);
    }
#pragma endregion

#pragma region �d�S�Z�o
    VECTOR GetCentroidOfTriangle(const VECTOR& position1, const VECTOR& position2, const VECTOR& position3)
    {
        VECTOR centroid{};
        centroid.x = (position1.x + position2.x + position3.x) / 3.0f;
        centroid.y = (position1.y + position2.y + position3.y) / 3.0f;
        centroid.z = (position1.z + position2.z + position3.z) / 3.0f;
        return centroid;
    }

    VECTOR GetCentroidOfAQuadrilateral(const VECTOR& position1, const VECTOR& position2, const VECTOR& position3, const VECTOR& position4)
    {
        return VECTOR();
    }
#pragma endregion

}