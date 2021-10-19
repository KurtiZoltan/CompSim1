#pragma once
#include "Spacetime.hpp"
#include "Utils.hpp"
#include "Math.hpp"

struct ReferenceFrame
{
    ReferenceFrame(const vec4& position, const vec4& velocity, const vec4& lookAt, const vec4& up);
    vec4 position;
    vec4 time;
    vec4 forward;
    vec4 up;
    vec4 right;
    vec4 lookAt;
};

class ImageRenderer
{
    Spacetime& m_spacetime;
    ReferenceFrame& m_referenceFrame;
    f32 m_fov;
    u32 m_width, m_height;
    f32* m_rayBeginnings;
    void normalize(vec4& v);
    void projectOrthogonal(vec4& a, const vec4& b);
public:
    ImageRenderer(Spacetime& spacetime, ReferenceFrame& referenceFrame, u32 width, u32 height);
    ~ImageRenderer();
    void genInitialRays();
    void initFrame(const vec4& time, const vec4& lookAt, const vec4& up);
};