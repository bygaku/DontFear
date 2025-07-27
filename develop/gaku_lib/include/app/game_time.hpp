#pragma once
#include <memory>

class GameTime final
{
public:	
    /// @brief インスタンスを初期化
    GameTime()  noexcept;

    /// @brief インスタンスを解放
    ~GameTime();

    /// @brief フレームカウントを +1
    /// @remark 毎フレーム処理前に呼び出す
    void IncrementFrame()                             noexcept;

    /// @brief 固定フレームの更新回数を更新して返す
    int UpdateFixedFrameCount()                       noexcept;

    /// @brief 現在の1フレーム処理時間 (seconds)
    [[nodiscard]] double GetCurrentFrameTime()  const noexcept;

    /// @brief fps_target_ として指定されている時間まで待つ
    void SleepForNextFrame()                    const noexcept;

    /// @brief fps 制限を指定
    void SetFpsTarget(int target)                     noexcept;

    /// @brief タイムスケールを設定
    static void SetTimeScale(const float& scale)      noexcept;

    /// @brief タイムスケールを取得：通常倍速は 1.f
    static const float& GetTimeScale()                noexcept { return time_scale_; }

    /// @brief 2つ前と1つ前のフレーム間の時間差 (seconds)
    /// @brief time_scaler_ によるリスケールに注意
    static const float& GetDeltaTime()                noexcept { return delta_time_; }

    /// @brief 現実時間に基づいた delta_time_を取得
    /// @brief time_scaler_ の影響を受けない
    static const float& GetUnscaledDeltaTime()        noexcept { return unscaled_delta_time_; }

    /// @brief 起動からの時間を取得
    /// @brief time_scaler_ の影響を受けない
    const float& GetBootTimer()                       noexcept;

    /// @brief カウントアップタイマーを取得
    /// @brief time_scaler_ の影響を受けない
    const float& GetUnscaledTimer()                   noexcept;

    /// @brief カウントアップタイマーを取得
    const float& GetTimer()                           noexcept;

private:
#pragma region privateメソッド
    /// @brief 現在のフレームの時間
    [[nodiscard]] const LONGLONG& GetTime()                 const noexcept { return time_; }

    /// @brief 最後に fps を更新した時間 (nanoseconds)
    [[nodiscard]] const LONGLONG& GetLastCheckedTime()      const noexcept { return fps_check_time_; }

    /// @brief 起動からの時間 (seconds)
    [[nodiscard]] const float&    GetTimeFromBoot()         const noexcept { return time_since_start_up_; }

    /// @brief 現在の frames per second
    [[nodiscard]] const int&      GetFps()                  const noexcept { return fps_; };

    /// @brief 最後の fps 更新からのフレームカウント
    [[nodiscard]] const int&      GetFpsCounter()           const noexcept { return fps_counter_; };

    /// @brief 現在の fps 制限
    [[nodiscard]] const int&      GetFpsTarget()            const noexcept { return fps_target_; };

    /// @brief 現在の fps 制限によるフレーム間の秒数 (seconds)
    [[nodiscard]] const double&   GetSecondsPerFrame()      const noexcept { return seconds_per_frame_; };

    /// @brief 最後の固定間隔更新の回数
    [[nodiscard]] const int&      GetLastFixedFrameCount()  const noexcept { return last_fixed_frame_count_; };

    /// @brief 起動してからのフレーム数
    [[nodiscard]] const UINT&     GetFrames()               const noexcept { return frames_; };
#pragma endregion

private:
    static bool  instantiated_;
    static float delta_time_;
    static float unscaled_delta_time_;
    static float time_scale_;

private:
    LONGLONG time_;
    LONGLONG fps_check_time_;
    LONGLONG fixed_update_check_time_;
    float    time_since_start_up_;
             
    UINT     frames_;
    int      fps_;
    int      fps_target_;
    int      fps_counter_;
    double   seconds_per_frame_;
    int      last_fixed_frame_count_;

    float    boot_timer_;
    float    timer_;
    float    unscaled_timer_;
};