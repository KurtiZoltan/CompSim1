#include <iostream>
#include "Schwarzschild.hpp"
#include "ImageRenderer.hpp"

f32 M = 1;

f32 eventHorizon(const vec4& position)
{
    return position[1] - (2 + 1e-2) * M;
}

RGB eventHorizonColor(const State& state)
{
    RGB ret;
    ret.r = 0;
    ret.g = 0;
    ret.b = 0;
    return ret;
}

f32 skySphere(const vec4& position)
{
    return 2 * M * 10 - position[1];
}

RGB skyColor(const State& state)
{
    RGB ret;
    ret.r = 0;
    ret.g = 0;
    ret.b = 255;
    return ret;
}

int main()
{
    Schwarzschild spacetime(M);
    Objects objects;
    objects.add(eventHorizon, eventHorizonColor);
    objects.add(skySphere, skyColor);
    ReferenceFrame referenceFrame(vec4(0, 3.5 * 2 * M, 3.14/2, 0), vec4(1, 0, 0, 0), vec4(0, -1, -0.1, 0), vec4(0, 0, -1, 0));
	ImageRenderer renderer(spacetime, objects, referenceFrame, 1024, 1024, 3.141592/2);
}
