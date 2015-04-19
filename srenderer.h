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
		SRenderer(FrameBuffer *, ShaderProgram *);

		void setFrameBuffer(FrameBuffer *);
		void enableBackfaceCulling(bool value)
		{ m_enableBackfaceCulling=value; }

		void render(const Mesh &);
	private:
		ShaderProgram *sp;
		FrameBuffer *fbo;
		VertexShaderOutput *outputSlot1, *outputSlot2, *outputSlot3;
		bool m_enableBackfaceCulling;
		
		void drawSpan(const VertexShaderOutput &a, const VertexShaderOutput &b, int y);
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