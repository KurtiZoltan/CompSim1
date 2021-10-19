#include <iostream>
#include "Schwarzschild.hpp"
#include "ImageRenderer.hpp"
#include "Math.hpp"

int main()
{
    Schwarzschild spacetime(1);
    ReferenceFrame referenceFrame(vec4(0, 10, 3.14/2, 0), vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0));
	ImageRenderer renderer(spacetime, referenceFrame, 1024, 1024);
    vec4 test;
    std::cout << test[3] << "\n";
    test[3] = 3;
    std::cout << test[3] << "\n";
    #ifndef NDEBUG
        std::cout << "DEBUG!!\n";
    #endif
    
    std::cout << referenceFrame.right << "\n";
}
