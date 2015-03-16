#ifndef _SRENDER_MATH_H_
#define _SRENDER_MATH_H_

#include <glm/vec3.hpp>
#include "cbase.h"

namespace SRenderer
{
	class Interpolatable
	{
		// The function calculates an interpolated value
		// along the fraction t between 0.0 and 1.0. 
		// When t = 1.0, endVal is returned.
		virtual void interpolate(const CBase &endValue, float t, CBase *out) const =0;
	};
}

#endif