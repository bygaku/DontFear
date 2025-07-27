#pragma once
#include <vector>
#include <memory>
#include <string>
#include "data/object_data.hpp"
#include "component/transform.hpp"

/// @brief オブジェクトインターフェイスクラス
/// @brief 描画, 更新を行うオブジェクトはすべてここを継承
/// @brief タグを継承先で設定
class GameObject abstract
{
public:
    /// @remark 禁止コンストラクタ
    GameObject() = delete;

    /// @brief インスタンスを初期化
    GameObject(const std::string& name)
        : transform_(std::make_shared<Transform>(*this))
        , name_(name)
        , tag_("none")
        , is_active_(false)
    {
    };

    /// @brief インスタンスを解放
    virtual ~GameObject() = default;

    /// @brief オブジェクトを初期化
    virtual void Initialize() noexcept abstract;

    /// @brief オブジェクトを更新
    virtual void Update()	  noexcept abstract;

    /// @brief オブジェクトを描画
    virtual void Draw() const noexcept abstract;

    /// @brief オブジェクトの最終更新
    /// HACK: 毎フレーム呼ばない設計にする
    virtual void LastUpdate() noexcept abstract;

    /// @brief オブジェクトを有効化
    void Activate()   noexcept  { is_active_ =  true; }

    /// @brief オブジェクトを削除せずに処理を停止
    void Deactivate() noexcept  { is_active_ = false; }

    /// @brief 名前を取得
    const std::string& GetName() const noexcept { return	  name_; }
                             
    /// @brief オブジェクトタグを取得
    /// @return 設定してないオブジェクトは "none"
    const std::string& GetTag()  const noexcept { return      tag_;  }

    /// @brief 更新中か判定を取得
    bool IsActive()              const noexcept { return is_active_; }

    /// @brief TransformComponentを取得
    const std::shared_ptr<Transform>& 
         GetTransform()          const noexcept { return transform_; }

protected:
    std::shared_ptr<Transform> transform_;      /// @brief Transformコンポーネント
    std::string                tag_;            /// @brief オブジェクトの種類

private:
    bool	                   is_active_;      /// @brief オブジェクトの更新を行うか判定
    std::string                name_;           /// @brief オブジェクトの名前

};