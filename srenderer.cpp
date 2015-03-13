#include <GLFW/glfw3.h>
#include "srenderer.h"
#include "glwrapper.h"

#define abs(a) (a>=0?(a):-(a))

namespace SRenderer
{

	SRenderer::SRenderer(int w_, int h_, VertexShader vs_, FragmentShader fs_):
		vs(vs_), fs(fs_)
	{
		setCanvasSize(w_, h_);
	}

	void SRenderer::setVertexShader(VertexShader vs_)
	{
		vs=vs_;
	}
	void SRenderer::setFragmentShader(FragmentShader fs_)
	{
		fs=fs_;
	}

	void SRenderer::setCanvasSize(int w_, int h_)
	{
		width=w_;
		height=h_;
		pixelWidth=2.0f/(width>=height?width:height);
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

		int samples=1+(abs(b.x-a.x)>=abs(b.y-a.y)?abs(b.x-a.x)/pixelWidth:abs(b.y-a.y)/pixelWidth);

		for(int i=0; i<=samples; i++)
		{
			const Vertex &v=a.interpolate(b, t);
			GLWrapper::drawPoint(v.x, v.y);
			t+=1.0f/samples;
		}
	}

	void SRenderer::drawTriangle(const Vertex &o, const Vertex a, const Vertex &b)
	{
		GLWrapper::drawTriangle(o.x, o.y, a.x, a.y, b.x, b.y);
	}

	void SRenderer::drawFilledTriangle(const Vertex &o, const Vertex a, const Vertex &b)
	{

		// Step 1 - find max |y'-y|
		float lo=abs(a.y-b.y), la=abs(o.y-b.y), lb=abs(o.y-a.y);
		const Vertex *bottom=&o, *top=&a, *mid=&b;


		if(lo>=la&&lo>=lb)
		{
			bottom=(a.y<b.y)? &a: &b;
			top=(a.y>b.y)? &a: &b;
			mid=&o;
		}

		if(la>=lo&&la>=lb)
		{
			bottom=(o.y<b.y)? &o: &b;
			top=(o.y>b.y)? &o: &b;
			mid=&a;
		}

		if(lb>=lo&&lb>=la)
		{
			bottom=(o.y<a.y)? &o: &a;
			top=(o.y>a.y)? &o: &a;
			mid=&b;
		}

		// Step2 - Calculate how many samples do we need to draw a line from bottom to top
		int samples=1+(abs(mid->x-bottom->x)>=abs(mid->y-bottom->y)
			?abs(mid->x-bottom->x)/pixelWidth:abs(mid->y-bottom->y)/pixelWidth);
		
		float y=bottom->y;

		// Step 3 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{

			if(abs(mid->y-bottom->y)<=1e-6) 
				continue;

			const Vertex &left=bottom->interpolate(*top, (y-bottom->y)/(top->y-bottom->y));
			const Vertex &right=bottom->interpolate(*mid, (y-bottom->y)/(mid->y-bottom->y));

			drawLine(right, left);
			
			y+=(mid->y-bottom->y)/samples;
		}

		samples=1+(abs(top->x-mid->x)>=abs(top->y-mid->y)
			?abs(top->x-mid->x)/pixelWidth:abs(top->y-mid->y)/pixelWidth);
		y=top->y;

		// Step 4 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			if(abs(mid->y-top->y)<=1e-6) 
				continue;

			const Vertex &left=top->interpolate(*bottom, (y-top->y)/(bottom->y-top->y));
			const Vertex &right=top->interpolate(*mid, (y-top->y)/(mid->y-top->y));

			drawLine(right, left);

			y+=(mid->y-top->y)/samples;
		}
	}
}