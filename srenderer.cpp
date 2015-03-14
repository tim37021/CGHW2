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

		int samples=1+(abs(b.pos.x-a.pos.x)>=abs(b.pos.y-a.pos.y)?abs(b.pos.x-a.pos.x)/pixelWidth:abs(b.pos.y-a.pos.y)/pixelWidth);

		for(int i=0; i<=samples; i++)
		{
			const Vertex &v=a.interpolate(b, t);
			float rf=1.0f, gf=1.0f, bf=1.0f;

			if(fs)
				fs(v, &rf, &gf, &bf);

			GLWrapper::setColor(rf, gf, bf);
			GLWrapper::drawPoint(v.pos.x, v.pos.y, v.pos.z);
			t+=1.0f/samples;
		}
	}

	void SRenderer::drawTriangle(const Vertex &o, const Vertex a, const Vertex &b)
	{
		GLWrapper::drawTriangle(o.pos.x, o.pos.y, a.pos.x, a.pos.y, b.pos.x, b.pos.y);
	}

	void SRenderer::drawFilledTriangle(const Vertex &o, const Vertex a, const Vertex &b)
	{
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
		
		float y=bottom->pos.y;

		// Step 3 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{

			if(abs(mid->pos.y-bottom->pos.y)<=1e-6) 
				continue;

			const Vertex &left=bottom->interpolate(*top, (y-bottom->pos.y)/(top->pos.y-bottom->pos.y));
			const Vertex &right=bottom->interpolate(*mid, (y-bottom->pos.y)/(mid->pos.y-bottom->pos.y));

			drawLine(right, left);
			
			y+=(mid->pos.y-bottom->pos.y)/samples;
		}

		samples=1+(abs(top->pos.x-mid->pos.x)>=abs(top->pos.y-mid->pos.y)
			?abs(top->pos.x-mid->pos.x)/pixelWidth:abs(top->pos.y-mid->pos.y)/pixelWidth);
		y=top->pos.y;

		// Step 4 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			if(abs(mid->pos.y-top->pos.y)<=1e-6) 
				continue;

			const Vertex &left=top->interpolate(*bottom, (y-top->pos.y)/(bottom->pos.y-top->pos.y));
			const Vertex &right=top->interpolate(*mid, (y-top->pos.y)/(mid->pos.y-top->pos.y));

			drawLine(right, left);

			y+=(mid->pos.y-top->pos.y)/samples;
		}
	}
}