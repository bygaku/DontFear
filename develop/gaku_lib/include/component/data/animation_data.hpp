#pragma once
#include <string>

struct AnimationTimeState
{
	int   info;						/// @brief �A�j���[�V�����̏��
	int   attach_index;				/// @brief �A�^�b�`���̔z��ԍ�
	float play_timer;				/// @brief �Đ��t���[���ʒu

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
	int			animation_handle;	/// @brief �A�j���[�V�����n���h��
	std::string tag;				/// @brief �A�j���[�V�����̃^�O
	float		play_speed;			/// @brief �Đ����x
	bool		is_loop;			/// @brief ���[�v����������

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