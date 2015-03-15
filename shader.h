#ifndef _SHADER_H_
#define _SHADER_H_

#include "math.h"
#include <glm/vec4.hpp>

namespace SRenderer
{
	struct VertexShaderOutput: Interpolatable<VertexShaderOutput>
	{
		glm::vec4 fragCoord;
		virtual VertexShaderOutput interpolate(
			const VertexShaderOutput &endValue,
			float t) const;
	};

	typedef void (*VertexShader)(const Vertex &, VertexShaderOutput *);
	typedef void (*FragmentShader)(const VertexShaderOutput &, glm::vec4 *);
}
#endif