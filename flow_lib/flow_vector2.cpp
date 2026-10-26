#include "flow_vector2.h"

namespace flow {
    float Vector2::Distance(Vector2 first, Vector2 second)
    {
        return (first - second).Magnitude();
    }

    float Vector2::Magnitude()
    {
        return std::sqrt(x * x + y * y);
    }

    const Vector2 Vector2::ZERO = {0, 0};
    const Vector2 Vector2::ONE = {1, 1};
}