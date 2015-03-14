#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <glm/vec3.hpp>

namespace SRenderer
{
	template <class T>
	class Interpolatable
	{
		// The function calculates an interpolated value
		// along the fraction t between 0.0 and 1.0. 
		// When t = 1.0, endVal is returned.
		virtual T interpolate(const T &endValue, float t) const =0;
	};

	struct Vertex: public Interpolatable<Vertex>
	{
		glm::vec3 pos;
		glm::vec3 normal;

		Vertex()=default;
		Vertex(const glm::vec3 &pos);
		Vertex(const glm::vec3 &pos, const glm::vec3 &normal);

		virtual Vertex interpolate(const Vertex &endValue, float t) const;
	};
}

#endif