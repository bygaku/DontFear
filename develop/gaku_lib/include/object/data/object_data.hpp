#pragma once
#include <string>

//static struct ObjectTag
//{
//    std::string kBox     = "Box";
//    std::string kText    = "Text";
//    std::string kCamera  = "Camera";
//    std::string kPlayer  = "Player";
//    std::string kPlane   = "Plane";
//    std::string kScore   = "Score";
//}ObjectTag;

/// @brief �I�u�W�F�N�g���ʃ^�O
enum class ObjectTag {
    Wall,           
    SystemWall,
    Skydome,
    Camera,
    Player,
    Enemy,
    Car,
};