#pragma once

#include "Utils.hpp"
#include "Math.hpp"
#include "Spacetime.hpp"

struct State
{
	vec4 position;
	vec4 velocity;
	
	f32& operator[](u32 i);
	f32 operator[](u32 i) const;
	void clear();
	State operator*(f32 a) const;
	State operator+(const State& other) const;
	const State& operator+=(const State& other);
	friend State operator*(f32 a,const State& state);
};

typedef vec4 (*Gamma)(const vec4& position, const vec4& velocity);
typedef f32(*Intersect)(const vec4& position);

struct Objects
{
	Intersect* intersects;
	f32* testValues;
	u32 numIntersects;

	Objects();
	~Objects();
	void add(Intersect intersect);
};

State integrate(const vec4& stratPosition, const vec4& startVelocity, const Spacetime& spacetime, const Objects& objects);
