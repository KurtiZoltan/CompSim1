#include <iostream>
#include "Schwarzschild.hpp"
#include "ImageRenderer.hpp"
#include "Math.hpp"

int main()
{
    Schwarzschild spacetime(1);
    ReferenceFrame referenceFrame(vec4(0, 10, 3.14/2, 0), vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0));
	ImageRenderer renderer(spacetime, referenceFrame, 1024, 1024);
}
