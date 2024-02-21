#include "quaternion.h"
#include "matrix.h"

Quaternion::Quaternion() : x(0), y(0), z(0), w(1) {}

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Quaternion::Quaternion(Vector3 eulerAngles)
{
    float pitch = eulerAngles.x * M_PI / 180.0;
    float roll = eulerAngles.y * M_PI / 180.0;
    float yaw = eulerAngles.z * M_PI / 180.0;

    Quaternion qx(std::sin(pitch / 2), 0, 0, std::cos(pitch / 2));
    Quaternion qy(0, std::sin(roll / 2), 0, std::cos(roll / 2));
    Quaternion qz(0, 0, std::sin(yaw / 2), std::cos(yaw / 2));

    Quaternion q = qy * qx * qz;
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

Vector3 operator *(Quaternion q, Vector3 v)
{
    return MatrixRotate(q) * Vector4(v, 1);
}

Quaternion operator *(Quaternion lhs, Quaternion rhs)
{
    return Quaternion(
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

std::ostream& operator<<(std::ostream& s, const Quaternion& q)
{
    return s << "(" << q.x << ",\t" << q.y << ",\t" << q.z << ",\t" << q.w << ")";
}