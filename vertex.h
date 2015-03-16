#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <glm/vec3.hpp>

namespace SRenderer
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;

		Vertex()=default;
		Vertex(const glm::vec3 &pos);
		Vertex(const glm::vec3 &pos, const glm::vec3 &normal);
	};
}

#endif