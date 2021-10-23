#include "Schwarzschild.hpp"
#include "ImageRenderer.hpp"
#include "tiffio.h"
#include <iostream>
#include <cmath>

f32 M = 1; //mass of the black hole
f32 Rs = 2 * M; //Schwarzschild radius
u32 N = 20; //checkerboard resolution

char* starfield;
u32 starfieldWidth, starfieldHeight;

void loadImage()
{
    const char* file = "../src/eso0932a.tif";
    std::cout << "Reading " << file << ".\n";
    TIFF* tif = TIFFOpen(file, "r");
    if (!tif)
    {
        std::cout << file << "couldn't be opened.";
        throw 1;
    }
    
    u32 w, h;
	size_t npixels;
	u8* raster;

	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    starfieldWidth = w;
    starfieldHeight = h;
	npixels = w * h;
    starfield = new char[npixels * 3];
	raster = (u8*) _TIFFmalloc(npixels * sizeof (uint32));
	if (raster != NULL) {
	    if (TIFFReadRGBAImage(tif, w, h, reinterpret_cast<u32*>(raster), 0)) {
            for (u32 i = 0; i < npixels; i++)
            {
                starfield[i * 3 + 0] = raster[4 * i + 0];
                starfield[i * 3 + 1] = raster[4 * i + 1];
                starfield[i * 3 + 2] = raster[4 * i + 2];
            }
	    }
	    _TIFFfree(raster);
	}
    
    TIFFClose(tif);
    std::cout << "Image loded.\n";
}

f32 eventHorizon(const vec4& position)
{
    return position[1] - (1 + 1e-3) * Rs;
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

RGB black(const State& state)
{
    return RGB();
}

f32 skySphere(const vec4& position)
{
    
    return Rs * 20 - position[1];
}

RGB skyColorNoCorrection(const State& state)
{
    f32 theta = state.position[2];
    f32 phi = state.position[3];
    f32 scale = N / PI;
    
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

RGB fancySky(const State& state)
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
    
    u32 index = (u32)(theta / PI * starfieldHeight) * starfieldWidth + (u32)((phi / 2 / PI + 0.5) * starfieldWidth);
    RGB ret;
    ret.r = starfield[3 * index + 0];
    ret.g = starfield[3 * index + 1];
    ret.b = starfield[3 * index + 2];
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
    
    f32 scale = N / PI;
    
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
    loadImage();
    
    Schwarzschild spacetime(M);
    Objects objects;
    objects.add(eventHorizon, black);
    objects.add(skySphere, fancySky);
    vec4 position = vec4(0, 10 * Rs, PI/2, 0);
    vec4 velocity = vec4(1, 0, 0, 0);
    vec4 forward  = vec4(0, -1, 0, 0);
    vec4 up       = vec4(0, 0, -1, 0);
    ReferenceFrame referenceFrame(position, velocity, forward, up);
	ImageRenderer renderer(spacetime, objects, referenceFrame, 1024, 1024, 90 * PI / 180);
    renderer.traceRays("../pic/test.tif");
    
    delete[] starfield;
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
