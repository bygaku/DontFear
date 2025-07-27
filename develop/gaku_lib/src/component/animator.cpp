#include "component/animator.hpp"
#include "app/game_time.hpp"

Animator::Animator(GameObject& owner, std::shared_ptr<Modeler> modeler)
    : Component { owner }
    , prev_animation_play_rate_(0.0f)
    , blend_rate_(0.0f)
    , blend_speed_(1.0f)
    , is_first_frame_change_animation_(false)
    , modeler_(modeler)
{
    time_state_data_[TimeState::kPrev] = time_state_data_[TimeState::kCurrent] = AnimationTimeState();
}

Animator::~Animator()
{
    for (auto& data : animation_info_) {
        MV1DeleteModel(data.second.animation_handle);
    }
}

void Animator::Initialize() noexcept
{
    DetachAnimation(TimeState::kPrev);
    DetachAnimation(TimeState::kCurrent);
}

void Animator::Update() noexcept
{
    is_first_frame_change_animation_ = false;

    BlendAnimation();
    PlayAnimation();
}

void Animator::AddAnimationHandle(const int kind, const std::string& file_path, const std::string& tag, const float play_speed, const bool is_loop) noexcept
{
    // 上書き不可
    if (animation_info_.count(kind)) return;

    auto handle = MV1LoadModel(file_path.c_str());
    animation_info_[kind] = AnimationInfo(handle, tag, play_speed, is_loop);
}

void Animator::SetAnimationBlendSpeed(const float blend_speed)
{
    blend_speed_ = blend_speed;
}

void Animator::AttachAnimation(const int next_anim_kind) noexcept
{
    // 現在のアニメーションと同じであった場合は変更しない
    if (time_state_data_.at(TimeState::kCurrent).info == next_anim_kind) return;

    is_first_frame_change_animation_ = true;

    DetachAnimation(TimeState::kPrev);

    // データを移行(Current ➡ Prev, Next ➡ Current)
    time_state_data_.at(TimeState::kPrev)				  = time_state_data_.at(TimeState::kCurrent);
    time_state_data_.at(TimeState::kCurrent).attach_index = MV1AttachAnim(modeler_->GetModelHandle(), 1, animation_info_.at(next_anim_kind).animation_handle, TRUE);
    time_state_data_.at(TimeState::kCurrent).info		  = next_anim_kind;
    SetPlayStartTime();

    // 前回のアニメーションが存在しない場合は、ブレンド済み(ブレンド率100%)とする
    blend_rate_ = time_state_data_.at(TimeState::kPrev).attach_index > -1 ? 0.0f : 1.0f;
}

void Animator::DetachAnimation(const TimeState time_state) noexcept
{
    if (time_state_data_.at(time_state).attach_index > -1) {
        MV1DetachAnim(modeler_->GetModelHandle(), time_state_data_.at(time_state).attach_index);
        time_state_data_.at(time_state).attach_index = -1;
    }
}

void Animator::SetPlayStartTime() noexcept
{
    if (animation_info_.count(time_state_data_.at(TimeState::kPrev).info)) {
        std::string prev_tag    = animation_info_.at(time_state_data_.at(TimeState::kPrev).info).tag;
        std::string current_tag = animation_info_.at(time_state_data_.at(TimeState::kCurrent).info).tag;

        // 同類アニメーションであった場合は再生率を引き継ぐ
        if (prev_tag == current_tag) {
            float current_total_t = MV1GetAttachAnimTotalTime(modeler_->GetModelHandle(), time_state_data_.at(TimeState::kCurrent).attach_index);
            float prev_total_t    = MV1GetAttachAnimTotalTime(modeler_->GetModelHandle(), time_state_data_.at(TimeState::kPrev).attach_index);

            prev_animation_play_rate_ = time_state_data_.at(TimeState::kPrev).play_timer / prev_total_t;
            time_state_data_.at(TimeState::kCurrent).play_timer = current_total_t * prev_animation_play_rate_;

            return;
        }
    }

    time_state_data_.at(TimeState::kCurrent).play_timer = 0.0f;
}

void Animator::PlayAnimation() noexcept
{
    for (auto& [state_t, data] : time_state_data_) {
        if (data.attach_index > -1) {
            float total_t = MV1GetAttachAnimTotalTime(modeler_->GetModelHandle(), data.attach_index);
            float blend_r = state_t == TimeState::kCurrent ? blend_rate_ : 1.0f - blend_rate_;

            data.play_timer += animation_info_.at(data.info).play_speed * GameTime::GetDeltaTime();

            if (data.play_timer > total_t) {
                data.play_timer = animation_info_.at(data.info).is_loop ? 0.0f : total_t;
            }

            MV1SetAttachAnimTime(modeler_->GetModelHandle(), data.attach_index, data.play_timer);
            MV1SetAttachAnimBlendRate(modeler_->GetModelHandle(), data.attach_index, blend_r);
        }
    }
}

void Animator::BlendAnimation() noexcept
{
    if (blend_rate_ >= 1.0f) return;

    blend_rate_ += blend_speed_ * GameTime::GetDeltaTime();

    if (blend_rate_ > 1.0f) {
        blend_rate_ = 1.0f;
    }
}