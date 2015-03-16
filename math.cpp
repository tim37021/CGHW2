#include "vertex.h"
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

}