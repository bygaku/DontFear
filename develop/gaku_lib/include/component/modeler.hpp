#pragma once
#include <string>
#include "component.hpp"
#include "transform.hpp"

/// @brief ���f���Ǘ�
class Modeler final : public Component
{
public:	
    /// @remark �֎~�R���X�g���N�^
    Modeler() = delete;

    /// @brief �C���X�^���X��������
    /// @param owner �R���|�[�l���g���L��
    Modeler(GameObject& owner);

    /// @brief �C���X�^���X�����
    ~Modeler()                 override;

    /// @brief �R���|�[�l���g��������
    void Initialize() noexcept override;

    /// @brief ���f����`��
    void Draw() const noexcept;

    /// @brief ���f���n���h�����擾
    int	 GetModelHandle() const                 noexcept;

    /// @brief ���f���n���h����ݒ�
    /// @param model_path ���f���̃t�@�C���p�X
    void SetModelHandle(std::string file_path)  noexcept;

    /// @brief ���f���n���h����ݒ�
    /// @param model_handle ���łɃ��[�h�ς݂̃n���h��
    void SetModelHandle(int model_handle)       noexcept;

    /// @brief ���f���n���h�������Z�b�g
    void ResetModelHandle()                     noexcept;

    /// @brief �s��
    void SetMatrix(MATRIX mat) const            noexcept;

private:
    int model_handle_;  /// @brief ���f���n���h��

};