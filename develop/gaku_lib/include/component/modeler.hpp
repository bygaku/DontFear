#pragma once
#include <string>
#include "component.hpp"
#include "transform.hpp"

/// @brief モデル管理
class Modeler final : public Component
{
public:	
    /// @remark 禁止コンストラクタ
    Modeler() = delete;

    /// @brief インスタンスを初期化
    /// @param owner コンポーネント所有者
    Modeler(GameObject& owner);

    /// @brief インスタンスを解放
    ~Modeler()                 override;

    /// @brief コンポーネントを初期化
    void Initialize() noexcept override;

    /// @brief モデルを描画
    void Draw() const noexcept;

    /// @brief モデルハンドルを取得
    int	 GetModelHandle() const                 noexcept;

    /// @brief モデルハンドルを設定
    /// @param model_path モデルのファイルパス
    void SetModelHandle(std::string file_path)  noexcept;

    /// @brief モデルハンドルを設定
    /// @param model_handle すでにロード済みのハンドル
    void SetModelHandle(int model_handle)       noexcept;

    /// @brief モデルハンドルをリセット
    void ResetModelHandle()                     noexcept;

    /// @brief 行列
    void SetMatrix(MATRIX mat) const            noexcept;

private:
    int model_handle_;  /// @brief モデルハンドル

};