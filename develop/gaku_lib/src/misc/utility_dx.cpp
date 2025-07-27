#include <DxLib.h>
#include "misc/utility_dx.hpp"

namespace utility
{
    dx_math::Vector2D_I GetCenterScreen() noexcept
    {
        dx_math::Vector2D_I size{};
        GetWindowSize(&size.x_, &size.y_);
        
        dx_math::Vector2D_I center{ size.x_ / 2, size.y_ / 2 };
        return center;
    }
}