#pragma once
#include <vector>
#include <memory>
#include <string>
#include "data/object_data.hpp"
#include "component/transform.hpp"

/// @brief �I�u�W�F�N�g�C���^�[�t�F�C�X�N���X
/// @brief �`��, �X�V���s���I�u�W�F�N�g�͂��ׂĂ������p��
/// @brief �^�O���p����Őݒ�
class GameObject abstract
{
public:
    /// @remark �֎~�R���X�g���N�^
    GameObject() = delete;

    /// @brief �C���X�^���X��������
    GameObject(const std::string& name)
        : transform_(std::make_shared<Transform>(*this))
        , name_(name)
        , tag_("none")
        , is_active_(false)
    {
    };

    /// @brief �C���X�^���X�����
    virtual ~GameObject() = default;

    /// @brief �I�u�W�F�N�g��������
    virtual void Initialize() noexcept abstract;

    /// @brief �I�u�W�F�N�g���X�V
    virtual void Update()	  noexcept abstract;

    /// @brief �I�u�W�F�N�g��`��
    virtual void Draw() const noexcept abstract;

    /// @brief �I�u�W�F�N�g�̍ŏI�X�V
    /// HACK: ���t���[���Ă΂Ȃ��݌v�ɂ���
    virtual void LastUpdate() noexcept abstract;

    /// @brief �I�u�W�F�N�g��L����
    void Activate()   noexcept  { is_active_ =  true; }

    /// @brief �I�u�W�F�N�g���폜�����ɏ������~
    void Deactivate() noexcept  { is_active_ = false; }

    /// @brief ���O���擾
    const std::string& GetName() const noexcept { return	  name_; }
                             
    /// @brief �I�u�W�F�N�g�^�O���擾
    /// @return �ݒ肵�ĂȂ��I�u�W�F�N�g�� "none"
    const std::string& GetTag()  const noexcept { return      tag_;  }

    /// @brief �X�V����������擾
    bool IsActive()              const noexcept { return is_active_; }

    /// @brief TransformComponent���擾
    const std::shared_ptr<Transform>& 
         GetTransform()          const noexcept { return transform_; }

protected:
    std::shared_ptr<Transform> transform_;      /// @brief Transform�R���|�[�l���g
    std::string                tag_;            /// @brief �I�u�W�F�N�g�̎��

private:
    bool	                   is_active_;      /// @brief �I�u�W�F�N�g�̍X�V���s��������
    std::string                name_;           /// @brief �I�u�W�F�N�g�̖��O

};