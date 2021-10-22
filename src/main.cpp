#include "Schwarzschild.hpp"
#include "ImageRenderer.hpp"
#include <iostream>
#include <cmath>

f32 M = 1;
f32 Rs = 2 * M;

f32 eventHorizon(const vec4& position)
{
    return position[1] - (1 + 1e-2) * Rs;
}

RGB eventHorizonColor(const State& state)
{
    f32 modTheta = state.position[2] / PI * 6;
    f32 modPhi = state.position[3] / PI * 6;
    bool n1 = (modTheta - std::floor(modTheta)) < 0.5;
    bool n2 = (modPhi - std::floor(modPhi)) < 0.5;
    RGB ret;
    if (n1 ^ n2)
    {
        ret.r = 0;
        ret.g = 255;
        ret.b = 0;
    }
    else
    {
        ret.r = 0;
        ret.g = 127;
        ret.b = 0;
    }
    return ret;
}

f32 skySphere(const vec4& position)
{
    
    return Rs * 100 - position[1];
}

RGB skyColorNoCorrection(const State& state)
{
    f32 modTheta = state.position[2] / PI * 6;
    f32 modPhi = state.position[3] / PI * 6;
    bool n1 = (modTheta - std::floor(modTheta)) < 0.5;
    bool n2 = (modPhi - std::floor(modPhi)) < 0.5;
    RGB ret;
    if (n1 ^ n2)
    {
        ret.r = 0;
        ret.g = 0;
        ret.b = 255;
    }
    else
    {
        ret.r = 0;
        ret.g = 0;
        ret.b = 127;
    }
    return ret;
}

RGB skyColor(const State& state)
{
    f32 r = state.position[1];
    f32 ctheta = std::cos(state.position[2]);
    f32 stheta = std::sin(state.position[2]);
    f32 sphi = std::sin(state.position[3]);
    f32 cphi = std::cos(state.position[3]);
    f32 dirx = state.velocity[1] / std::sqrt(1 - 2 * M / r) * stheta * cphi + state.velocity[2] * r * ctheta * cphi - state.velocity[3] * r * stheta * sphi;
    f32 diry = state.velocity[1] / std::sqrt(1 - 2 * M / r) * stheta * sphi + state.velocity[2] * r * ctheta * sphi + state.velocity[3] * r * stheta * cphi;
    f32 dirz = state.velocity[1] / std::sqrt(1 - 2 * M / r) * ctheta - state.velocity[2] * r * stheta;
    f32 theta = std::atan2(std::sqrt(dirx * dirx + diry * diry), dirz);
    f32 phi = std::atan2(diry, dirx);
    
    f32 scale = 20 / PI;
    
    bool n1 = (theta * scale - std::floor(theta * scale)) < 0.5;
    bool n2 = (phi * scale - std::floor(phi * scale)) < 0.5;
    RGB ret;
    if (n1 ^ n2)
    {
        ret.r = 0;
        ret.g = 0;
        ret.b = 255;
    }
    else
    {
        ret.r = 0;
        ret.g = 0;
        ret.b = 127;
    }
    return ret;
}

RGB redshift(const State& state)
{
    f32 fo = -state.velocity[0] * std::sqrt(1 - 2 * M / state.position[1]);
    f32 b = 1 / fo;
    RGB ret;
    ret.r = std::max(std::min((int)(255 * (1 - b / 5)), 255), 0);
    ret.g = 0;
    ret.b = std::max(std::min((int)(255 * (0 + b / 5)), 255), 0);
    return ret;
}
#ifndef PRINT //disable PRINT in Utils.hpp to enable this code part
int main()
{
    Schwarzschild spacetime(M);
    Objects objects;
    objects.add(eventHorizon, redshift);
    objects.add(skySphere, redshift);
    ReferenceFrame referenceFrame(vec4(0, 1.5 * Rs, PI/2, 0), vec4(1, 0, 0, 0.15), vec4(0, 0, 1, 0), vec4(0, 1, 0, 0));
	ImageRenderer renderer(spacetime, objects, referenceFrame, 1024, 1024, 90 * PI / 180);
    renderer.traceRays("../pic/sideRedhift.tif");
}
#else //!PRINT
int main()
{
    Schwarzschild spacetime(M);
    Objects objects;
    
    vec4 position(0, 100, PI/2, 0);
    vec4 velocity(1, 0, 0, 0.075 / 100);
    velocity = velocity / std::sqrt(spacetime.g(position, velocity, velocity));
    trace(position, velocity, spacetime, objects);
}
#endif //!PRINT
