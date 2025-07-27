#pragma once
#include <memory>
#include <string>
#include "component.hpp"
#include "misc/math/quaternion.hpp"

using namespace dx_math;

/// @brief ���W�E��]�s��Ǘ�
class Transform final : public Component
{
public:
    /// @remark �֎~�R���X�g���N�^
    Transform() = delete;
    
    /// @brief �C���X�^���X��������
    /// @param object �R���|�[�l���g���L��
    Transform(GameObject& object);

    /// @brief �C���X�^���X�����
    ~Transform()               override;

    /// @brief �R���|�[�l���g��������
    void Initialize() noexcept override;

    #pragma region ���[�J�����W�n�̑���
    /// @brief �I�u�W�F�N�g�̍��W���擾
    const VECTOR&     GetLocalPosition() const;

    /// @brief �I�u�W�F�N�g�̎p�����擾
    const Quaternion& GetLocalRotation() const;

    /// @brief �I�u�W�F�N�g�̃X�P�[���擾
    const VECTOR&     GetLocalScale()    const;

    /// @remark �I�u�W�F�N�g�̍��W��ݒ� 
    void              SetLocalPosition     (const VECTOR& pos);
    void              SetLocalPosition     (float x, float y, float z);

    /// @brief �I�u�W�F�N�g�̎p����ݒ�
    void              SetLocalRotation     (const Quaternion& rot);

    /// @remark �I�u�W�F�N�g�̎p�����I�C���[�p�Őݒ�
    void              SetLocalRotationEuler(const VECTOR& euler);
    void              SetLocalRotationEuler(float x, float y, float z);

    /// @remark �I�u�W�F�N�g�̃X�P�[���ݒ�
    void              SetLocalScale        (const VECTOR& scale);
    void              SetLocalScale        (float x, float y, float z);
    void              SetLocalScale        (float uni_scale);
    #pragma endregion

    #pragma region ���[���h���W�̍X�V�Ǝ擾
    /// @brief ���[���h���W�n�������I�ɍX�V
    void              UpdateWorldTransform();

    /// @brief ���[���h���W���擾
    const VECTOR&     GetWorldPosition();

    /// @brief ���[���h��̎p�����擾
    const Quaternion& GetWorldRotation();

    /// @brief ���[���h��̃X�P�[�����擾
    const VECTOR&     GetWorldScale();

    /// @brief ���[���h���W��ݒ�
    void              SetWorldPosition(const VECTOR& pos);

    /// @brief ���[���h��̎p����ݒ�
    void              SetWorldRotation(const Quaternion& rot);
    #pragma endregion

    #pragma region �ړ��E��]����
    /// @brief �I�u�W�F�N�g�̕��s�ړ�
    /// @param world_space ���[���h���W��
    void Translate  (const VECTOR& translation,  bool world_space = false);

    /// @brief �I�u�W�F�N�g����]������
    /// @param world_space ���[���h���W��
    void Rotate     (const Quaternion& rotation, bool world_space = false);

    /// @brief �I�u�W�F�N�g���I�C���[�p�ŉ�]������
    /// @param world_space ���[���h���W��
    void RotateEuler(const VECTOR& euler_angles, bool world_space = false);

    /// @brief target �̕���������
    /// @param up ��]�� 
    void LookAt     (const VECTOR& target, const VECTOR& up = VGet(0.0f, 1.0f, 0.0f));
    #pragma endregion

    #pragma region �����x�N�g��
    /// @brief �I�u�W�F�N�g�̑O���F+Z
    VECTOR GetForward();

    /// @brief �I�u�W�F�N�g�̉E���F+X
    VECTOR GetRight();

    /// @brief �I�u�W�F�N�g�̏���F+Y
    VECTOR GetUp();
    #pragma endregion

    #pragma region �e�q�֌W
    /// @brief �e�̏����擾
    Transform*                     GetParent()            const;

    /// @brief �e��o�^����
    void                           SetParent(Transform* new_parent);

    /// @brief �o�^����Ă���q�̃R���e�i���擾
    const std::vector<Transform*>& GetChildren()          const;

    /// @brief �o�^����Ă���q�̏����擾
    /// @param index �Ԗڂɓo�^���ꂽ�q
    Transform*                     GetChild(size_t index) const;

    /// @brief �q�̐ڑ������擾
    size_t                         GetChildCount()        const;
    #pragma endregion

    #pragma region �s��擾
    /// @brief ���[���h���W�s��
    const MATRIX& GetWorldMatrix();

    /// @brief ���[�J�����W�s��
    MATRIX GetLocalMatrix() const;
    #pragma endregion

private:
    VECTOR     local_position_;
    Quaternion local_rotation_;
    VECTOR     local_scale_;

    VECTOR     world_position_;
    Quaternion world_rotation_;
    VECTOR     world_scale_;
    MATRIX     world_matrix_;
    bool       is_dirty_;

    Transform*              parent_;
    std::vector<Transform*> children_;

private:
    void AddChild(Transform* child);

    void RemoveChild(Transform* child);

    void MarkDirty();

};