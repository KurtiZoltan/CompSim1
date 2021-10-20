#include "Math.hpp"

vec4 operator*(f32 a, const vec4& v)
{
    return v * a;
}

std::ostream& operator<<(std::ostream& os, const vec4& v)
{
    os << "vec4(" << v[0];
    for (u32 i = 1; i < 4; i++)
        os << ", " << v[i];
    os << ")";
    return os;
}
