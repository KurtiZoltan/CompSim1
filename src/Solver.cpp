#include "Solver.hpp"
#include <cmath>
#include <fstream>

f32& State::operator[](u32 i)
{
	if (i < 4)
	{
		return position[i];
	}
	else
	{
		return velocity[i - 4];
	}
}

f32 State::operator[](u32 i) const
{
	if (i < 4)
	{
		return position[i];
	}
	else
	{
		return velocity[i - 4];
	}
}

void State::clear()
{
	for (u32 i = 0; i < 8; i++)
		(*this)[i] = 0;
}

State State::operator*(f32 a) const
{
	State ret;
	ret.position = this->position * a;
	ret.velocity = this->velocity * a;
	return ret;
}

State operator*(f32 a,const State& state)
{
	return state * a;
}

State State::operator+(const State& other) const
{
	State ret = *this;
	ret += other;
	return ret;
}

const State& State::operator+=(const State& other)
{
	this->position += other.position;
	this->velocity += other.velocity;
	return *this;
}

//RK4
const u32 order = 4;
const f32 a[] = {0,   0,   0, 0,
                 0.5, 0,   0, 0,
                 0,   0.5, 0, 0,
                 0,   0,   1, 0};
const f32 c[] = {0, 0.5, 0.5, 1};
const f32 b[] = {1/6.0, 1/3.0, 1/3.0, 1/6.0};

#ifndef PRINT
const f32 relTol = 1e-6;
const f32 abstol = 1e-6;
#else
const f32 relTol = 5e-7;
const f32 abstol = 5e-7;
#endif
const f32 errorGoal = 0.25;
const f32 errorOrder = 5;

f32 calcError(const State& y1, const State& y2)
{
    f32 max = 0;
    for (u32 i = 0; i < 8; i++)
    {
        f32 tol = std::max(abstol, relTol * std::abs(y1[i]));
        float diff = std::abs(y1[i] - y2[i]);
        max = std::max(max, diff / tol);
    }
    return max;
}

void step(const Spacetime& spacetime, f32& t, State& y, f32 h)
{
    State yprime;
	State k[order];
    for (u32 i = 0; i < order; i++)
    {
        State dy;
		dy.clear();
        #pragma omp simd
        for (u32 j = 0; j < i; j++)
        {
            dy += a[i * order + j] * k[j];
        }
        State temp = y + h * dy;
        k[i].position = temp.velocity;
        k[i].velocity = spacetime.Gammauu(temp.position, temp.velocity);
        yprime += b[i] * k[i];
    }
    y += h * yprime;
    t += h;
}

RGB eqsolver(const Spacetime& spacetime, f32 t0, const State& y0, f32 h0, f32 tfinal, const Objects& objects)
{
    f32 t = t0;
    f32 h = h0;
    State y = y0;
    for (u32 i = 0; i < objects.numIntersects; i++)
    {
        objects.testValues[i] = objects.intersects[i](y.position);
    }
    
    #ifdef PRINT
    u32 goodstep = 0, badstep = 0;
    std::ofstream file;
    const char* txtname = "../python/orbit.txt";
    file.open(txtname, std::ios::out);
    if (!file.is_open())
    {
        std::cout << "File " << txtname << " couldn't be opened.\n";
        throw 1;
    }
    #endif
    
    while (t < tfinal)
    {
        f32 t1 = t, t2 = t;
        State y1 =y, y2 = y;
        step(spacetime, t1, y1, h);
        step(spacetime, t1, y1, h);
        step(spacetime, t2, y2, 2*h);
        
        f32 error = calcError(y1, y2);
		
        if (error < 1)
        {
            for (u32 i = 0; i < objects.numIntersects; i++)
            {
                f32 test = objects.intersects[i](y.position);
                if (test * objects.testValues[i] <= 0)
                {
                    #ifndef NDEBUG
                    std::cout << "Intersect with object " << i << "!\n";
                    #endif
                    #ifdef PRINT
                    file.close();
                    #endif
                    return objects.colors[i](y);
                }
                objects.testValues[i] = test;
            }
            t = t1;
            y = y1;
            #ifdef PRINT
            file << t << " " << y.position[0] << " " << y.position[1] << " " << y.position[2] << " " << y.position[3] << " " << y.velocity[0] << " " << y.velocity[1] << " " << y.velocity[2] << " " << y.velocity[3] << "\n";
            goodstep++;
            #endif
        }
        #ifdef PRINT
        else
        {
            badstep++;
        }
        #endif
        h = h / std::max(std::pow(error / errorGoal, 1/errorOrder), 0.5f);
    }
    #ifdef PRINT
    file.close();
    std::cout << "Good steps: " << goodstep << "\nBad steps: " << badstep << "\n";
    #endif
    RGB ret;
    ret.r = 255;
    ret.g = 0;
    ret.b = 255;
	return ret;
}

RGB trace(const vec4& stratPosition, const vec4& startVelocity,  const Spacetime& spacetime, const Objects& objects)
{
    State start;
    start.position = stratPosition;
    start.velocity = startVelocity;
	return eqsolver(spacetime, 0, start, 1, 3000, objects);
}

Objects::Objects() : 
    intersects(nullptr), colors(nullptr), testValues(nullptr), numIntersects(0)
{
    
}

Objects::~Objects()
{
    delete[] intersects;
    delete[] testValues;
}

void Objects::add(Intersect intersect, Color color)
{
    Intersect* newIntersects = new Intersect[numIntersects + 1];
    Color* newColors = new Color[numIntersects + 1];
    f32* newTestalues = new f32[numIntersects + 1];
    for (u32 i = 0; i < numIntersects; i++)
    {
        newIntersects[i] = intersects[i];
        newColors[i] = colors[i];
        newTestalues[i] = testValues[i];
    }
    newIntersects[numIntersects] = intersect;
    newColors[numIntersects] = color;
    newTestalues[numIntersects] = 1;
    numIntersects++;
    delete[] intersects;
    delete[] colors;
    delete[] testValues;
    intersects = newIntersects;
    colors = newColors;
    testValues = newTestalues;
}
