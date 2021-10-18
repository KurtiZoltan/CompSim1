#include "Schwarzschild.hpp"
#include <cmath>

Schwarzschild::Schwarzschild(f32 M) :
    m_M(M)
{
    
}

/*
position: t r theta phi
*/
f32 Schwarzschild::g(const vec4& position, const vec4& a, const vec4& b) const
{
    f32 g00 = 1 - 2 * m_M / position[1];
    f32 g11 = -1 / g00;
    f32 g22 = -position[1] * position[1];
    f32 sintheta = sinf(position[2]);
    f32 g33 = g22 * sintheta * sintheta;
    return g00 * a[0] * b[0] + g11 * a[1] * b[1] + g22 * a[2] * b[2] + g33 * a[3] * b[3];
}

vec4 Schwarzschild::Gammauu(const vec4& position, const vec4& v) const
{
    f32 Rs = 2 * m_M;
    f32 r = position[1], theta = position[2];
    f32 v0 = v[0], v1 = v[1], v2 = v[2], v3 = v[3];
    f32 sintheta = sinf(theta);
    f32 costheta = cosf(theta);
    f32 G001 = Rs / (2 * r * (r - Rs));
    f32 G100 = Rs * (r - Rs) / (2 * r * r * r);
    f32 G111 = -G001;
    f32 G122 = Rs - r;
    f32 G133 = G122*sintheta*sintheta;
    f32 G212 = 1/r;
    f32 G233 = -sintheta*costheta;
    f32 G313 = G212;
    f32 G323 = costheta/sintheta;
    vec4 ret;
    ret[0] = -2*G001*v0*v1;
    ret[1] = -G100*v0*v0-G111*v1*v1-G122*v2*v2-G133*v3*v3;
    ret[2] = -2*G212*v1*v2-G233*v3*v3;
    ret[3] = -2*G313*v1*v3-2*G323*v2*v3;
    return ret;
}