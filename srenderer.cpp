#include <GLFW/glfw3.h>
#include "srenderer.h"
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
		//TODO: Pixel based drawLine
		/*glm::ivec2 aUV(0, y);
		glm::ivec2 bUV(0, y);

		aUV.x=(a.fragCoord.x/a.fragCoord.w+1.0f)*fbo->getWidth()/2.0f;
		aUV.y=(a.fragCoord.y/a.fragCoord.w+1.0f)*fbo->getWidth()/2.0f;
		bUV.x=(b.fragCoord.x/b.fragCoord.w+1.0f)*fbo->getWidth()/2.0f;
		bUV.y=(b.fragCoord.y/b.fragCoord.w+1.0f)*fbo->getWidth()/2.0f;

		glm::vec2 dir;
		if(bUV.x!=aUV.x)
			dir=glm::vec2(1.0f, (float)(bUV.y-aUV.y)/(bUV.x-aUV.x));
		else
			dir=glm::vec2(1.0f, 0.0f);

		int step=0;
		do
		{
		}while(aUV+t!=bUV.x);*/
	}

	void SRenderer::drawSpan(const VertexShaderOutput &a, const VertexShaderOutput &b, int y)
	{
		// Pixel-based filling
		glm::ivec2 aUV(0, y);
		glm::ivec2 bUV(0, y);

		aUV.x=(a.fragCoord.x/a.fragCoord.w+1.0f)*fbo->getWidth()/2.0f;
		bUV.x=(b.fragCoord.x/b.fragCoord.w+1.0f)*fbo->getWidth()/2.0f;

		int start, end;
		const VertexShaderOutput *left, *right;
		if(aUV.x<bUV.x)
		{
			start=aUV.x;
			left=&a;
			right=&b;
			end=bUV.x;
		}else
		{
			start=bUV.x;
			left=&b;
			right=&a;
			end=aUV.x;
		}

		int x=start;

		while(x>=start&&x<=end)
		{
			left->interpolate(*right, (float)(x-start)/abs(aUV.x-bUV.x), outputSlot3);
			outputSlot3->fragCoord/=outputSlot3->fragCoord.w;

			glm::vec4 fragColor(1.0f, 1.0f, 1.0f, 1.0f);

			if(fs)
				fs(*outputSlot3, &fragColor);
			fbo->setPixel(x, y, outputSlot3->fragCoord.z, fragColor);

			count++;
			x++;
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
		float lo=abs(a.fragCoord.y/a.fragCoord.w-b.fragCoord.y/b.fragCoord.w), 
			la=abs(o.fragCoord.y/o.fragCoord.w-b.fragCoord.y/b.fragCoord.w), 
			lb=abs(o.fragCoord.y/o.fragCoord.w-a.fragCoord.y/a.fragCoord.w);
		const VertexShaderOutput *bottom=&o, *top=&a, *mid=&b;

		if(lo>=la&&lo>=lb)
		{
			bottom=(a.fragCoord.y/a.fragCoord.w<b.fragCoord.y/b.fragCoord.w)? &a: &b;
			top=(a.fragCoord.y/a.fragCoord.w>b.fragCoord.y/b.fragCoord.w)? &a: &b;
			mid=&o;
		}

		if(la>=lo&&la>=lb)
		{
			bottom=(o.fragCoord.y/o.fragCoord.w<b.fragCoord.y/b.fragCoord.w)? &o: &b;
			top=(o.fragCoord.y/o.fragCoord.w>b.fragCoord.y/b.fragCoord.w)? &o: &b;
			mid=&a;
		}

		if(lb>=lo&&lb>=la)
		{
			bottom=(o.fragCoord.y/o.fragCoord.w<a.fragCoord.y/a.fragCoord.w)? &o: &a;
			top=(o.fragCoord.y/o.fragCoord.w>a.fragCoord.y/a.fragCoord.w)? &o: &a;
			mid=&b;
		}

		glm::ivec2 bottomUV;
		glm::ivec2 midUV;
		glm::ivec2 topUV;

		// Use pixel-based algorithm 
		bottomUV.x=(bottom->fragCoord.x/bottom->fragCoord.w+1.0f)*fbo->getWidth()/2.0f;
		bottomUV.y=(bottom->fragCoord.y/bottom->fragCoord.w+1.0f)*fbo->getHeight()/2.0f;

		midUV.x=(mid->fragCoord.x/mid->fragCoord.w+1.0f)*fbo->getWidth()/2.0f;
		midUV.y=(mid->fragCoord.y/mid->fragCoord.w+1.0f)*fbo->getHeight()/2.0f;

		topUV.x=(top->fragCoord.x/top->fragCoord.w+1.0f)*fbo->getWidth()/2.0f;
		topUV.y=(top->fragCoord.y/top->fragCoord.w+1.0f)*fbo->getHeight()/2.0f;

		int y = bottomUV.y;
		while(bottomUV.y<midUV.y&&y<=midUV.y)
		{
			bottom->interpolate(*top, (float)(y-bottomUV.y)/(topUV.y-bottomUV.y), outputSlot1);
			bottom->interpolate(*mid, (float)(y-bottomUV.y)/(midUV.y-bottomUV.y), outputSlot2);

			drawSpan(*outputSlot1, *outputSlot2, y);
			y++;
		}

		y = topUV.y;
		while(topUV.y>midUV.y&&y>=midUV.y)
		{
			top->interpolate(*bottom, (float)(y-topUV.y)/(bottomUV.y-topUV.y), outputSlot1);
			top->interpolate(*mid, (float)(y-topUV.y)/(midUV.y-topUV.y), outputSlot2);

			drawSpan(*outputSlot1, *outputSlot2, y);
			y--;
		}
	}
}