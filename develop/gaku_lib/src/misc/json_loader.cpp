#include <iostream>
#include <fstream>
#include <sstream>
#include "misc/json_loader.hpp"
#include "misc/assert_dx.hpp"

JsonLoader* JsonLoader::instance_          = nullptr;
const std::string JsonLoader::config_data_ = "../resources/json/config.json";
const std::string JsonLoader::save_data_   = "../resources/json/save.json";

JsonLoader::JsonLoader()
{
    auto load_succeed = IsLoadingFile();
    ASSERT(load_succeed, " json file couldn't be load.");
}

JsonLoader::~JsonLoader()
{
}

bool JsonLoader::IsLoadingFile()
{
    try {
        std::ifstream config_data (config_data_.c_str());
        std::ifstream save_data	  (save_data_.c_str());

        if (!config_data.good())  return false;
        if (!save_data.good())    return false;

        /// @remark 各データ代入
        config_data  >> json_data_[JsonFile.Config];
        save_data    >> json_data_[JsonFile.Save];
    }
    catch (...) {
        return false;
    }

    return true;
}

//インスタンス生成
void JsonLoader::Create() noexcept
{
    if (instance_) return;
    instance_ = new JsonLoader();
}

//インスタンス破棄
void JsonLoader::Destroy() noexcept
{
    if (!instance_) return;
    delete instance_;
}

json JsonLoader::GetConfigData() const
{
    auto& data = json_data_.at(JsonFile.Config);
    return data;
}

json JsonLoader::GetSaveData() const
{
    auto& data = json_data_.at(JsonFile.Save);
    return data;
}

//文字列の型変換
std::wstring JsonLoader::Widening(const std::string& str) noexcept
{
    std::wostringstream wstm;
    const std::ctype<wchar_t>& ctfacet = use_facet<std::ctype<wchar_t>>(wstm.getloc());

    for (size_t i = 0; i < str.size(); ++i) {
        wstm << ctfacet.widen(str[i]);
    }

    return wstm.str();
}