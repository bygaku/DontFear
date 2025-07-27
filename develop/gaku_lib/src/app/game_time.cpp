#include <DxLib.h>
#include "app/game_time.hpp"

bool  GameTime::instantiated_        = false;
float GameTime::delta_time_          = 0.f;
float GameTime::unscaled_delta_time_ = 0.f;
float GameTime::time_scale_          = 1.f;

GameTime::GameTime() noexcept
    : time_(0)
    , fps_check_time_(0)
    , fixed_update_check_time_(0)
    , time_since_start_up_(0.f)
    , fps_(0)
    , fps_counter_(0)
    , last_fixed_frame_count_(0)
    , fps_target_(60)
    , seconds_per_frame_(1.0 / fps_target_)
    , frames_(0)
    , boot_timer_(0.f)
    , unscaled_timer_(0.f)
    , timer_(0.f)
{
    instantiated_ = true;

    // 起動時のシステム時間を取得
    time_                    = GetNowHiPerformanceCount();
    fps_check_time_          = GetNowHiPerformanceCount();
    fixed_update_check_time_ = GetNowHiPerformanceCount();
}

GameTime::~GameTime()
{
    instantiated_ = false;
}

void GameTime::IncrementFrame() noexcept
{ 
    // フレーム終了時のシステム時間
    const LONGLONG now = GetNowHiPerformanceCount();

    // デルタタイム算出
    unscaled_delta_time_  = (now - time_) / 1000000.0f;
    delta_time_           = unscaled_delta_time_ * time_scale_;
    time_                 = now;

    // 1秒経過する間に実行されたメインループの回数を FPS とする
    ++fps_counter_;

    if (now - fps_check_time_ > 1000000.0f) {
        fps_            = fps_counter_;
        fps_counter_    = 0;
        fps_check_time_ = now;
    }

    // delta_time_ を毎フレーム足して、秒算出
    time_since_start_up_ += unscaled_delta_time_;
}

int GameTime::UpdateFixedFrameCount() noexcept
{
    int frame_counts = 0;

    while (fixed_update_check_time_ < time_) {
        fixed_update_check_time_ += 20000;
        frame_counts++;
    }

    last_fixed_frame_count_ = frame_counts;
    
    return frame_counts;
}

double GameTime::GetCurrentFrameTime() const noexcept
{
    // 1フレームの時間を算出して返す
    const LONGLONG now = GetNowHiPerformanceCount();
    return (now - time_) / 1000000.0;
}

void GameTime::SleepForNextFrame() const noexcept
{
    auto frame_time = GetCurrentFrameTime();

    if (GetSecondsPerFrame() > frame_time) {
        WaitTimer(static_cast<int>(GetSecondsPerFrame() - frame_time) * 1000);
    }
}

void GameTime::SetFpsTarget(int target) noexcept
{
    if (target < 0) {
        fps_target_        = target;
        seconds_per_frame_ = 0;

        return;
    }

    fps_target_        = target;
    seconds_per_frame_ = 1.0 / fps_target_;
}

const float& GameTime::GetBootTimer() noexcept
{
    boot_timer_ += unscaled_delta_time_;
    return boot_timer_;
}

const float& GameTime::GetUnscaledTimer() noexcept
{
    unscaled_timer_ += unscaled_delta_time_;
    return unscaled_timer_;
}

const float& GameTime::GetTimer() noexcept
{
    timer_ += delta_time_;
    return timer_;
}

void GameTime::SetTimeScale(const float& scale) noexcept
{
    time_scale_ = scale;
}
