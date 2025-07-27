#pragma once
#include <cmath>

namespace dx_math
{
    class Vector2D final
    {
    public:
        float x_, y_;

        explicit Vector2D()
            : x_(0.0f), y_(0.0f)
        {
        };

        explicit Vector2D(float x, float y)
            : x_(x), y_(y)
        {
        };

        Vector2D operator+(const Vector2D& rhs) const noexcept
        {
            return Vector2D(x_ + rhs.x_, y_ + rhs.y_);
        }

        Vector2D operator-(const Vector2D& rhs) const noexcept
        {
            return Vector2D(x_ - rhs.x_, y_ - rhs.y_);
        }

        Vector2D operator*(float scale)         const noexcept
        {
            return Vector2D(x_ * scale, y_ * scale);
        }

        float Length()                          const noexcept
        {
            return sqrt(x_ * x_ + y_ * y_);
        }

        Vector2D Normalized()                   const noexcept
        {
            float len = Length();
            if (len > 0.0f)  {
                return Vector2D(x_ / len, y_ / len);
            }
            return Vector2D(0.0f, 0.0f);
        }
    };

    class Vector2D_I final
    {
    public:
        int x_, y_;
    
        explicit Vector2D_I()
            : x_(0), y_(0)
        {
        };
    
        explicit Vector2D_I(int x, int y)
            : x_(x), y_(y)
        {
        };
    
        Vector2D_I operator+(const Vector2D_I& rhs) const noexcept
        {
            return Vector2D_I(x_ + rhs.x_, y_ + rhs.y_);
        }
    
        Vector2D_I operator-(const Vector2D_I& rhs) const noexcept
        {
            return Vector2D_I(x_ - rhs.x_, y_ - rhs.y_);
        }
    
        Vector2D_I operator*(int scale)             const noexcept
        {
            return Vector2D_I(x_ * scale, y_ * scale);
        }
    
        float length()                              const noexcept
        {
            return static_cast<float>(sqrt(x_ * x_ + y_ * y_));
        }
    
        Vector2D_I normalized()                     const noexcept
        {
            float len = length();
            if (len > 0.0f) {
                return Vector2D_I(x_ / static_cast<int>(len), y_ / static_cast<int>(len));
            }
            return Vector2D_I(0, 0);
        }
    };

}
