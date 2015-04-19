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

	class ShaderProgram
	{
	public:
		virtual VertexShaderOutput *callVertexShader(const Vertex &v)
		{
			VertexShaderOutput *vout = new VertexShaderOutput;
			vout->fragCoord=glm::vec4(v.pos, 1.0f);
			return vout;
		}
		virtual void callFragmentShader(const VertexShaderOutput &vso, glm::vec4 *out)
		{ *out=glm::vec4(1.0f); }
	};
}
#endif