#include "shader.h"
#include <glm/glm.hpp>

namespace SRenderer
{
	void VertexShaderOutput::interpolate(const CBase &endValue, float t, CBase *out) const
	{
		VertexShaderOutput *result=dynamic_cast<VertexShaderOutput *>(out);
		const VertexShaderOutput &endValue2=dynamic_cast<const VertexShaderOutput &>(endValue);
		result->fragCoord=glm::mix(fragCoord, endValue2.fragCoord, t);
	};

	CBase *VertexShaderOutput::clone() const
	{
		VertexShaderOutput *result = new VertexShaderOutput();
		*result = *this;
		return result;
	}
}