#pragma once
#include "game_object.hpp"
#include "misc/collider/collider_data.hpp"
#include "component/rigidbody.hpp"

/// @brief 物理オブジェクト基底クラス
class CollidableObject abstract : public GameObject
{
public:
    /// @brief 位置補正順位
    enum class Weight : int	{
        Bouncy,     // Ball
        Low,		// Trash Can 
        Normal,		// Player, Enemy
        High,		// Car
        Static		// Wall, Map
    };

public:
    /// @remark 禁止コンストラクタ
    CollidableObject() = delete;

    /// @brief インスタンスを初期化
    CollidableObject(const std::string& name, Weight weight, ColliderInfo::Type collider_type, bool is_trigger);

    /// @brief インスタンスを解放
    virtual ~CollidableObject();

    /// @brief オブジェクトを初期化
    virtual void Initialize()   noexcept;

    /// @brief オブジェクトを更新
    virtual void Update()	    noexcept abstract;

    /// @brief オブジェクトを描画
    virtual void Draw()   const noexcept abstract;

    /// @brief オブジェクトの最終更新
    /// HACK: 毎フレーム呼ばない設計にする
    virtual void LastUpdate()   noexcept abstract;

    /// @brief 衝突判定
    virtual void OnCollide(const CollidableObject& object) noexcept abstract;

    /// @brief 質量を取得
    Weight GetWeight() const noexcept { return weight_; }

    /// @brief 当たり判定を無視（スルー）するタグの追加
    void AddThroughTag	 (std::string tag)					   noexcept;
 
    /// @brief 当たり判定を無視（スルー）するタグの削除
    void RemoveThroughTag(std::string tag)					   noexcept;

    /// @brief 当たり判定を無視（スルー）する対象かどうか
    bool IsThroughTarget(const CollidableObject* target) const noexcept;

    /// @brief Rigidbody を取得
    Rigidbody GetRigidbody()    const noexcept { return     rigidbody_; }

    /// @brief 当たり判定データを取得
    std::shared_ptr<ColliderInfo>
              GetColliderInfo() const noexcept { return collider_info_; }

private:
    /// @brief 当たり判定データを作成
    std::shared_ptr<ColliderInfo> CreateColliderInfo(ColliderInfo::Type collider_type, bool is_trigger);

protected:
    Weight                        weight_;          /// @brief 物質の質量
    Rigidbody                     rigidbody_;	    /// @brief 物理データ
    float                         speed_;           /// @brief スピード

private:
    std::shared_ptr<ColliderInfo> collider_info_;	/// @brief 当たり判定データ
    std::vector<std::string>      through_tags_;    /// @brief 当たり判定を無視するタグリスト

//    friend Physics;
//
//private: /// @remark Physicsのみが扱う型や変数
//    VECTOR future_pos_;         /// @brief 移動先のポジション

};
