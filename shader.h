#ifndef _SHADER_H_
#define _SHADER_H_

#include "math.h"

namespace SRenderer
{
	typedef void (*VertexShader)(const Vertex &, Interpolatable<Vertex> *);
	typedef void (*FragmentShader)(const Interpolatable<Vertex> &, float &r, float &g, float &b);
}
#endif