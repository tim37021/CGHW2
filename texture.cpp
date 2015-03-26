#include "texture.h"
#include <bitmap_image.hpp>

#define INF 999999

namespace SRenderer
{

	ImageTexture::ImageTexture(int w, int h):
		Texture(w, h, new unsigned char[4*w*h])
	{
	}

	ImageTexture::ImageTexture(const char *filename)
	{
		loadBMPImage(filename);
	}

	void ImageTexture::clear()
	{
		int x, y;
		unsigned char *raw_ptr=data();
		for(y=0; y<m_height; y++)
		{
			for(x=0; x<m_width; x++)
			{
				const int offset = y*m_width+x;
				raw_ptr[4*offset]=0;
				raw_ptr[4*offset+1]=0;
				raw_ptr[4*offset+2]=0;
				raw_ptr[4*offset+3]=0;
			}
		}
	}

	void ImageTexture::setPixel(const glm::vec2 &pos, const glm::vec4 &color)
	{
		setPixel(pos.x*m_width, pos.y*m_height, color);
	}

	void ImageTexture::setPixel(int fragPixel_x, int fragPixel_y, const glm::vec4 &color)
	{
		unsigned char *raw_ptr=data();

		// Clamp to border
		fragPixel_x=glm::clamp<int>(fragPixel_x, 0, m_width-1);
		fragPixel_y=glm::clamp<int>(fragPixel_y, 0, m_height-1);

		int offset = fragPixel_y*m_width+fragPixel_x;

		raw_ptr[4*offset]=static_cast<int>(glm::clamp(color.b, 0.0f, 1.0f)*255.0f);
		raw_ptr[4*offset+1]=static_cast<int>(glm::clamp(color.g, 0.0f, 1.0f)*255.0f);
		raw_ptr[4*offset+2]=static_cast<int>(glm::clamp(color.r, 0.0f, 1.0f)*255.0f);
	}

	glm::vec4 ImageTexture::getPixel(int x, int y) const
	{
		unsigned char *raw_ptr=data();

		// cull pixels that are outside the window
		x=glm::clamp<int>(x, 0, m_width-1);
		y=glm::clamp<int>(y, 0, m_height-1);

		int offset = y*m_width+x;

		//BGRA -> RGBA
		return glm::vec4(raw_ptr[offset*4+2]/255.0, 
					raw_ptr[offset*4+1]/255.0, 
					raw_ptr[offset*4]/255.0,
					raw_ptr[offset*4+3]/255.0);
	}

	glm::vec4 ImageTexture::getPixel(const glm::vec2 &pos) const
	{
		// We must convert NDC to window coordinate first
		// [0, 1] -> [0, width] and [0, height]
		return getPixel(pos.x*m_width, pos.y*m_height);
	}

	// Image Loader
	void ImageTexture::loadBMPImage(const char *filename)
	{
		bitmap_image image(filename);

		unsigned char r, g, b;

		const unsigned int height = image.height();
		const unsigned int width  = image.width();

		assign(new unsigned char[4*height*width]);

		unsigned char *raw_ptr=data();

		for (std::size_t y = 0; y < height; ++y)
		{
			for (std::size_t x = 0; x < width; ++x)
			{
				int offset = y*width+x;
				image.get_pixel(x, height-1-y, r, g, b);
				raw_ptr[4*offset]=b;
				raw_ptr[4*offset+1]=g;
				raw_ptr[4*offset+2]=r;
				raw_ptr[4*offset+3]=0;
			}
		}

		m_width = width;
		m_height = height;
	}

	DepthTexture::DepthTexture(int w, int h):
		Texture(w, h, new float[w*h])
	{
	}

	void DepthTexture::clear()
	{
		int x, y;
		float *raw_ptr=data();
		for(y=0; y<m_height; y++)
		{
			for(x=0; x<m_width; x++)
			{
				const int offset = y*m_width+x;
				raw_ptr[offset]=INF;
			}
		}
	}

	void DepthTexture::setPixel(const glm::vec2 &pos, float depth)
	{
		setPixel(pos.x*m_width, pos.y*m_height, depth);
	}

	void DepthTexture::setPixel(int fragPixel_x, int fragPixel_y, float depth)
	{
		float *raw_ptr=data();

		// Clamp to border
		fragPixel_x=glm::clamp<int>(fragPixel_x, 0, m_width-1);
		fragPixel_y=glm::clamp<int>(fragPixel_y, 0, m_height-1);

		int offset = fragPixel_y*m_width+fragPixel_x;
		raw_ptr[offset]=depth;
	}

	float DepthTexture::getPixel(int x, int y) const
	{
		float *raw_ptr=data();

		// Clamp to border
		x=glm::clamp<int>(x, 0, m_width-1);
		y=glm::clamp<int>(y, 0, m_height-1);

		return raw_ptr[y*m_width+x];
	}
}