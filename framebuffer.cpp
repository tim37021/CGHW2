#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "framebuffer.h"

#define INF 999999.0f

static const char *vertex_shader=
"#version 330\n"
"layout(location=0) in vec3 vert;\n"
"layout(location=1) in vec2 texCoord;\n"
"out vec2 vTexCoord;\n"
"void main()\n"
"{\n"
"vTexCoord=texCoord;\n"
"gl_Position=vec4(vert, 1.0);\n"
"}\n";

static const char *fragment_shader=
"#version 330\n"
"uniform sampler2D text;\n"
"in vec2 vTexCoord;\n"
"void main()\n"
"{\n"
"gl_FragColor=texture(text, vTexCoord);\n"
"}\n";

// {vert, texCoord}
static const GLfloat data[4][5]=
{
	{-1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	{-1.0f, -1.0f, 0.0f, 0.0f, 0.0f},
	{1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
	{1.0f, 1.0f, 0.0f, 1.0f, 1.0f}
};

static const unsigned int indices[]={0, 1, 2, 0, 2, 3};


namespace SRenderer
{
	FrameBuffer::FrameBuffer(int width, int height):
		m_width(width), m_height(height), 
		pixelTexture(width, height), depthTexture(width, height)
	{
		clearBuffer();

		setupGLStuff();

		// Create texture on GPU
		glGenTextures(1, &m_texture);
		resetGLTexture();
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteTextures(1, &m_texture);
		//glDeleteVertexArrays(1, &vao);
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

		int offset = fragPixel_y*m_width+fragPixel_x;

		setPixel(fragPixel_x, fragPixel_y, pos.z, color);
	}

	void FrameBuffer::setPixel(int fragPixel_x, int fragPixel_y, float z, const glm::vec4 &color)
	{
		int offset = fragPixel_y*m_width+fragPixel_x;

		if(!m_enableDepthTest||z<depthTexture.getPixel(fragPixel_x, fragPixel_y))
		{
			pixelTexture.setPixel(fragPixel_x, fragPixel_y, color);
			depthTexture.setPixel(fragPixel_x, fragPixel_y, z);
		}
	}

	void FrameBuffer::clearBuffer()
	{
		pixelTexture.clear();
		depthTexture.clear();
	}

	void FrameBuffer::enableDepthTest(bool value)
	{
		m_enableDepthTest=value;
	}

	void FrameBuffer::resize(int w, int h)
	{
		m_width = w;
		m_height = h;
		pixelTexture = ImageTexture(w, h);
		depthTexture = DepthTexture(w, h);

		pixelTexture.clear();

		resetGLTexture();

	}

	void FrameBuffer::upload() const
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height,
        	GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)pixelTexture.data());

        glUseProgram(m_glshader);

        glBindVertexArray(m_glvao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        int text=glGetUniformLocation(m_glshader, "text");
        glUniform1i(text, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void FrameBuffer::setupGLStuff()
	{
		GLuint vs, fs;
		glGenVertexArrays(1, &m_glvao);
		glBindVertexArray(m_glvao);

		glGenBuffers(1, &m_glvbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_glvbo);
		glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid *)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));

		//generate index buffer
		glGenBuffers(1, &m_glibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		vs=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, (const GLchar**)&vertex_shader, nullptr);

		glCompileShader(vs);

		int status, maxLength;
		char *infoLog=nullptr;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if(status==GL_FALSE)
		{
			glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

			/* The maxLength includes the NULL character */
			infoLog = new char[maxLength];

			glGetShaderInfoLog(vs, maxLength, &maxLength, infoLog);

			fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);

			/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
			/* In this simple program, we'll just leave */
			delete [] infoLog;
			return;
		}

		fs=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, (const GLchar**)&fragment_shader, nullptr);
		glCompileShader(fs);

		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		if(status==GL_FALSE)
		{
			glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

			/* The maxLength includes the NULL character */
			infoLog = new char[maxLength];

			glGetShaderInfoLog(fs, maxLength, &maxLength, infoLog);

			fprintf(stderr, "Fragment Shader Error: %s\n", infoLog);

			/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
			/* In this simple program, we'll just leave */
			delete [] infoLog;
			return;
		}

		m_glshader=glCreateProgram();
		// Attach our shaders to our program
		glAttachShader(m_glshader, vs);
		glAttachShader(m_glshader, fs);

		glLinkProgram(m_glshader);

		glGetProgramiv(m_glshader, GL_LINK_STATUS, &status);

		if(status==GL_FALSE)
		{
			glGetProgramiv(m_glshader, GL_INFO_LOG_LENGTH, &maxLength);
			

			/* The maxLength includes the NULL character */
			infoLog = new char[maxLength];
			glGetProgramInfoLog(m_glshader, maxLength, NULL, infoLog);

			glGetProgramInfoLog(m_glshader, maxLength, &maxLength, infoLog);

			fprintf(stderr, "Vertex Shader Error: %s\n", infoLog);

			/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
			/* In this simple program, we'll just leave */
			delete [] infoLog;
			return;
		}
	} 
	void FrameBuffer::resetGLTexture()
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)pixelTexture.data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}