#pragma once
#include <unordered_map>
#include "modeler.hpp"
#include "data/animation_data.hpp"

/// @brief アニメーション管理
class Animator final : public Component
{
public:
    enum class TimeState
    {
        kPrev,
        kCurrent,
    };

public:
    /// @remark 禁止コンストラクタ
    Animator() = delete;

    /// @brief インスタンスを初期化
    /// @param owner コンポーネント所有者
    /// @param modeler モデル管理
    Animator(GameObject& owner, std::shared_ptr<Modeler> modeler);

    /// @brief インスタンスを解放
    ~Animator()		           override;

    /// @brief コンポーネントを初期化
    void Initialize() noexcept override;

    /// @brief アニメーターを毎フレーム更新
    void Update()	  noexcept;

    /// @brief アニメーションをアタッチ
    /// @param next_anim_kind 次に再生したいアニメーション
    void AttachAnimation   (const int next_anim_kind) noexcept;

    /// @brief アニメ―ションを追加
    /// @param kind アニメーションの種類
    /// @param file_path ファイルパス
    /// @param tag アニメーションの登録名
    /// @param play_speed アニメーションのスピード ( 30 Frame: 100% speed )
    /// @param is_loop 強制ループするアニメーションか判定
    void AddAnimationHandle(const int kind, const std::string& file_path, const std::string& tag, const float play_speed, const bool is_loop) noexcept;

    /// @brief ブレンドスピードを設定
    void SetAnimationBlendSpeed(const float blend_speed);
private:
    /// @brief アニメーションをデタッチ
    void DetachAnimation(const TimeState time_state)  noexcept;

    /// @brief スタートタイムを設定
    void SetPlayStartTime()                           noexcept;
                                                      
    /// @brief アニメーション再生
    void PlayAnimation()                              noexcept;

    /// @brief アニメーションブレンド開始
    void BlendAnimation()                             noexcept;

private:
    std::unordered_map<int, AnimationInfo>			  animation_info_;
    std::unordered_map<TimeState, AnimationTimeState> time_state_data_;

    float prev_animation_play_rate_;
    float blend_rate_;
    float blend_speed_;
    bool  is_first_frame_change_animation_;

    std::shared_ptr<Modeler> modeler_;
};