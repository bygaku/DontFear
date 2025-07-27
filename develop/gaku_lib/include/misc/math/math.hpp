#pragma once
#include <algorithm>
#include <optional>
#include <array>
#include <unordered_map>

#include "misc/math/quaternion.hpp"
#include "misc/math/vector2d.hpp"

namespace dx_math
{
	static constexpr float kDegreesToRadian = DX_PI_F /  180.0f;	/// @brief ディグリーをラジアンに変換(変換対象と掛け算を行う)
	static constexpr float kRadianToDegrees = 180.0f  / DX_PI_F;	/// @brief ラジアンをディグリーに変換(変換対象と掛け算を行う)

#pragma region ベクトル
	/// @brief 単位ベクトル変換
	/// @return 0ベクトルでなければ、正規化したベクトルを返す
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vec);
#pragma endregion

#pragma region 法線ベクトル算出
	/// @brief 基準とするベクトルから法線ベクトルを求め、その内の一つを取得
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& vec1, const VECTOR& vec2);

	/// @brief 基準とするベクトルから法線ベクトルを求め、その内の一つを取得
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& vec);
#pragma endregion

#pragma region 重心算出
	/// @brief 三角形の幾何学的重心を取得
	[[nodiscard]] VECTOR GetCentroidOfTriangle(const VECTOR& position1, const VECTOR& position2, const VECTOR& position3);

	/// @brief 四角形の物理的重心を取得
	/// TODO: 不具合発生中の可能性あり
	[[nodiscard]] VECTOR GetCentroidOfAQuadrilateral(const VECTOR& position1, const VECTOR& position2, const VECTOR& position3, const VECTOR& position4);
#pragma endregion

#pragma region Operator for DxLib
	// VECTOR + VECTOR
	inline VECTOR operator+ (const VECTOR& v1, const VECTOR& v2) { return VAdd(v1, v2); }

	// VECTOR - VECTOR
	inline VECTOR operator- (const VECTOR& v1, const VECTOR& v2) { return VSub(v1, v2);	}

	// VECTOR * VECTOR (外積)
	inline VECTOR operator* (const VECTOR& v1, const VECTOR& v2) { return VCross(v1, v2); }

	// VECTOR * float
	inline VECTOR operator* (const VECTOR& v, const float& f)    { return VGet(v.x * f, v.y * f, v.z * f);	}
															     
	// float * VECTOR										     
	inline VECTOR operator* (const float& f, const VECTOR& v)    { return VGet(v.x * f, v.y * f, v.z * f); }

	// VECTOR * MATRIX
	inline VECTOR operator* (VECTOR& v, const MATRIX& m)		 { return VTransform(v, m); }

	// VECTOR += VECTOR
	inline VECTOR& operator+= (VECTOR& v1, const VECTOR& v2) {
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		return v1;
	}

	// VECTOR -= VECTOR
	inline VECTOR& operator-= (VECTOR& v1, const VECTOR& v2) {
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		return v1;
	}

	// VECTOR *= float
	inline VECTOR& operator*= (VECTOR& v, const float& f) {
		v.x *= f;
		v.y *= f;
		v.z *= f;
		return v;
	}

	// VECTOR == VECTOR
	inline bool operator== (const VECTOR& v1, const VECTOR& v2) { 
		return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
	}

	// VECTOR != VECTOR
	inline bool operator!= (const VECTOR& v1, const VECTOR& v2)  { return !(v1 == v2); }

	// MATRIX + MATRIX
	inline MATRIX operator+ (const MATRIX& m1, const MATRIX& m2) { return MAdd(m1, m2); }

	// MATRIX * MATRIX
	inline MATRIX operator* (const MATRIX& m1, const MATRIX& m2) { return MMult(m1, m2); }

	// MATIX * float
	inline MATRIX operator* (const MATRIX& m, const float& f)	 { return MScale(m, f); }

	// float * MATRIX
	inline MATRIX operator* (const float& f, const MATRIX& m)	 { return MScale(m, f); }
#pragma endregion

}