#ifndef ASSERT_MACRO_HPP
#define ASSERT_MACRO_HPP
#pragma once

#include <DxLib.h>
#include <string_view>
#include <source_location>

namespace utility
{
    /// @brief �G���[���b�Z�[�W��\�����ďI������֐�
    [[noreturn]] void errorAssert(
        std::string_view conditional_expression,
        std::string_view error_mes,
        std::string_view file,
        std::string_view func,
        int line);

    /// @brief �G���[�Ɋ֘A����萔
    namespace error_constants 
    {
        constexpr int  kExitCode = 99;
        constexpr auto kAssertionHeader =
            "Assertion Error! Please press 'X button' or 'Escape key' on the window to exit!\n\n";
    }
}

#ifdef _DEBUG
/// @brief �G���[�����������Ƃ��ɃG���[���b�Z�[�W��\������B
/// DxLib �̓�����~�߁A�Ǝ��̃G���[���b�Z�[�W��\������B
/// DxLib ������������Ă��Ȃ��ꍇ�́A��O�𓊂���B
/// @param expr TRUE������
/// @param error_message �G���[���b�Z�[�W
#define ASSERT(expr, error_message)                                     \
do {                                                                    \
    const auto result_ = (expr);                                        \
    if (!(result_))                                                     \
    {                                                                   \
        const auto location = std::source_location::current();          \
        if (DxLib_IsInit() != TRUE)                                     \
        {                                                               \
            std::fprintf(stderr, "DxLib is not initialized.\n");        \
            std::exit(1);                                               \
        }                                                               \
        else                                                            \
        {                                                               \
            utility::errorAssert(#expr, error_message,                  \
                location.file_name(), location.function_name(),         \
                location.line());                                       \
        }                                                               \
    }                                                                   \
} while(false)

/// @brief �|�C���^��nullptr�łȂ����Ƃ��m�F����
#define ASSERT_NOT_NULL_PTR(ptr)         \
ASSERT((ptr) != nullptr, "nullptr.")     \

#else

#define ASSERT(expr, error_mes)  ((void)0)
#define ASSERT_NOT_NULL_PTR(ptr) ((void)0)

#endif

#endif // ASSERT_MACRO_HPP