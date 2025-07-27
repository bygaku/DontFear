#pragma once
#include <nlohmann/json.hpp>
#include "data/json_file_type.hpp"

using json = nlohmann::json;

class JsonLoader
{
public:
	/// @brief インスタンスを生成
	static void Create()  noexcept;

	/// @brief インスタンスを破棄
	static void Destroy() noexcept;

	/// @brief インスタンスを取得
	static JsonLoader* GetInstance() noexcept { return instance_; }

	/// @brief コンフィグデータを取得
	json GetConfigData() const;

	/// @brief セーブデータを取得
	json GetSaveData()	 const;

	/// @brief  string型のwstring型変換
	/// @param	str: 変換する文字列
	/// @return	変換した文字列
	std::wstring Widening(const std::string& str) noexcept;

private:
	/// @brief インスタンスを初期化
	JsonLoader();

	/// @brief インスタンスを解放
	~JsonLoader();

	/// @brief ロード成功したか
	bool IsLoadingFile();

private:
	std::unordered_map<std::string, json>	json_data_;		/// @brief Jsonのデータ
	static		 JsonLoader*				instance_;		/// @brief クラスのインスタンス

private:
	static const std::string config_data_;	/// @brief config_data.jsonのファイルパス
	static const std::string save_data_;	/// @brief save_data.jsonのファイルパス

};

