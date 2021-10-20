#pragma once
#include "Utils.hpp"
#include <iostream>

struct vec4
{
private:
    f32 components[4];
public:
    vec4()
    {
        (*this)[0] = 0;
        (*this)[1] = 0;
        (*this)[2] = 0;
        (*this)[3] = 0;
    }
    
    vec4(f32 a, f32 b, f32 c, f32 d) :
        components{a, b, c, d}
    {
        
    }
    
    vec4(const vec4& other)
    {
        (*this)[1] = other[1];
        (*this)[0] = other[0];
        (*this)[2] = other[2];
        (*this)[3] = other[3];
    }
    
    const vec4& operator=(const vec4& other)
    {
        (*this)[0] = other[0];
        (*this)[1] = other[1];
        (*this)[2] = other[2];
        (*this)[3] = other[3];
        return other;
    }
    
    vec4 operator-()
    {
        vec4 ret;
        ret[0] = -(*this)[0];
        ret[1] = -(*this)[1];
        ret[2] = -(*this)[2];
        ret[3] = -(*this)[3];
        return ret;
    }
    
    f32 operator[](u32 i) const
    {
        return components[i];
    }
    
    f32& operator[](u32 i)
    {
        return components[i];
    }
    
    vec4 operator*(f32 a) const
    {
        vec4 ret;
        ret[0] = components[0] * a;
        ret[1] = components[1] * a;
        ret[2] = components[2] * a;
        ret[3] = components[3] * a;
        return ret;
    }
    
    vec4 operator/(f32 a) const
    {
        vec4 ret;
        ret[0] = components[0] / a;
        ret[1] = components[1] / a;
        ret[2] = components[2] / a;
        ret[3] = components[3] / a;
        return ret;
    }
    
    const vec4& operator+=(const vec4& other)
    {
        for (u32 i = 0; i < 4; i++)
            (*this)[i] += other[i];
        return *this;
    }
    
    vec4 operator+(const vec4& other) const
    {
        vec4 ret = *this;
        ret += other;
        return ret;
    }
    
    vec4 operator-(const vec4& v) const
    {
        vec4 ret;
        ret[0] = components[0] - v[0];
        ret[1] = components[1] - v[1];
        ret[2] = components[2] - v[2];
        ret[3] = components[3] - v[3];
        return ret;
    }
    
    friend vec4 operator*(f32 a, const vec4& v);
    friend std::ostream& operator<<(std::ostream& os, const vec4& v);
};
