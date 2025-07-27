#include <DxLib.h>
#include "component/transform.hpp"

Transform::Transform(GameObject& object)
    : Component(object)
    , local_position_(VGet(0.0f, 0.0f, 0.0f))
    , local_rotation_(Quaternion::Identity())
    , local_scale_(VGet(1.0f, 1.0f, 1.0f))
    , world_position_(VGet(0.0f, 0.0f, 0.0f))
    , world_rotation_(Quaternion::Identity())
    , world_scale_(VGet(1.0f, 1.0f, 1.0f))
    , is_dirty_(true)
    , parent_(nullptr)
{
    world_matrix_ = MGetIdent();
}

Transform::~Transform()
{
    // �e���玩�����폜
    if (parent_) {
        parent_->RemoveChild(this);
    }

    // �q�������̐e��null�ɐݒ�
    for (auto* child : children_) {
        if (child) {
            child->parent_ = nullptr;
        }
    }
}

void Transform::Initialize() noexcept
{
    is_dirty_ = true;
}

#pragma region GET AND SET LOCAL TRANSFORM
const VECTOR& Transform::GetLocalPosition() const
{
    return local_position_;
}

void Transform::SetLocalPosition(const VECTOR& pos)
{
    local_position_ = pos;
    MarkDirty();
}

void Transform::SetLocalPosition(float x, float y, float z)
{
    local_position_ = VGet(x, y, z);
    MarkDirty();
}

const Quaternion& Transform::GetLocalRotation() const
{
    return local_rotation_;
}

void Transform::SetLocalRotation(const Quaternion& rot)
{
    local_rotation_ = rot.Normalized();
    MarkDirty();
}

void Transform::SetLocalRotationEuler(const VECTOR& euler)
{
    local_rotation_ = Quaternion::FromEulerRadians(euler.x, euler.y, euler.z);
    MarkDirty();
}

void Transform::SetLocalRotationEuler(float x, float y, float z)
{
    local_rotation_ = Quaternion::FromEulerRadians(x, y, z);
    MarkDirty();
}

const VECTOR& Transform::GetLocalScale() const
{
    return local_scale_;
}

void Transform::SetLocalScale(const VECTOR& scale)
{
    local_scale_ = scale;
    MarkDirty();
}

void Transform::SetLocalScale(float x, float y, float z)
{
    local_scale_ = VGet(x, y, z);
    MarkDirty();
}

void Transform::SetLocalScale(float uni_scale)
{
    local_scale_ = VGet(uni_scale, uni_scale, uni_scale);
    MarkDirty();
}
#pragma endregion

#pragma region UPDATE AND GET WORLD TRANSFORM
/// @brief ���[���h���W�n�������I�ɍX�V
/// @remark �X�V��Dirty��TRUE�̂Ƃ��̂�
void Transform::UpdateWorldTransform()
{
    if (!is_dirty_) return;

    if (parent_) {
        parent_->UpdateWorldTransform();

        // �e�̃��[���h�ϊ���K�p
        world_position_ = parent_->GetWorldPosition();
        world_rotation_ = parent_->GetWorldRotation();
        world_scale_    = parent_->GetWorldScale();

        // ���[�J���ϊ���K�p
        VECTOR scaled_pos  = VGet(
            local_position_.x * world_scale_.x,
            local_position_.y * world_scale_.y,
            local_position_.z * world_scale_.z
        );

        VECTOR rotated_pos = world_rotation_.RotateVector(scaled_pos);

        world_position_    = VAdd(world_position_, rotated_pos);
        world_rotation_    = world_rotation_ * local_rotation_;
        world_scale_       = VGet(
            world_scale_.x * local_scale_.x,
            world_scale_.y * local_scale_.y,
            world_scale_.z * local_scale_.z
        );
    }
    else {
        // �e�����Ȃ��ꍇ�̓��[�J�������[���h
        world_position_ = local_position_;
        world_rotation_ = local_rotation_;
        world_scale_    = local_scale_;
    }

    // ���[���h�s����X�V
    MATRIX scale_matrix = MGetScale(world_scale_);
    MATRIX rot_matrix   = world_rotation_.ToMatrix();
    MATRIX trans_matrix = MGetTranslate(world_position_);

    world_matrix_   = MMult(scale_matrix, MMult(rot_matrix, trans_matrix));
    is_dirty_       = false;
}

/// @brief ���[���h���W���擾
const VECTOR& Transform::GetWorldPosition()
{
    UpdateWorldTransform();
    return world_position_;
}

/// @brief ���[���h��̎p�����擾
const Quaternion& Transform::GetWorldRotation()
{
    UpdateWorldTransform();
    return world_rotation_;
}

/// @brief ���[���h��̃X�P�[�����擾
const VECTOR& Transform::GetWorldScale()
{
    UpdateWorldTransform();
    return world_scale_;
}

/// @brief ���[���h���W��ݒ�
void Transform::SetWorldPosition(const VECTOR& pos)
{
    if (parent_) {
        UpdateWorldTransform();

        VECTOR     parent_pos     = parent_->GetWorldPosition();
        Quaternion parent_rot_inv = parent_->GetWorldRotation().Inverse();
        VECTOR     parent_scale   = parent_->GetWorldScale();

        VECTOR local_pos = VSub(pos, parent_pos);
        local_pos        = parent_rot_inv.RotateVector(local_pos);
        local_pos        = VGet(
                           local_pos.x / parent_scale.x,
                           local_pos.y / parent_scale.y,
                           local_pos.z / parent_scale.z
                           );

        local_position_  = local_pos;
    }
    else {
        local_position_  = pos;
    }

    MarkDirty();
}

