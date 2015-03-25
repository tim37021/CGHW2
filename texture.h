#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glm/glm.hpp>
#include <memory>

namespace SRenderer
{
	// custom deleter for shared_ptr array
	template< typename T >
	struct ArrayDeleter
	{
		void operator ()( T const * p)
		{ 
			delete[] p; 
		}
	};

	template <class T>
	class Texture
	{
	public:
		Texture()=default;
		Texture(int w, int h, T *ptr):
			m_width(w), 
			m_height(h), 
			m_buffer(ptr, ArrayDeleter<T>()){}

		virtual void clear()=0;

		int getWidth() const {return m_width;}
		int getHeight() const {return m_height;}

		operator bool() const { return m_buffer.get()!=nullptr; }

		// Dangerous zone
		void *data() const { return m_buffer.get(); }
	protected:
		std::shared_ptr<T> m_buffer;
		int m_width, m_height;
	};

	class ImageTexture: public Texture<unsigned char>
	{
	public:
		ImageTexture(int, int);
		ImageTexture(const char *);

		virtual void clear();

		// color will be auto-clamped, [0, 1]
		void setPixel(int x, int y, const glm::vec4 &color);
		void setPixel(const glm::vec2 &pos, const glm::vec4 &color);

		glm::vec4 getPixel(int x, int y) const;
		glm::vec4 getPixel(const glm::vec2 &pos) const;

		void saveImage(const char *filename) const;

		// Dangerous zone
		// Overloading Texture's data method
		unsigned char *data() const
		{ return reinterpret_cast<unsigned char *>(m_buffer.get()); }
	private:
		void loadBMPImage(const char *filename);
	};

	class DepthTexture: public Texture<float>
	{
	public:
		DepthTexture(int, int);

		virtual void clear();

		// color will be auto-clamped
		void setPixel(int x, int y, float depth);
		void setPixel(const glm::vec2 &pos, float depth);

		float getPixel(int x, int y) const;
		float getPixel(const glm::vec2 &pos) const;

		// Dangerous zone
		// Overloading Texture's data method
		float *data() const
		{ return reinterpret_cast<float *>(m_buffer.get()); }
	};
}

#endif