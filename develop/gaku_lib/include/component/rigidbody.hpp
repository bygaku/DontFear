#pragma once

/// @brief �����v�Z�f�[�^
class Rigidbody final
{
public:
    /// @brief �C���X�^���X��������
    Rigidbody();

    /// @brief �C���X�^���X�����
    ~Rigidbody();

    /// @brief �I�u�W�F�N�g��������
    void Initialize() noexcept;

    /// @brief ���W���擾
    const VECTOR& GetPosition()  const noexcept { return position_;    }

    /// @brief �������擾
    const VECTOR& GetDirection() const noexcept { return direction_;   }

    /// @brief �ړ��ʂ��擾
    const VECTOR& GetVelocity()  const noexcept { return velocity_;    }
    
    /// @brief �d�͂�������I�u�W�F�N�g���ʂ��擾
    bool          OnGravity()    const noexcept { return use_gravity_; }

    /// @brief ���W��ݒ�
    void SetPosition  (const VECTOR& position)        noexcept;

    /// @brief �ړ��ʂ�ݒ�
    void SetVelocity  (const VECTOR& velocity)        noexcept;

    /// @brief ���̃I�u�W�F�N�g�ɏd�͂�������
    void SetUseGravity(bool gravity)		          noexcept;

    /// @brief �Փ˂̗͂�������
    void AddForce     (const VECTOR& force_direction) noexcept;

    /// @brief ���t���[���̍��W��ۑ�
    void SaveFuturePosition(const VECTOR& future_pos) noexcept;

    /// @brief ���t���[���̍��W���擾
    const VECTOR& GetFuturePosition() const noexcept { return future_pos_; }
private:
    VECTOR	   position_;			/// @brief ���W
    VECTOR	   direction_;			/// @brief ����
    VECTOR	   velocity_;			/// @brief ���x�x�N�g��
    VECTOR     future_pos_;         /// @brief ���t���[���̍��W
    bool	   use_gravity_;		/// @brief �d�͂�������I�u�W�F�N�g

};
