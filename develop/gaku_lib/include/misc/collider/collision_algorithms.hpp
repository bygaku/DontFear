#pragma once
#include <DxLib.h>
#include <algorithm>
#include <cmath>

namespace collision_algorithms {

    /// @brief 点と線分の最近接点を取得
    inline VECTOR ClosestPointOnSegment(const VECTOR& point, const VECTOR& seg_start, const VECTOR& seg_end) {
        VECTOR seg = VSub(seg_end, seg_start);
        float t = VDot(VSub(point, seg_start), seg) / VSquareSize(seg);
        t = std::clamp(t, 0.0f, 1.0f);
        return VAdd(seg_start, VScale(seg, t));
    }

    /// @brief 点と三角形の最近接点を取得
    inline VECTOR ClosestPointOnTriangle(const VECTOR& point, const VECTOR& a, const VECTOR& b, const VECTOR& c) {
        // 重心座標系で計算
        VECTOR ab = VSub(b, a);
        VECTOR ac = VSub(c, a);
        VECTOR ap = VSub(point, a);

        float d1 = VDot(ab, ap);
        float d2 = VDot(ac, ap);
        if (d1 <= 0.0f && d2 <= 0.0f) return a;

        VECTOR bp = VSub(point, b);
        float d3 = VDot(ab, bp);
        float d4 = VDot(ac, bp);
        if (d3 >= 0.0f && d4 <= d3) return b;

        float vc = d1 * d4 - d3 * d2;
        if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
            float v = d1 / (d1 - d3);
            return VAdd(a, VScale(ab, v));
        }

        VECTOR cp = VSub(point, c);
        float d5 = VDot(ab, cp);
        float d6 = VDot(ac, cp);
        if (d6 >= 0.0f && d5 <= d6) return c;

