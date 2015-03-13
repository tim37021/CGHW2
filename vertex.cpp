#include "math.h"

using namespace SRenderer;

Vertex Vertex::Interpolate(Vertex endValue, float t)
{
	Vertex result;

	result.x=x*(1.0f-t)+endValue.x*t;
	result.y=y*(1.0f-t)+endValue.y*t;
	result.z=z*(1.0f-t)+endValue.z*t;

	return result;
}
