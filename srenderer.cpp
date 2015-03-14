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

			Vertex vo=mesh.vertices[o], va=mesh.vertices[a], vb=mesh.vertices[b];
			
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

	void SRenderer::drawLine(const Vertex a, const Vertex &b)
	{
		float t=0.0f;

		float pixelWidth=fbo->getPixelWidth();

		int samples=1+(abs(b.pos.x-a.pos.x)>=abs(b.pos.y-a.pos.y)?abs(b.pos.x-a.pos.x)/pixelWidth:abs(b.pos.y-a.pos.y)/pixelWidth);

		for(int i=0; i<=samples; i++)
		{
			const Vertex &v=a.interpolate(b, t);
			glm::vec4 fragColor(1.0f, 1.0f, 1.0f, 1.0f);

			if(fs)
				fs(v, &fragColor);

			fbo->setPixel(v.pos, fragColor);
			t+=1.0f/samples;
		}
	}

	void SRenderer::drawTriangle(const Vertex &o, const Vertex a, const Vertex &b)
	{
		drawLine(o, a);
		drawLine(o, b);
		drawLine(a, b);
	}

	void SRenderer::drawFilledTriangle(const Vertex &o, const Vertex a, const Vertex &b)
	{
		float pixelWidth=fbo->getPixelWidth();

		// Step 1 - find max |y'-y|
		float lo=abs(a.pos.y-b.pos.y), la=abs(o.pos.y-b.pos.y), lb=abs(o.pos.y-a.pos.y);
		const Vertex *bottom=&o, *top=&a, *mid=&b;

		if(lo>=la&&lo>=lb)
		{
			bottom=(a.pos.y<b.pos.y)? &a: &b;
			top=(a.pos.y>b.pos.y)? &a: &b;
			mid=&o;
		}

		if(la>=lo&&la>=lb)
		{
			bottom=(o.pos.y<b.pos.y)? &o: &b;
			top=(o.pos.y>b.pos.y)? &o: &b;
			mid=&a;
		}

		if(lb>=lo&&lb>=la)
		{
			bottom=(o.pos.y<a.pos.y)? &o: &a;
			top=(o.pos.y>a.pos.y)? &o: &a;
			mid=&b;
		}

		// Step2 - Calculate how many samples do we need to draw a line from bottom to top
		int samples=1+(abs(mid->pos.x-bottom->pos.x)>=abs(mid->pos.y-bottom->pos.y)
			?abs(mid->pos.x-bottom->pos.x)/pixelWidth:abs(mid->pos.y-bottom->pos.y)/pixelWidth);

		// Step 3 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			const float y = bottom->pos.y+i*(mid->pos.y-bottom->pos.y)/samples;

			if(abs(mid->pos.y-bottom->pos.y)<=1e-6) 
				continue;

			const Vertex &left=bottom->interpolate(*top, (y-bottom->pos.y)/(top->pos.y-bottom->pos.y));
			const Vertex &right=bottom->interpolate(*mid, (y-bottom->pos.y)/(mid->pos.y-bottom->pos.y));

			drawLine(right, left);
			
			
		}

		samples=1+(abs(top->pos.x-mid->pos.x)>=abs(top->pos.y-mid->pos.y)
			?abs(top->pos.x-mid->pos.x)/pixelWidth:abs(top->pos.y-mid->pos.y)/pixelWidth);

		// Step 4 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			const float y=top->pos.y + i*(mid->pos.y-top->pos.y)/samples;

			if(abs(mid->pos.y-top->pos.y)<=1e-6) 
				continue;

			const Vertex &left=top->interpolate(*bottom, (y-top->pos.y)/(bottom->pos.y-top->pos.y));
			const Vertex &right=top->interpolate(*mid, (y-top->pos.y)/(mid->pos.y-top->pos.y));

			drawLine(right, left);
		}
	}
}