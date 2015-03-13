#ifndef _VERTEX_H_
#define _VERTEX_H_

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
		float x, y, z;
		float r, g, b;

		Vertex()=default;
		Vertex(float, float, float);

		virtual Vertex interpolate(const Vertex &endValue, float t) const;
	};
}

#endif