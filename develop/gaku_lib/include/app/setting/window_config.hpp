#pragma once
#include "misc/json_loader.hpp"

/// @brief ウィンドウ設定
struct WindowConfig
{
    WindowConfig()
        : is_full_screen(JsonLoader::GetInstance()->GetConfigData()["WindowConfig"]["is_full_screen"])
        , project_name(JsonLoader::GetInstance()->GetConfigData()["WindowConfig"]["project_name"])
        , screen_width(JsonLoader::GetInstance()->GetConfigData()["WindowConfig"]["screen_width"])
        , screen_height(JsonLoader::GetInstance()->GetConfigData()["WindowConfig"]["screen_height"])
        , color_bit(JsonLoader::GetInstance()->GetConfigData()["WindowConfig"]["color_bit"])
    {
    };

    bool        is_full_screen;
    std::string project_name;
    int         screen_width;
    int         screen_height;
    int         color_bit;
};