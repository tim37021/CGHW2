#include "texture.h"

#define INF 999999

namespace SRenderer
{

	ImageTexture::ImageTexture(int w, int h):
		Texture(w, h, new unsigned char[4*w*h])
	{
	}

	void ImageTexture::clear()
	{
		int x, y;
		int width=getWidth(), height=getHeight();
		unsigned char *raw_ptr=data();
		for(y=0; y<height; y++)
		{
			for(x=0; x<width; x++)
			{
				const int offset = y*width+x;
				raw_ptr[4*offset]=0;
				raw_ptr[4*offset+1]=0;
				raw_ptr[4*offset+2]=0;
				raw_ptr[4*offset+3]=0;
			}
		}
	}

	void ImageTexture::setPixel(const glm::vec2 &pos, const glm::vec4 &color)
	{
		int width=getWidth(), height=getHeight();
		// We must convert NDC to window coordinate first
		// [-1, 1] -> [0, width] and [0, height]
		int fragPixel_x = (pos.x+1)*width/2.0f;
		int fragPixel_y = (pos.y+1)*height/2.0f;

		setPixel(fragPixel_x, fragPixel_y, color);
	}

	void ImageTexture::setPixel(int fragPixel_x, int fragPixel_y, const glm::vec4 &color)
	{
		unsigned char *raw_ptr=data();
		int width=getWidth(), height=getHeight();

		// cull pixels that are outside the window
		if(fragPixel_x < 0 || fragPixel_x >= width || fragPixel_y < 0 || fragPixel_y >= height)
			return;

		int offset = fragPixel_y*width+fragPixel_x;

		raw_ptr[4*offset]=static_cast<int>(glm::clamp(color.b, 0.0f, 1.0f)*255.0f);
		raw_ptr[4*offset+1]=static_cast<int>(glm::clamp(color.g, 0.0f, 1.0f)*255.0f);
		raw_ptr[4*offset+2]=static_cast<int>(glm::clamp(color.r, 0.0f, 1.0f)*255.0f);
	}



	DepthTexture::DepthTexture(int w, int h):
		Texture(w, h, new float[w*h])
	{
	}

	void DepthTexture::clear()
	{
		int x, y;
		int width=getWidth(), height=getHeight();
		float *raw_ptr=data();
		for(y=0; y<height; y++)
		{
			for(x=0; x<width; x++)
			{
				const int offset = y*width+x;
				raw_ptr[offset]=INF;
			}
		}
	}

	void DepthTexture::setPixel(const glm::vec2 &pos, float depth)
	{
		int width=getWidth(), height=getHeight();
		// We must convert NDC to window coordinate first
		// [-1, 1] -> [0, width] and [0, height]
		int fragPixel_x = (pos.x+1)*width/2.0f;
		int fragPixel_y = (pos.y+1)*height/2.0f;

		setPixel(fragPixel_x, fragPixel_y, depth);
	}

	void DepthTexture::setPixel(int fragPixel_x, int fragPixel_y, float depth)
	{
		float *raw_ptr=data();
		int width=getWidth(), height=getHeight();

		// cull pixels that are outside the window
		if(fragPixel_x < 0 || fragPixel_x >= width || fragPixel_y < 0 || fragPixel_y >= height)
			return;

		int offset = fragPixel_y*width+fragPixel_x;
		raw_ptr[offset]=depth;
	}

	float DepthTexture::getPixel(int x, int y) const
	{
		float *raw_ptr=data();
		int width=getWidth(), height=getHeight();

		// cull pixels that are outside the window
		x=glm::clamp<int>(x, 0, width-1);
		y=glm::clamp<int>(y, 0, height-1);

		return raw_ptr[y*width+x];
	}
}