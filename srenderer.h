#ifndef _SRENDERER_H_
#define _SRENDERER_H_

#include "shader.h"
#include "mesh.h"
#include "framebuffer.h"

namespace SRenderer
{
	class SRenderer
	{
	public:
		SRenderer(FrameBuffer *, VertexShader, FragmentShader);

		void setVertexShader(VertexShader);
		void setFragmentShader(FragmentShader);

		void render(const Mesh &);
	private:
		VertexShader vs;
		FragmentShader fs;
		FrameBuffer *fbo;
		
		void drawLine(const VertexShaderOutput &a, const VertexShaderOutput &b);
		void drawTriangle(
			const VertexShaderOutput &o, 
			const VertexShaderOutput &a, 
			const VertexShaderOutput &b
		);
		void drawFilledTriangle(
			const VertexShaderOutput &o,
			const VertexShaderOutput &a, 
			const VertexShaderOutput &b
		);
	};
}

#endif