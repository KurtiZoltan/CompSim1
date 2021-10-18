#pragma once
#include "Utils.hpp"
#include "Math.hpp"

class Spacetime
{
    
public:
    virtual f32 g(const vec4& position, const vec4& a, const vec4& b) const = 0;
    virtual vec4 Gammauu(const vec4& position, const vec4& u) const = 0;
};