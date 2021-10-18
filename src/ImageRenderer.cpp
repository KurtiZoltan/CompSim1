#include <cmath>
#include "ImageRenderer.hpp"
#include <iostream>

ImageRenderer::ImageRenderer(Spacetime& spacetime, ReferenceFrame& referenceFrame, u32 width, u32 height) :
    m_spacetime(spacetime), m_referenceFrame(referenceFrame), m_width(width), m_height(height)
{
    m_rayBeginnings = new f32[m_width * m_height * 8 * sizeof(f32)];
}

ImageRenderer::~ImageRenderer()
{
    delete[] m_rayBeginnings;
}

void ImageRenderer::genInitialRays()
{
    for (u32 y = 0; y < m_height; y++)
    {
        for (u32 x = 0; x < m_width; x++)
        {
            u32 index = (m_width * y + x);
            m_rayBeginnings[index * 8 + 0] = m_referenceFrame.position[0];
            m_rayBeginnings[index * 8 + 1] = m_referenceFrame.position[1];
            m_rayBeginnings[index * 8 + 2] = m_referenceFrame.position[2];
            m_rayBeginnings[index * 8 + 3] = m_referenceFrame.position[3];
            
            f32 up = static_cast<f32>(y) / (m_height - 1) * 2 - 1;
            f32 right = static_cast<f32>(x) / (m_width - 1) * 2 - 1;
            
            vec4 dir = (m_referenceFrame.forward + m_referenceFrame.up * up + m_referenceFrame.right * right) / sqrtf(1 + up * up + right * right) - m_referenceFrame.time;
            
            m_rayBeginnings[index * 8 + 4] = dir[0];
            m_rayBeginnings[index * 8 + 5] = dir[1];
            m_rayBeginnings[index * 8 + 6] = dir[2];
            m_rayBeginnings[index * 8 + 7] = dir[3];
        }
    }
}

void ImageRenderer::initFrame(const vec4& time, const vec4& lookAt, const vec4& up)
{
    m_referenceFrame.time = time;
    
    m_referenceFrame.forward = lookAt;
    projectOrthogonal(m_referenceFrame.forward, time);
    
    m_referenceFrame.up = up;
    projectOrthogonal(m_referenceFrame.up, m_referenceFrame.time);
    projectOrthogonal(m_referenceFrame.up, m_referenceFrame.forward);
    
    m_referenceFrame.right = up;
    projectOrthogonal(m_referenceFrame.right, m_referenceFrame.time);
    projectOrthogonal(m_referenceFrame.right, m_referenceFrame.forward);
    projectOrthogonal(m_referenceFrame.right, m_referenceFrame.up);
    
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
            switch (i)
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
    std::cout << "\n";
#endif
}

void ImageRenderer::normalize(vec4& v)
{
    v = v / sqrtf(abs(m_spacetime.g(m_referenceFrame.position, v, v)));
}

void ImageRenderer::projectOrthogonal(vec4& a, const vec4& b)
{
    a = a - b * m_spacetime.g(m_referenceFrame.position, a, b) / m_spacetime.g(m_referenceFrame.position, b, b);
}