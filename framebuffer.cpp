#include <GLFW/glfw3.h>
#include "framebuffer.h"

#define INF 999999.0f

namespace SRenderer
{
	FrameBuffer::FrameBuffer(int width, int height):
		m_width(width), m_height(height)
	{
		m_pixelBuffer = new glm::vec3[width*height];
		m_depthBuffer = new float[width*height];
		clearBuffer();
	}

	FrameBuffer::~FrameBuffer()
	{
		delete [] m_pixelBuffer;
		delete [] m_depthBuffer;
	}

	void FrameBuffer::getCanvasSize(int *width, int *height) const
	{
		*width=m_width;
		*height=m_height;
	}

	void FrameBuffer::setPixel(const glm::vec3 &pos, const glm::vec3 &color)
	{
		// We must convert NDC to window coordinate first
		// [-1, 1] -> [0, width] and [0, height]
		int fragPixel_x = (pos.x+1)*m_width/2.0f;
		int fragPixel_y = (pos.y+1)*m_height/2.0f;

		// cull pixels that are outside the window
		if(fragPixel_x >= m_width || fragPixel_y >= m_height)
			return;

		int offset = fragPixel_y*m_width+fragPixel_x;

		if(!m_enableDepthTest||pos.z<m_depthBuffer[offset])
		{
			m_pixelBuffer[offset]=color;
			m_depthBuffer[offset]=pos.z;
		}
	}

	void FrameBuffer::clearBuffer()
	{
		int x, y;
		for(y=0; y<m_height; y++)
		{
			for(x=0; x<m_width; x++)
			{
				const int offset = y*m_width+x;
				m_pixelBuffer[offset]=glm::vec3();
				m_depthBuffer[offset]=INF;
			}
		}
	}

	void FrameBuffer::enableDepthTest(bool value)
	{
		m_enableDepthTest=value;
	}

	void FrameBuffer::upload() const
	{
		int x, y;
		glBegin(GL_POINTS);
		for(y=0; y<m_height; y++)
		{
			for(x=0; x<m_width; x++)
			{
				const int offset = y*m_width+x;
				glColor3f(m_pixelBuffer[offset].x, m_pixelBuffer[offset].y, m_pixelBuffer[offset].z);
				glVertex2f(x*2.0f/m_width-1.0f, y*2.0f/m_height-1.0f);
			}
		}
		glEnd();
	}
}