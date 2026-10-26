#include "flow_transform.h"
#include <math.h>
#include <numbers>

namespace flow
{
    Vector3 Transform::GetRight() const
    {
        float yaw = rotation.y + std::numbers::pi / 2;

        Vector3 right = {sin(yaw),
                         0,
                         cos(yaw)};

        return right;
    }

    Vector3 Transform::GetForward() const
    {
        float yaw = rotation.y + std::numbers::pi / 2;
        float pitch = rotation.x;

        Vector3 forward = {
            -cos(pitch) * cos(yaw),
            -sin(pitch),
            cos(pitch) * sin(yaw)};

        return forward;
    }

    Vector3 Transform::GetUp() const
    {
        float yaw = rotation.y + std::numbers::pi / 2;
        float pitch = rotation.x - std::numbers::pi / 2;

        Vector3 up = {
            -cos(pitch) * cos(yaw),
            -sin(pitch),
            cos(pitch) * sin(yaw)};

        return up;
    }

    Vector3 Transform::GetLeft() const
    {
        return GetRight() * -1;
    }

    Vector3 Transform::GetBackward() const
    {
        return GetForward() * -1;
    }

    Vector3 Transform::GetDown() const
    {
        return GetUp() * -1;
    }
}