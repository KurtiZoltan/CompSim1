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

int main()
{
    Schwarzschild spacetime(M);
    Objects objects;
    objects.add(eventHorizon, eventHorizonColor);
    objects.add(skySphere, skyColorNoCorrection);
    ReferenceFrame referenceFrame(vec4(0, 20 * 2 * M, 3.14/2, 0), vec4(1, 0, 0, 0), vec4(0, -1, 0, 0), vec4(0, 0, -1, 0));
	ImageRenderer renderer(spacetime, objects, referenceFrame, 1024, 1024, 3.141592/2);
    renderer.traceRays("../pic/test.tif");
}
