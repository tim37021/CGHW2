#include "math.h"

namespace SRenderer{

	Vertex::Vertex(const glm::vec3 &pos_):
		pos(pos_)
	{

	}

	Vertex::Vertex(const glm::vec3 &pos_, const glm::vec3 &normal_):
		pos(pos_), normal(normal_)
	{

	}


	Vertex Vertex::interpolate(const Vertex &endValue, float t) const
	{
		Vertex result;

		result.pos=pos*(1.0f-t)+endValue.pos*t;
		result.normal=normal*(1.0f-t)+endValue.normal*t;
		result.worldPos=worldPos*(1.0f-t)+endValue.worldPos*t;

		return result;
	}

}