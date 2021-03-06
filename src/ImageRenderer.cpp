#include "ImageRenderer.hpp"
#include "tiffio.h"
#include <iostream>
#include <cmath>

ReferenceFrame::ReferenceFrame(const vec4& position, const vec4& velocity, const vec4& lookAt, const vec4& up) :
    position(position), time(velocity), up(up), lookAt(lookAt)
{
    
}

ImageRenderer::ImageRenderer(Spacetime& spacetime, Objects& objects, ReferenceFrame& referenceFrame, u32 width, u32 height, f32 fov) :
    m_spacetime(spacetime), m_objects(objects), m_referenceFrame(referenceFrame), m_width(width), m_height(height), m_fov(fov)
{
    m_rayBeginnings = new f32[m_width * m_height * 8 * sizeof(f32)];
    initFrame(m_referenceFrame.time, m_referenceFrame.lookAt, m_referenceFrame.up);
}

ImageRenderer::~ImageRenderer()
{
    delete[] m_rayBeginnings;
}

void ImageRenderer::traceRays(const char* file)
{
    genInitialRays();
    u8* image  = new u8[m_width * m_height * 3];
    #pragma omp parallel for
    for (u32 y = 0; y < m_height; y++)
    {
        #pragma omp simd
        for (u32 x = 0; x < m_width; x++)
        {
            u32 index = m_width * y + x;
            vec4 position;
            position[0] = m_rayBeginnings[index * 8 + 0];
            position[1] = m_rayBeginnings[index * 8 + 1];
            position[2] = m_rayBeginnings[index * 8 + 2];
            position[3] = m_rayBeginnings[index * 8 + 3];
            vec4 velocity;
            velocity[0] = m_rayBeginnings[index * 8 + 4];
            velocity[1] = m_rayBeginnings[index * 8 + 5];
            velocity[2] = m_rayBeginnings[index * 8 + 6];
            velocity[3] = m_rayBeginnings[index * 8 + 7];
            
            RGB color = trace(position, velocity, m_spacetime, m_objects);
            image[index * 3 + 0] = color.r;
            image[index * 3 + 1] = color.g;
            image[index * 3 + 2] = color.b;
        }
    }
    std::cout << "Writing " << file << ".\n";
    TIFF* tif = TIFFOpen(file, "w");
    if (!tif)
    {
        std::cout << "TIFF could not be written, try creating the ../pic folder.\n";
        throw 1;
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, m_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, m_height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_BOTLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
    for (u32 i = 0; i < m_height; i++)
    {
        TIFFWriteScanline(tif, image + i * (3 * m_width), i, 0);
    }
    TIFFClose(tif);
    delete[] image;
}

void ImageRenderer::genInitialRays()
{
    f32 heightScale = std::tan(m_fov / 2) * m_height / m_width;
    f32 widthScale = std::tan(m_fov / 2);
    for (u32 y = 0; y < m_height; y++)
    {
        for (u32 x = 0; x < m_width; x++)
        {
            u32 index = m_width * y + x;
            m_rayBeginnings[index * 8 + 0] = m_referenceFrame.position[0];
            m_rayBeginnings[index * 8 + 1] = m_referenceFrame.position[1];
            m_rayBeginnings[index * 8 + 2] = m_referenceFrame.position[2];
            m_rayBeginnings[index * 8 + 3] = m_referenceFrame.position[3];
            
            f32 up = (static_cast<f32>(y) / (m_height - 1) * 2 - 1) * heightScale;
            f32 right = (static_cast<f32>(x) / (m_width - 1) * 2 - 1) * widthScale;
            
            vec4 dir = (m_referenceFrame.forward + m_referenceFrame.up * up + m_referenceFrame.right * right) / std::sqrt(1 + up * up + right * right) - m_referenceFrame.time;
            
            m_rayBeginnings[index * 8 + 4] = dir[0];
            m_rayBeginnings[index * 8 + 5] = dir[1];
            m_rayBeginnings[index * 8 + 6] = dir[2];
            m_rayBeginnings[index * 8 + 7] = dir[3];
        }
    }
}

f32 det(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3)
{
    vec4 v[4];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    v[3] = v3;
    f32 swapfactor = 1;
    for (u32 i = 0; i < 4; i++)
    {
        f32 norm = 0;
        for (u32 j = 0; j < 4; j++)
        {
            norm += v[i][j] * v[i][j];
        }
        norm = std::sqrt(norm);
        if (norm < EPSILON)
            return 0;
        if (std::abs(v[i][i]) / norm < EPSILON)
        {
            f32 maxNormRatio = -1;
            u32 maxIndex = 0;
            for (u32 j = i + 1; j < 4; j++)
            {
                f32 normRatio = 0;
                for (u32 k = 0; k < 4; k++)
                {
                    normRatio += v[j][k] * v[j][k];
                }
                normRatio = std::sqrt(normRatio);
                normRatio = std::abs(v[j][i]) / normRatio;
                if (normRatio > maxNormRatio)
                {
                    maxNormRatio = normRatio;
                    maxIndex = j;
                }
            }
            if (maxNormRatio > EPSILON)
            {
                swapfactor = -swapfactor;
                vec4 temp = v[i];
                v[i] = v[maxIndex];
                v[maxIndex] = temp;
            }
            else
            {
                return 0;
            }
        }
        for (u32 j = i + 1; j < 4; j++)
        {
            v[j] = v[j] - v[j][i] / v[i][i] * v[i];
        }
    }
    return swapfactor * v[0][0] * v[1][1] * v[2][2] * v[3][3];
}

void ImageRenderer::initFrame(const vec4& time, const vec4& lookAt, const vec4& up)
{
    if (m_spacetime.g(m_referenceFrame.position, time, time) < 0)
    {
        std::cout << "Time vector is spacelike. Stopping.\n";
        throw 1;
    }
    
    m_referenceFrame.time = time;
    
    m_referenceFrame.forward = lookAt;
    projectOrthogonal(m_referenceFrame.forward, time);
    
    m_referenceFrame.up = up;
    projectOrthogonal(m_referenceFrame.up, m_referenceFrame.time);
    projectOrthogonal(m_referenceFrame.up, m_referenceFrame.forward);
    
    vec4 sideDir;
    f32 maxNorm = 0;
    for (u32 i = 0; i < 4; i++)
    {
        vec4 guess;
        guess[i] = 1;
        projectOrthogonal(guess, m_referenceFrame.time);
        projectOrthogonal(guess, m_referenceFrame.forward);
        projectOrthogonal(guess, m_referenceFrame.up);
        f32 norm = fabsf(m_spacetime.g(m_referenceFrame.position, guess, guess));
        if (norm > maxNorm)
        {
            maxNorm = norm;
            sideDir = guess;
        }
    }
    
    if (0 < det(m_referenceFrame.time, m_referenceFrame.forward, m_referenceFrame.up, sideDir))
    {
        m_referenceFrame.right = sideDir;
    }
    else
    {
        m_referenceFrame.right = -sideDir;
    }
    
    normalize(m_referenceFrame.time);
    normalize(m_referenceFrame.forward);
    normalize(m_referenceFrame.up);
    normalize(m_referenceFrame.right);
#ifndef NDEBUG
    for (u32 i = 0; i < 4; i++)
    {
        vec4 a;
        switch (i)
        {
            case 0:
                a = m_referenceFrame.time;
                break;
            case 1:
                a = m_referenceFrame.forward;
                break;
            case 2:
                a = m_referenceFrame.up;
                break;
            case 3:
                a = m_referenceFrame.right;
                break;
        }
        for (u32 j = 0; j < 4; j++)
        {
            vec4 b;
            switch (j)
            {
                case 0:
                    b = m_referenceFrame.time;
                    break;
                case 1:
                    b = m_referenceFrame.forward;
                    break;
                case 2:
                    b = m_referenceFrame.up;
                    break;
                case 3:
                    b = m_referenceFrame.right;
                    break;
            }
            std::cout << m_spacetime.g(m_referenceFrame.position, a, b) << " ";
        }
        std::cout << "\n";
    }
#endif
}

void ImageRenderer::normalize(vec4& v)
{
    v = v / std::sqrt(std::abs(m_spacetime.g(m_referenceFrame.position, v, v)));
}

void ImageRenderer::projectOrthogonal(vec4& a, const vec4& b)
{
    a = a - m_spacetime.g(m_referenceFrame.position, a, b) / m_spacetime.g(m_referenceFrame.position, b, b) * b;
}
