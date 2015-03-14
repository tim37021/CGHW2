#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <glm/vec3.hpp>
#include <glm/vec3.hpp>

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
		void setPixel(const glm::vec3 &pos, const glm::vec3 &color);

		void clearBuffer();

		void enableDepthTest(bool);

		void upload() const;
	private:
		int m_width, m_height;

		//row major
		glm::vec3 *m_pixelBuffer;
		float *m_depthBuffer;

		//Settings
		bool m_enableDepthTest;
	};
}

#endif