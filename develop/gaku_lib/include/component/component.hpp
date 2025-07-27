#pragma once
#include <memory>

class GameObject;

/// @brief コンポーネント基底クラス
/// @brief タグを継承先で設定
class Component abstract
{
public:
	/// @remark 禁止コンストラクタ
	Component() = delete;

	/// @brief インスタンスを初期化
	/// @param object コンポーネント所有者
	Component(GameObject& object)
		: owner_(object)
		, is_active_(true)
	{
	}

	/// @brief インスタンスを解放
	virtual ~Component() = default;

	/// @brief コンポーネント初期化
	virtual void Initialize() noexcept	abstract;

	/// @brief コンポーネント有効化
	void Activate()   noexcept { is_active_ =  true; }

	/// @brief コンポーネントの処理を停止
	void Deactivate() noexcept { is_active_ = false; }

protected:
	/// @brief 所持者のアクセス権を取得
	GameObject* GetOwner() const noexcept { return &owner_; }

private:
	GameObject& owner_;			/// @brief 所有者
	bool		is_active_;		/// @brief オブジェクトの更新を行うか判定

};