/// @brief ���[���h��̎p����ݒ�
void Transform::SetWorldRotation(const Quaternion& rot)
{
    if (parent_) {
        UpdateWorldTransform();

        Quaternion parent_rot_inv = parent_->GetWorldRotation().Inverse();
        local_rotation_           = parent_rot_inv * rot;
    }
    else {
        local_rotation_           = rot;
    }

    MarkDirty();
}
#pragma endregion

#pragma region MOVE/ROTATE FOR OBJECTS
/// @brief �I�u�W�F�N�g�̕��s�ړ�
/// @param world_space ���[���h���W��
void Transform::Translate(const VECTOR& translation, bool world_space)
{
    if (world_space) {
        local_position_ = VAdd(local_position_, translation);
    }
    else {
        VECTOR local_translation = local_rotation_.RotateVector(translation);
        local_position_ = VAdd(local_position_, local_translation);
    }

    MarkDirty();
}

/// @brief �I�u�W�F�N�g����]������
/// @param world_space ���[���h���W��
void Transform::Rotate(const Quaternion& rotation, bool world_space)
{
    if (world_space) {
        local_rotation_ = rotation * local_rotation_;
    }
    else {
        local_rotation_ = local_rotation_ * rotation;
    }

    local_rotation_.Normalize();

    MarkDirty();
}

/// @brief �I�u�W�F�N�g���I�C���[�p�ŉ�]������
/// @param world_space ���[���h���W��
void Transform::RotateEuler(const VECTOR& euler_angles, bool world_space)
{
    Quaternion rotation = Quaternion::FromEulerRadians(euler_angles.x, euler_angles.y, euler_angles.z);
    Rotate(rotation, world_space);
}

/// @brief target �̕���������
/// @param up ��]�� 
void Transform::LookAt(const VECTOR& target, const VECTOR& up)
{
    VECTOR world_pos = GetWorldPosition();
    VECTOR forward   = VNorm(VSub(target, world_pos));

    Quaternion look_rotation = Quaternion::FromToRotation(VGet(0.0f, 0.0f, 1.0f), forward);
    SetWorldRotation(look_rotation);
}
#pragma endregion

#pragma region OBJECT DIRECTION VECTOR
/// @brief �I�u�W�F�N�g�̑O���F+Z
VECTOR Transform::GetForward()
{
    return GetWorldRotation().RotateVector(VGet(0.0f, 0.0f, 1.0f));
}

/// @brief �I�u�W�F�N�g�̉E���F+X
VECTOR Transform::GetRight()
{
    return GetWorldRotation().RotateVector(VGet(1.0f, 0.0f, 0.0f));
}

/// @brief �I�u�W�F�N�g�̏���F+Y
VECTOR Transform::GetUp()
{
    return GetWorldRotation().RotateVector(VGet(0.0f, 1.0f, 0.0f));
}
#pragma endregion

#pragma region PARENT
/// @brief �e�̏����擾
Transform* Transform::GetParent() const
{
    return parent_;
}

/// @brief �e��o�^����
void Transform::SetParent(Transform* new_parent)
{
    if (parent_ == new_parent) return;

    // �Â��e����폜
    if (parent_) {
        parent_->RemoveChild(this);
    }

    // �V�����e�ɒǉ�
    parent_ = new_parent;
    if (parent_) {
        parent_->AddChild(this);
    }

    MarkDirty();
}

/// @brief �o�^����Ă���q�̃R���e�i���擾
const std::vector<Transform*>& Transform::GetChildren() const
{
    return children_;
}

/// @brief �o�^����Ă���q�̏����擾
/// @param index �Ԗڂɓo�^���ꂽ�q
Transform* Transform::GetChild(size_t index) const
{
    if (index < children_.size()) {
        return children_[index];
    }

    return nullptr;
}

/// @brief �q�̐ڑ������擾
size_t Transform::GetChildCount() const
{
    return children_.size();
}
#pragma endregion

#pragma region GET MATRIX
/// @brief ���[���h���W�s��
const MATRIX& Transform::GetWorldMatrix()
{
    UpdateWorldTransform();
    return world_matrix_;
}

/// @brief ���[�J�����W�s��
MATRIX Transform::GetLocalMatrix() const
{
    MATRIX scale_matrix = MGetScale(local_scale_);
    MATRIX rot_matrix   = local_rotation_.ToMatrix();
    MATRIX trans_matrix = MGetTranslate(local_position_);

    return MMult(scale_matrix, MMult(rot_matrix, trans_matrix));
}
#pragma endregion

#pragma region PRIVATE METHODS
void Transform::AddChild(Transform* child)
{
    if (child && std::find(children_.begin(), children_.end(), child) == children_.end()) {
        children_.push_back(child);
    }
}

void Transform::RemoveChild(Transform* child)
{
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        children_.erase(it);
    }
}

void Transform::MarkDirty()
{
    is_dirty_ = true;

    for (auto* child : children_) {
        if (child) {
            child->MarkDirty();
        }
    }
}
#pragma endregion