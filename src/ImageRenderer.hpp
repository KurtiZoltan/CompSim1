#pragma once

#include "Utils.hpp"
#include "Math.hpp"
#include "Spacetime.hpp"
#include "Solver.hpp"

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
    Objects& m_objects;
    ReferenceFrame& m_referenceFrame;
    u32 m_width, m_height;
    f32 m_fov;

    void normalize(vec4& v);
    void projectOrthogonal(vec4& a, const vec4& b);
    void genInitialRays();
    void initFrame(const vec4& time, const vec4& lookAt, const vec4& up);
public:
    f32* m_rayBeginnings;
    ImageRenderer(Spacetime& spacetime, Objects& objects, ReferenceFrame& referenceFrame, u32 width, u32 height, f32 fov);
    ~ImageRenderer();
    void traceRays(const char* file);
};
