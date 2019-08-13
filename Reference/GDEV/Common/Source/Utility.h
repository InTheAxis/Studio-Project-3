#ifndef UTILITY_H
#define UTILITY_H

#include "Mtx44.h"
#include "Vertex.h"

Position operator*(const Mtx44& lhs, const Position& rhs);

//must overload float * const T 
template<typename T>
T Lerpf(T start, T end, float weight)
{
	return start + weight * (end - start);
}

#endif