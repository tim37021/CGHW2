#include "shader.h"
#include <glm/glm.hpp>

namespace SRenderer
{
	VertexShaderOutput VertexShaderOutput::interpolate(const VertexShaderOutput &endValue, float t) const
	{
		VertexShaderOutput result;
		result.fragCoord=glm::mix(fragCoord, endValue.fragCoord, t);
		return result;
	};
}