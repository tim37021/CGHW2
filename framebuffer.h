#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace SRenderer
{
	class FrameBuffer
	{
	public:
		FrameBuffer(int, int);
		~FrameBuffer();

		void getCanvasSize(int *, int *) const;
		float getPixelWidth() const
			{return 2.0f/(m_width>=m_height?m_width:m_height);}

		//NDC
		void setPixel(const glm::vec3 &pos, const glm::vec4 &color);
		//Pixel based
		void setPixel(int x, int y, float z, const glm::vec4 &color);

		void clearBuffer();

		void enableDepthTest(bool);

		int getWidth() const{ return m_width; }
		int getHeight() const{ return m_height; }

		void upload() const;
	private:
		int m_width, m_height;
		unsigned int m_texture;

		//row major
		//BGRA
		unsigned char *m_pixelBuffer;
		float *m_depthBuffer;

		//Settings
		bool m_enableDepthTest;
	};
}

#endif