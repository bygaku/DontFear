#pragma once
#include <string>

struct AnimationTimeState
{
	int   info;						/// @brief アニメーションの情報
	int   attach_index;				/// @brief アタッチ中の配列番号
	float play_timer;				/// @brief 再生フレーム位置

	AnimationTimeState()
		: info(-1)
		, attach_index(-1)
		, play_timer(0.0f) 
	{
	}

	AnimationTimeState(int info, int attach_index, float play_timer)
		: info(info)
		, attach_index(attach_index)
		, play_timer(play_timer)
	{
	}
};

struct AnimationInfo
{
	int			animation_handle;	/// @brief アニメーションハンドル
	std::string tag;				/// @brief アニメーションのタグ
	float		play_speed;			/// @brief 再生速度
	bool		is_loop;			/// @brief ループを強制する

	AnimationInfo()
		: animation_handle(-1)
		, tag("")
		, play_speed(0.0f)
		, is_loop(true) 
	{
	}

	AnimationInfo(int anim_handle, std::string tag, float play_speed, bool is_loop)
		: animation_handle(anim_handle)
		, tag(tag)
		, play_speed(play_speed)
		, is_loop(is_loop)
	{
	}
};