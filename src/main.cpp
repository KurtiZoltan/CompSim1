#include "Schwarzschild.hpp"
#include "ImageRenderer.hpp"
#include <iostream>
#include <cmath>

f32 M = 1;

f32 eventHorizon(const vec4& position)
{
    return position[1] - (2 + 1e-2) * M;
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
    
    return 2 * M * 100 - position[1];
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

int main()
{
    Schwarzschild spacetime(M);
    Objects objects;
    objects.add(eventHorizon, eventHorizonColor);
    objects.add(skySphere, skyColor);
    ReferenceFrame referenceFrame(vec4(0, 40 * 2 * M, PI/2, 0), vec4(1, 0, 0, 0), vec4(0, -1, 0, 0), vec4(0, 0, -1, 0));
	ImageRenderer renderer(spacetime, objects, referenceFrame, 1024, 1024, 70 * PI / 180);
    renderer.traceRays("../pic/test.tif");
}
