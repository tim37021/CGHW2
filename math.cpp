#include "math.h"

namespace SRenderer{

	Vertex::Vertex(float x_, float y_, float z_):
		x(x_), y(y_), z(z_)
	{

	}


	Vertex Vertex::interpolate(const Vertex &endValue, float t) const
	{
		Vertex result;

		result.x=x*(1.0f-t)+endValue.x*t;
		result.y=y*(1.0f-t)+endValue.y*t;
		result.z=z*(1.0f-t)+endValue.z*t;

		return result;
	}

}