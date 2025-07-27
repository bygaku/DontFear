#pragma once

/// @brief 物理計算データ
class Rigidbody final
{
public:
    /// @brief インスタンスを初期化
    Rigidbody();

    /// @brief インスタンスを解放
    ~Rigidbody();

    /// @brief オブジェクトを初期化
    void Initialize() noexcept;

    /// @brief 座標を取得
    const VECTOR& GetPosition()  const noexcept { return position_;    }

    /// @brief 向きを取得
    const VECTOR& GetDirection() const noexcept { return direction_;   }

    /// @brief 移動量を取得
    const VECTOR& GetVelocity()  const noexcept { return velocity_;    }
    
    /// @brief 重力をかけるオブジェクト判別を取得
    bool          OnGravity()    const noexcept { return use_gravity_; }

    /// @brief 座標を設定
    void SetPosition  (const VECTOR& position)        noexcept;

    /// @brief 移動量を設定
    void SetVelocity  (const VECTOR& velocity)        noexcept;

    /// @brief このオブジェクトに重力をかける
    void SetUseGravity(bool gravity)		          noexcept;

    /// @brief 衝突の力を加える
    void AddForce     (const VECTOR& force_direction) noexcept;

    /// @brief 次フレームの座標を保存
    void SaveFuturePosition(const VECTOR& future_pos) noexcept;

    /// @brief 次フレームの座標を取得
    const VECTOR& GetFuturePosition() const noexcept { return future_pos_; }
private:
    VECTOR	   position_;			/// @brief 座標
    VECTOR	   direction_;			/// @brief 向き
    VECTOR	   velocity_;			/// @brief 速度ベクトル
    VECTOR     future_pos_;         /// @brief 次フレームの座標
    bool	   use_gravity_;		/// @brief 重力をかけるオブジェクト

};
