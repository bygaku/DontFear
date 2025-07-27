#pragma once
#include <memory>

class GameObject;

/// @brief �R���|�[�l���g���N���X
/// @brief �^�O���p����Őݒ�
class Component abstract
{
public:
	/// @remark �֎~�R���X�g���N�^
	Component() = delete;

	/// @brief �C���X�^���X��������
	/// @param object �R���|�[�l���g���L��
	Component(GameObject& object)
		: owner_(object)
		, is_active_(true)
	{
	}

	/// @brief �C���X�^���X�����
	virtual ~Component() = default;

	/// @brief �R���|�[�l���g������
	virtual void Initialize() noexcept	abstract;

	/// @brief �R���|�[�l���g�L����
	void Activate()   noexcept { is_active_ =  true; }

	/// @brief �R���|�[�l���g�̏������~
	void Deactivate() noexcept { is_active_ = false; }

protected:
	/// @brief �����҂̃A�N�Z�X�����擾
	GameObject* GetOwner() const noexcept { return &owner_; }

private:
	GameObject& owner_;			/// @brief ���L��
	bool		is_active_;		/// @brief �I�u�W�F�N�g�̍X�V���s��������

};