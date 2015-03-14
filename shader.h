#ifndef _SHADER_H_
#define _SHADER_H_

#include "math.h"
#include <glm/vec4.hpp>

namespace SRenderer
{
	typedef void (*VertexShader)(const Vertex &, Interpolatable<Vertex> *);
	typedef void (*FragmentShader)(const Interpolatable<Vertex> &, glm::vec4 *);
}
#endif