#include <GLFW/glfw3.h>
#include "srenderer.h"
#include "glwrapper.h"
#include <glm/glm.hpp>

#define abs(a) (a>=0?(a):-(a))
extern int count;

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
		outputSlot1=nullptr;
		outputSlot2=nullptr;
		outputSlot3=nullptr;
		for(int i=0; i<mesh.indices.size(); i+=3)
		{
			int o=mesh.indices[i]-1;
			int a=mesh.indices[i+1]-1;
			int b=mesh.indices[i+2]-1;

			VertexShaderOutput *vo, *va, *vb;
			if(vs)
			{
				// Pass each to vertex shader if vs is not null
				vo=vs(mesh.vertices[o]);
				va=vs(mesh.vertices[a]);
				vb=vs(mesh.vertices[b]);
			}
			
			outputSlot1=outputSlot1?outputSlot1: dynamic_cast<VertexShaderOutput *>(vo->clone());
			outputSlot2=outputSlot2?outputSlot2: dynamic_cast<VertexShaderOutput *>(vo->clone());
			outputSlot3=outputSlot3?outputSlot3: dynamic_cast<VertexShaderOutput *>(vo->clone());

			//backface culling
			glm::vec3 oo(vo->fragCoord.x, vo->fragCoord.y, 0);
			glm::vec3 aa(va->fragCoord.x, va->fragCoord.y, 0);
			glm::vec3 bb(vb->fragCoord.x, vb->fragCoord.y, 0);

			glm::vec3 oa=aa-oo, ob=bb-oo;
			if(glm::cross(oa, ob).z>0)
			{
				// Call to SRenderer::drawTriangle
				drawFilledTriangle(*vo, *va, *vb);
			}

			delete vo;
			delete va;
			delete vb;
		}
		delete outputSlot1;
		delete outputSlot2;
		delete outputSlot3;
	}

	void SRenderer::drawLine(const VertexShaderOutput &a, const VertexShaderOutput &b)
	{
		float t=0.0f;

		float pixelWidth=fbo->getPixelWidth();

		int samples=1+(abs(b.fragCoord.x-a.fragCoord.x)>=abs(b.fragCoord.y-a.fragCoord.y)?
			abs(b.fragCoord.x/b.fragCoord.w-a.fragCoord.x/a.fragCoord.w)/pixelWidth:
			abs(b.fragCoord.y/b.fragCoord.w-a.fragCoord.y/a.fragCoord.w)/pixelWidth);

		count+=samples;

		for(int i=0; i<=samples; i++)
		{
			a.interpolate(b, t, outputSlot3);
			VertexShaderOutput *v = outputSlot3;
			glm::vec4 fragColor(1.0f, 1.0f, 1.0f, 1.0f);

			v->fragCoord/=v->fragCoord.w;
			if(fs)
				fs(*v, &fragColor);

			fbo->setPixel(glm::vec3(v->fragCoord), fragColor);
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
		int samples=1+abs(mid->fragCoord.y/mid->fragCoord.w-bottom->fragCoord.y/bottom->fragCoord.w)/pixelWidth;
		// Step 3 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			const float y = bottom->fragCoord.y+i*(mid->fragCoord.y-bottom->fragCoord.y)/samples;

			if(abs(mid->fragCoord.y-bottom->fragCoord.y)<=1e-6) 
				continue;

			bottom->interpolate(*top, (y-bottom->fragCoord.y)/(top->fragCoord.y-bottom->fragCoord.y), outputSlot1);
			bottom->interpolate(*mid, (y-bottom->fragCoord.y)/(mid->fragCoord.y-bottom->fragCoord.y), outputSlot2);

			drawLine(*outputSlot1, *outputSlot2);
			
		}
		
		samples=100+abs(top->fragCoord.y/top->fragCoord.w-mid->fragCoord.y/mid->fragCoord.w)/pixelWidth;

		// Step 4 - Fill the triangle
		for(int i=0; i<=samples; i++)
		{
			const float y=top->fragCoord.y+i*(mid->fragCoord.y-top->fragCoord.y)/samples;

			if(abs(mid->fragCoord.y-top->fragCoord.y)<=1e-6) 
				continue;

			top->interpolate(*bottom, (y-top->fragCoord.y)/(bottom->fragCoord.y-top->fragCoord.y), outputSlot1);
			top->interpolate(*mid, (y-top->fragCoord.y)/(mid->fragCoord.y-top->fragCoord.y), outputSlot2);

			drawLine(*outputSlot1, *outputSlot2);
		}
	}
}