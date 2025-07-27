#pragma once
#include <cstdint>
#include <functional>
#include <algorithm>
#include "component.hpp"

/// @brief �̗͊Ǘ�
class Health final : public Component
{
public:
	/// @remark �֎~�R���X�g���N�^
	Health() = delete;

	/// @brief �C���X�^���X��������
	/// @param owner �R���|�[�l���g���L��
	/// @param max_health ���� �ő�̗͒l
	Health(GameObject& owner, uint32_t max_health);

	/// @brief �C���X�^���X�����
	~Health()				   override;

	/// @brief �R���|�[�l���g��������
	void Initialize() noexcept override;

#pragma region �̗̓A�N�Z�T
	/// @remark ����
	using HEALTH_CHANGED_CALLBACK = std::function<void(uint32_t current_health, uint32_t prev_health)>;
	using DEATH_ACTION_CALLBACK	  = std::function<void()>;

	/// @brief ���݂̗̑͂��擾
	[[nodiscard]] const uint32_t& GetCurrentHealth() const { return current_health_; }

	/// @brief �ő�̗͒l���擾
	[[nodiscard]] const uint32_t& GetMaxHealth()	 const { return		max_health_; }

	/// @brief �����Ă��邩������擾
	[[nodiscard]] const bool	  IsAlive()			 const { return		  is_alive_; }

#undef min
#undef max
	/// @brief �ő�̗͒l���X�V
	/// @attention �񐄏��F���������̎g�p
	void UpgradeMaxHealth(uint32_t max_health);
	
	/// @brief ���G���Ԃ�ݒ�
	/// @attention �񐄏��F���������ȊO�ł̎g�p
	void SetInvincibilityTime(const float& time);

	/// @brief �̗͕ύX���̏�����o�^
	void RegisterOnHealthChanged(const HEALTH_CHANGED_CALLBACK& health_changed_process);

	/// @brief ���S���̏�����o�^
	void RegisterOnDeath(const DEATH_ACTION_CALLBACK& death_process);
	
	/// @brief �_���[�W���󂯂�
	void TakeDamage(uint32_t amount) noexcept;

	/// @brief �񕜂��󂯂�
	void TakeHeal  (uint32_t amount) noexcept;

	/// @brief �̗͒l��������
	void Reset()					 noexcept;
#pragma endregion

private:
	bool  is_alive_;							/// @brief �����Ă��邩
	bool  is_invincible_;						/// @brief ���G����
	float invincibility_time_;					/// @brief ���G����
	float invincibility_timer_;					/// @brief ���G���ԃ^�C�}�[

	HEALTH_CHANGED_CALLBACK on_health_changed_; /// @brief �̗͂̕ύX��ʒm
	DEATH_ACTION_CALLBACK   on_death_;		    /// @brief ���S��ʒm
	uint32_t				current_health_;    /// @brief ���݂̗̑͒l
	uint32_t				max_health_;	    /// @brief �ő�̗̑͒l

};
