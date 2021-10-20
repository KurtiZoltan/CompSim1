#pragma once
#include "Utils.hpp"
#include "Math.hpp"
#include "Spacetime.hpp"

class Schwarzschild : public Spacetime
{
private:
    f32 m_M;
public:
    Schwarzschild(f32 M);
    f32 g(const vec4& position, const vec4& a, const vec4& b) const;
    vec4 Gammauu(const vec4& position, const vec4& u) const;
};
