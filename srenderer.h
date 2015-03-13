#ifndef _SRENDERER_H_
#define _SRENDERER_H_

#include "shader.h"
#include "mesh.h"

namespace SRenderer
{
	class SRenderer
	{
	public:
		SRenderer(int, int, VertexShader, FragmentShader);

		void setVertexShader(VertexShader);
		void setFragmentShader(FragmentShader);
		void setCanvasSize(int, int);

		void render(const Mesh &);
	private:
		VertexShader vs;
		FragmentShader fs;

		int width, height;
		float pixelWidth;

		void drawLine(const Vertex a, const Vertex &b);
		void drawTriangle(const Vertex &o, const Vertex a, const Vertex &b);
		void drawFilledTriangle(const Vertex &o, const Vertex a, const Vertex &b);
	};
}

#endif