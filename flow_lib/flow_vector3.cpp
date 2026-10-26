#include "flow_vector3.h"

namespace flow {

    const Vector3 Vector3::ZERO{0, 0, 0};
    const Vector3 Vector3::ONE{1, 1, 1};

    const Vector3 Vector3::FORWARD{0, 0, 1};
    const Vector3 Vector3::UP{0, 1, 0};
    const Vector3 Vector3::RIGHT{1, 0, 0};

    float Vector3::mag()
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    void Vector3::normalize()
    {
        float magnitude = mag();
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }
}