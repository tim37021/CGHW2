#ifndef _SHADER_H_
#define _SHADER_H_

#include "math.h"
#include "vertex.h"
#include <glm/vec4.hpp>

namespace SRenderer
{
	struct VertexShaderOutput: public CBase, public Interpolatable
	{
		//no need for virtual destructor..
		glm::vec4 fragCoord;
		virtual void interpolate(
			const CBase &endValue,
			float t,
			CBase *) const;
		virtual CBase *clone() const;
	};

	typedef VertexShaderOutput *(*VertexShader)(const Vertex &);
	typedef void (*FragmentShader)(const VertexShaderOutput &, glm::vec4 *);
}
#endif