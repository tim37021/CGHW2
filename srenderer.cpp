#include <GLFW/glfw3.h>
#include "srenderer.h"
#include "glwrapper.h"

#define abs(a) (a>=0?(a):-(a))

namespace SRenderer
{

	SRenderer::SRenderer(FrameBuffer *fbo_, VertexShader vs_, FragmentShader fs_):
		fbo(fbo_), vs(vs_), fs(fs_)
	{

	}

	void SRenderer::setVertexShader(VertexShader vs_)
	{
		vs=vs_;
	}
	void SRenderer::setFragmentShader(FragmentShader fs_)
	{
		fs=fs_;
	}

	void SRenderer::render(const Mesh &mesh)
	{
		for(int i=0; i<mesh.indices.size(); i+=3)
		{
			int o=mesh.indices[i]-1;
			int a=mesh.indices[i+1]-1;
			int b=mesh.indices[i+2]-1;

			VertexShaderOutput vo, va, vb;
			if(vs)
			{
				// Pass each to vertex shader if vs is not null
				vs(mesh.vertices[o], &vo);
				vs(mesh.vertices[a], &va);
				vs(mesh.vertices[b], &vb);
			}
			
			// Call to SRenderer::drawTriangle
			drawFilledTriangle(vo, va, vb);
		}
	}

	void SRenderer::drawLine(const VertexShaderOutput &a, const VertexShaderOutput &b)
	{
		float t=0.0f;

		float pixelWidth=fbo->getPixelWidth();

		int samples=1+(abs(b.fragCoord.x-a.fragCoord.x)>=abs(b.fragCoord.y-a.fragCoord.y)?
			abs(b.fragCoord.x-a.fragCoord.x)/pixelWidth:abs(b.fragCoord.y-a.fragCoord.y)/pixelWidth);

		for(int i=0; i<=samples; i++)
		{
			const VertexShaderOutput &v=a.interpolate(b, t);
			glm::vec4 fragColor(1.0f, 1.0f, 1.0f, 1.0f);

			if(fs)
				fs(v, &fragColor);

			fbo->setPixel(glm::vec3(v.fragCoord), fragColor);
			t+=1.0f/samples;
		}
	}

	void SRenderer::drawTriangle(const VertexShaderOutput &o, const VertexShaderOutput &a, const VertexShaderOutput &b)
	{
		drawLine(o, a);
		drawLine(o, b);
		drawLine(a, b);
	}

	void SRenderer::drawFilledTriangle(const VertexShaderOutput &o, const VertexShaderOutput &a, const VertexShaderOutput &b)
	{
		float pixelWidth=fbo->getPixelWidth();

		// Step 1 - find max |y'-y|
		float lo=abs(a.fragCoord.y-b.fragCoord.y), la=abs(o.fragCoord.y-b.fragCoord.y), lb=abs(o.fragCoord.y-a.fragCoord.y);
		const VertexShaderOutput *bottom=&o, *top=&a, *mid=&b;

		if(lo>=la&&lo>=lb)
		{
			bottom=(a.fragCoord.y<b.fragCoord.y)? &a: &b;
			top=(a.fragCoord.y>b.fragCoord.y)? &a: &b;
			mid=&o;
		}

		if(la>=lo&&la>=lb)
		{
			bottom=(o.fragCoord.y<b.fragCoord.y)? &o: &b;
			top=(o.fragCoord.y>b.fragCoord.y)? &o: &b;
			mid=&a;
		}

		if(lb>=lo&&lb>=la)
		{
			bottom=(o.fragCoord.y<a.fragCoord.y)? &o: &a;
			top=(o.fragCoord.y>a.fragCoord.y)? &o: &a;
			mid=&b;
		}

		// Step2 - Calculate how many samples do we need to draw a line from bottom to top
		int samples=1+(abs(mid->fragCoord.x-bottom->fragCoord.x)>=abs(mid->fragCoord.y-bottom->fragCoord.y)
			?abs(mid->fragCoord.x-bottom->fragCoord.x)/pixelWidth:abs(mid->fragCoord.y-bottom->fragCoord.y)/pixelWidth);

		// Step 3 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			const float y = bottom->fragCoord.y+i*(mid->fragCoord.y-bottom->fragCoord.y)/samples;

			if(abs(mid->fragCoord.y-bottom->fragCoord.y)<=1e-6) 
				continue;

			const VertexShaderOutput &left=bottom->interpolate(*top, 
				(y-bottom->fragCoord.y)/(top->fragCoord.y-bottom->fragCoord.y));
			const VertexShaderOutput &right=bottom->interpolate(*mid,
				(y-bottom->fragCoord.y)/(mid->fragCoord.y-bottom->fragCoord.y));

			drawLine(right, left);
			
			
		}

		samples=1+(abs(top->fragCoord.x-mid->fragCoord.x)>=abs(top->fragCoord.y-mid->fragCoord.y)
			?abs(top->fragCoord.x-mid->fragCoord.x)/pixelWidth:abs(top->fragCoord.y-mid->fragCoord.y)/pixelWidth);

		// Step 4 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			const float y=top->fragCoord.y + i*(mid->fragCoord.y-top->fragCoord.y)/samples;

			if(abs(mid->fragCoord.y-top->fragCoord.y)<=1e-6) 
				continue;

			const VertexShaderOutput &left=top->interpolate(*bottom, 
				(y-top->fragCoord.y)/(bottom->fragCoord.y-top->fragCoord.y));
			const VertexShaderOutput &right=top->interpolate(*mid, 
				(y-top->fragCoord.y)/(mid->fragCoord.y-top->fragCoord.y));

			drawLine(right, left);
		}
	}
}