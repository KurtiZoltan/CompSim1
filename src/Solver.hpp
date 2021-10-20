#pragma once

#include "Utils.hpp"
#include "Math.hpp"
#include "Spacetime.hpp"

struct Intersection
{
	vec4 position;
	vec4 velocity;
};

typedef vec4 (*Gamma)(const vec4& position, const vec4& velocity);
typedef f32(*Object)(const vec4& position);

struct Objects
{
	Object* objects;
	f32* testValues;
	u32 numIntersects;

	Objects() = delete;
};

Intersection integrate(const vec4& stratPosition, const vec4& startVelocity, Gamma f, const Objects& intersections);