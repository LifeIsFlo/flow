#pragma once
#include "flow_vector3.h"

namespace flow
{
    struct Transform
    {
        Vector3 scale{1, 1, 1};
        Vector3 position{0, 0, 0};
        Vector3 rotation{0, 0, 0};

        Vector3 GetRight() const;
        Vector3 GetForward() const;
        Vector3 GetUp() const;
        Vector3 GetLeft() const;
        Vector3 GetBackward() const;
        Vector3 GetDown() const;
    };
}