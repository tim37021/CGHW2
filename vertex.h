#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <glm/glm.hpp>

namespace SRenderer
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 texCoord;
		glm::vec3 normal;

		Vertex()=default;
		Vertex(const glm::vec3 &pos);
		Vertex(const glm::vec3 &pos,
			const glm::vec2 &texCoord, 
			const glm::vec3 &normal);
		Vertex(const glm::vec3 &pos, const glm::vec3 &normal);
	};
}

#endif