        float vb = d5 * d2 - d1 * d6;
        if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
            float w = d2 / (d2 - d6);
            return VAdd(a, VScale(ac, w));
        }

        float va = d3 * d6 - d5 * d4;
        if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
            float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
            return VAdd(b, VScale(VSub(c, b), w));
        }

        float denom = 1.0f / (va + vb + vc);
        float v = vb * denom;
        float w = vc * denom;
        return VAdd(a, VAdd(VScale(ab, v), VScale(ac, w)));
    }

    /// @brief カプセル同士の衝突判定
    inline bool TestCapsuleCapsule(const VECTOR& a_start, const VECTOR& a_end, float a_radius,
        const VECTOR& b_start, const VECTOR& b_end, float b_radius,
        VECTOR& hit_point, VECTOR& hit_normal, float& penetration) {
        // 線分間の最短距離を計算
        VECTOR a_dir = VSub(a_end, a_start);
        VECTOR b_dir = VSub(b_end, b_start);
        VECTOR w = VSub(a_start, b_start);

        float a = VSquareSize(a_dir);
        float b = VDot(a_dir, b_dir);
        float c = VSquareSize(b_dir);
        float d = VDot(a_dir, w);
        float e = VDot(b_dir, w);
        float D = a * c - b * b;

        float sc, tc;

        if (D < 0.00001f) {
            sc = 0.0f;
            tc = (b > c ? d / b : e / c);
        }
        else {
            sc = (b * e - c * d) / D;
            tc = (a * e - b * d) / D;
        }

        sc = std::clamp(sc, 0.0f, 1.0f);
        tc = std::clamp(tc, 0.0f, 1.0f);

        VECTOR closest_a = VAdd(a_start, VScale(a_dir, sc));
        VECTOR closest_b = VAdd(b_start, VScale(b_dir, tc));

        VECTOR diff = VSub(closest_a, closest_b);
        float dist_sq = VSquareSize(diff);
        float radius_sum = a_radius + b_radius;

        if (dist_sq <= radius_sum * radius_sum) {
            float dist = sqrtf(dist_sq);
            if (dist > 0.00001f) {
                hit_normal = VScale(diff, 1.0f / dist);
                penetration = radius_sum - dist;
                hit_point = VAdd(closest_b, VScale(hit_normal, b_radius));
            }
            else {
                hit_normal = VGet(0, 1, 0);
                penetration = radius_sum;
                hit_point = closest_a;
            }
            return true;
        }

        return false;
    }

    /// @brief カプセルと球の衝突判定
    inline bool TestCapsuleSphere(const VECTOR& cap_start, const VECTOR& cap_end, float cap_radius,
        const VECTOR& sphere_center, float sphere_radius,
        VECTOR& hit_point, VECTOR& hit_normal, float& penetration) {
        VECTOR closest = ClosestPointOnSegment(sphere_center, cap_start, cap_end);
        VECTOR diff = VSub(sphere_center, closest);
        float dist_sq = VSquareSize(diff);
        float radius_sum = cap_radius + sphere_radius;

        if (dist_sq <= radius_sum * radius_sum) {
            float dist = sqrtf(dist_sq);
            if (dist > 0.00001f) {
                hit_normal = VScale(diff, 1.0f / dist);
                penetration = radius_sum - dist;
                hit_point = VAdd(closest, VScale(hit_normal, cap_radius));
            }
            else {
                hit_normal = VGet(0, 1, 0);
                penetration = radius_sum;
                hit_point = closest;
            }
            return true;
        }

        return false;
    }

    /// @brief カプセルと三角形の衝突判定
    inline bool TestCapsuleTriangle(const VECTOR& cap_start, const VECTOR& cap_end, float radius,
        const VECTOR& v0, const VECTOR& v1, const VECTOR& v2,
        VECTOR& hit_point, VECTOR& hit_normal, float& penetration) {
        // 三角形の法線
        VECTOR edge1 = VSub(v1, v0);
        VECTOR edge2 = VSub(v2, v0);
        VECTOR tri_normal = VNorm(VCross(edge1, edge2));

        // カプセルの中心線と三角形平面の交差判定
        VECTOR cap_dir = VSub(cap_end, cap_start);
        float cap_len = VSize(cap_dir);
        cap_dir = VScale(cap_dir, 1.0f / cap_len);

        // 各頂点、辺との最短距離をチェック
        float min_dist_sq = FLT_MAX;
        VECTOR closest_point;

        // 頂点との距離
        for (const VECTOR* v : { &v0, &v1, &v2 }) {
            VECTOR closest = ClosestPointOnSegment(*v, cap_start, cap_end);
            float dist_sq = VSquareSize(VSub(*v, closest));
            if (dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
                closest_point = *v;
            }
        }

        // 辺との距離
        const VECTOR* verts[3] = { &v0, &v1, &v2 };
        for (int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            // 線分と線分の最短距離を求める
            VECTOR edge_start = *verts[i];
            VECTOR edge_end = *verts[j];

            // 簡易版：エッジ上の点とカプセル線分の最短距離
            for (float t = 0; t <= 1.0f; t += 0.1f) {
                VECTOR edge_point = VAdd(edge_start, VScale(VSub(edge_end, edge_start), t));
                VECTOR closest = ClosestPointOnSegment(edge_point, cap_start, cap_end);
                float dist_sq = VSquareSize(VSub(edge_point, closest));
                if (dist_sq < min_dist_sq) {
                    min_dist_sq = dist_sq;
                    closest_point = edge_point;
                }
            }
        }

        // 三角形の面との距離
        VECTOR tri_closest = ClosestPointOnTriangle(cap_start, v0, v1, v2);
        VECTOR cap_closest = ClosestPointOnSegment(tri_closest, cap_start, cap_end);
        float face_dist_sq = VSquareSize(VSub(tri_closest, cap_closest));
        if (face_dist_sq < min_dist_sq) {
            min_dist_sq = face_dist_sq;
            closest_point = tri_closest;
        }

        // 衝突判定
        if (min_dist_sq <= radius * radius) {
            float dist = sqrtf(min_dist_sq);
            VECTOR cap_closest_final = ClosestPointOnSegment(closest_point, cap_start, cap_end);
            VECTOR diff = VSub(closest_point, cap_closest_final);

            if (dist > 0.00001f) {
                hit_normal = VScale(diff, 1.0f / dist);
                penetration = radius - dist;
                hit_point = closest_point;
            }
            else {
                hit_normal = tri_normal;
                penetration = radius;
                hit_point = closest_point;
            }
            return true;
        }

        return false;
    }

    /// @brief 球と球の衝突判定
    inline bool TestSphereSphere(const VECTOR& center_a, float radius_a,
        const VECTOR& center_b, float radius_b,
        VECTOR& hit_point, VECTOR& hit_normal, float& penetration) {
        VECTOR diff = VSub(center_b, center_a);
        float dist_sq = VSquareSize(diff);
        float radius_sum = radius_a + radius_b;

        if (dist_sq <= radius_sum * radius_sum) {
            float dist = sqrtf(dist_sq);
            if (dist > 0.00001f) {
                hit_normal = VScale(diff, 1.0f / dist);
                penetration = radius_sum - dist;
                hit_point = VAdd(center_a, VScale(hit_normal, radius_a));
            }
            else {
                hit_normal = VGet(0, 1, 0);
                penetration = radius_sum;
                hit_point = center_a;
            }
            return true;
        }

        return false;
    }

} // namespace collision_algorithms