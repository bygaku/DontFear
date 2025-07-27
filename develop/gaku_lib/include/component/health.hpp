#pragma once
#include <cstdint>
#include <functional>
#include <algorithm>
#include "component.hpp"

/// @brief 体力管理
class Health final : public Component
{
public:
	/// @remark 禁止コンストラクタ
	Health() = delete;

	/// @brief インスタンスを初期化
	/// @param owner コンポーネント所有者
	/// @param max_health 初期 最大体力値
	Health(GameObject& owner, uint32_t max_health);

	/// @brief インスタンスを解放
	~Health()				   override;

	/// @brief コンポーネントを初期化
	void Initialize() noexcept override;

#pragma region 体力アクセサ
	/// @remark 命名
	using HEALTH_CHANGED_CALLBACK = std::function<void(uint32_t current_health, uint32_t prev_health)>;
	using DEATH_ACTION_CALLBACK	  = std::function<void()>;

	/// @brief 現在の体力を取得
	[[nodiscard]] const uint32_t& GetCurrentHealth() const { return current_health_; }

	/// @brief 最大体力値を取得
	[[nodiscard]] const uint32_t& GetMaxHealth()	 const { return		max_health_; }

	/// @brief 生きているか判定を取得
	[[nodiscard]] const bool	  IsAlive()			 const { return		  is_alive_; }

#undef min
#undef max
	/// @brief 最大体力値を更新
	/// @attention 非推奨：初期化時の使用
	void UpgradeMaxHealth(uint32_t max_health);
	
	/// @brief 無敵時間を設定
	/// @attention 非推奨：初期化時以外での使用
	void SetInvincibilityTime(const float& time);

	/// @brief 体力変更時の処理を登録
	void RegisterOnHealthChanged(const HEALTH_CHANGED_CALLBACK& health_changed_process);

	/// @brief 死亡時の処理を登録
	void RegisterOnDeath(const DEATH_ACTION_CALLBACK& death_process);
	
	/// @brief ダメージを受ける
	void TakeDamage(uint32_t amount) noexcept;

	/// @brief 回復を受ける
	void TakeHeal  (uint32_t amount) noexcept;

	/// @brief 体力値を初期化
	void Reset()					 noexcept;
#pragma endregion

private:
	bool  is_alive_;							/// @brief 生きているか
	bool  is_invincible_;						/// @brief 無敵中か
	float invincibility_time_;					/// @brief 無敵時間
	float invincibility_timer_;					/// @brief 無敵時間タイマー

	HEALTH_CHANGED_CALLBACK on_health_changed_; /// @brief 体力の変更を通知
	DEATH_ACTION_CALLBACK   on_death_;		    /// @brief 死亡を通知
	uint32_t				current_health_;    /// @brief 現在の体力値
	uint32_t				max_health_;	    /// @brief 最大の体力値

};
