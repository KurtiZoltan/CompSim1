#include "Solver.hpp"
#include <cmath>

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

State integrate(const vec4& stratPosition, const vec4& startVelocity, Gamma f, const Objects& intersections)
{
	State intersection;
	
	
	return intersection;
}

//RK4
const u32 order = 4;
const f32 a[] = {0,   0,   0, 0,
                 0.5, 0,   0, 0,
                 0,   0.5, 0, 0,
                 0,   0,   1, 0};
const f32 c[] = {0, 0.5, 0.5, 1};
const f32 b[] = {1/6.0, 1/3.0, 1/3.0, 1/6.0};

const f32 relTol = 1e-6;
const f32 abstol = 1e-6;
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

void step(State (*f)(f32 t, const State& y), f32& t, State& y, f32 h)
{
    State yprime;
	State k[order];
    for (u32 i = 0; i < order; i++)
    {
        State dy;
		dy.clear();
        for (u32 j = 0; j < i; j++)
        {
            dy += a[i * order + j] * k[j];
        }
        k[i] = f(t + h * c[i], y + h * dy);
        yprime += b[i] * k[i];
    }
    y += h * yprime;
    t += h;
}

State fasteqsolver(State (*f)(f32 t, const State& y), f32 t0, const State& y0, f32 h0, f32 tfinal)
{
    f32 t = t0;
    f32 h = h0;
    State y = y0;
    
    State* k = new State[order];
    State dy;
    f32 t1, t2;
    State y1, y2;
    f32 error;
    while (t < tfinal)
    {
        t1 = t2 = t;
        y1 = y2 = y;
        step(f, t1, y1, h);
        step(f, t1, y1, h);
        step(f, t2, y2, 2*h);
        
        error = calcError(y1, y2);
		
        if (error < 1)
        {
            //test collisions
            t = t1;
            y = y1;
        }
        h = h / std::min(std::pow(error / errorGoal, 1/errorOrder), 0.5f);
    }
    delete[] k;
	return y;
}