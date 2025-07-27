#pragma once
#include "collidable_object.hpp"
#include "component/modeler.hpp"

/// @brief 静止オブジェクト
class StaticObject abstract : public CollidableObject
{
public:
    /// @remark 禁止コンストラクタ
    StaticObject() = delete;

    /// @brief インスタンスを初期化
    StaticObject(const std::string& name)
        : CollidableObject{ name, Weight::Static, ColliderInfo::Type::Mesh, false }
        , model_(nullptr)
    {
    }

    /// @brief インスタンスを解放
    virtual ~StaticObject() = default;

    /// @brief オブジェクトを初期化
    virtual void Initialize() noexcept abstract;

    /// @brief オブジェクトを更新
    virtual void Update()	  noexcept abstract;

    /// @brief オブジェクトを描画
    virtual void Draw() const noexcept abstract;

    /// @brief オブジェクトの最終更新
    /// HACK: 毎フレーム呼ばない設計にする
    virtual void LastUpdate() noexcept abstract;

    /// @brief 衝突判定
    virtual void OnCollide(const CollidableObject& object) noexcept abstract;

protected: // TODO: Subclass Sandbox

protected: // TODO: Add Component
    std::shared_ptr<Modeler>  model_;

};
