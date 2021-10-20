#include <iostream>
#include "Schwarzschild.hpp"
#include "ImageRenderer.hpp"

int main()
{
    Schwarzschild spacetime(1);
    Objects objects;
    ReferenceFrame referenceFrame(vec4(0, 4, 3.14/2, 0), vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0));
	ImageRenderer renderer(spacetime, objects, referenceFrame, 1024, 1024, 3.141592/2);
}
