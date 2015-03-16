#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "framebuffer.h"

#define INF 999999.0f

namespace SRenderer
{
	FrameBuffer::FrameBuffer(int width, int height):
		m_width(width), m_height(height)
	{
		m_pixelBuffer = new unsigned char[4*width*height];
		m_depthBuffer = new float[width*height];
		clearBuffer();

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)m_pixelBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FrameBuffer::~FrameBuffer()
	{
		delete [] m_pixelBuffer;
		delete [] m_depthBuffer;

		glDeleteTextures(1, &m_texture);
	}

	void FrameBuffer::getCanvasSize(int *width, int *height) const
	{
		*width=m_width;
		*height=m_height;
	}

	void FrameBuffer::setPixel(const glm::vec3 &pos, const glm::vec4 &color)
	{
		// We must convert NDC to window coordinate first
		// [-1, 1] -> [0, width] and [0, height]
		int fragPixel_x = (pos.x+1)*m_width/2.0f;
		int fragPixel_y = (pos.y+1)*m_height/2.0f;

		// cull pixels that are outside the window
		if(fragPixel_x < 0 || fragPixel_x >= m_width || fragPixel_y < 0 || fragPixel_y >= m_height)
			return;

		int offset = fragPixel_y*m_width+fragPixel_x;

		if(!m_enableDepthTest||pos.z<m_depthBuffer[offset])
		{
			m_pixelBuffer[4*offset]=static_cast<int>(glm::clamp(color.b, 0.0f, 1.0f)*255.0f);
			m_pixelBuffer[4*offset+1]=static_cast<int>(glm::clamp(color.g, 0.0f, 1.0f)*255.0f);
			m_pixelBuffer[4*offset+2]=static_cast<int>(glm::clamp(color.r, 0.0f, 1.0f)*255.0f);
			m_depthBuffer[offset]=pos.z;
		}
	}

	void FrameBuffer::setPixel(int fragPixel_x, int fragPixel_y, float z, const glm::vec4 &color)
	{
		// cull pixels that are outside the window
		if(fragPixel_x < 0 || fragPixel_x >= m_width || fragPixel_y < 0 || fragPixel_y >= m_height)
			return;

		int offset = fragPixel_y*m_width+fragPixel_x;

		if(!m_enableDepthTest||z<m_depthBuffer[offset])
		{
			m_pixelBuffer[4*offset]=static_cast<int>(glm::clamp(color.b, 0.0f, 1.0f)*255.0f);
			m_pixelBuffer[4*offset+1]=static_cast<int>(glm::clamp(color.g, 0.0f, 1.0f)*255.0f);
			m_pixelBuffer[4*offset+2]=static_cast<int>(glm::clamp(color.r, 0.0f, 1.0f)*255.0f);
			m_depthBuffer[offset]=z;
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
				m_pixelBuffer[4*offset]=0;
				m_pixelBuffer[4*offset+1]=0;
				m_pixelBuffer[4*offset+2]=0;
				m_pixelBuffer[4*offset+3]=0;
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
		glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height,
        	GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)m_pixelBuffer);

        glEnable(GL_TEXTURE_2D);

		// draw a point with texture
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);   glVertex3f( 1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);   glVertex3f( 1.0f,  1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);   glVertex3f(-1.0f,  1.0f, 0.0f);
		glEnd();

		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}