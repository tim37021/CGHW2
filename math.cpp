#include "math.h"
#include <glm/glm.hpp>

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

		result.pos=glm::mix(pos, endValue.pos, t);
		result.normal=glm::mix(normal, endValue.normal, t);
		result.worldPos=glm::mix(worldPos, endValue.worldPos, t);

		return result;
	}

}