#include <GLFW/glfw3.h>
#include "glwrapper.h"

#define abs(a) (a>=0?(a):-(a))

void GLWrapper::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void GLWrapper::drawPoint(float x, float y)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void GLWrapper::drawPoint(float x, float y, float z)
{
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();
}

void GLWrapper::setColor(float r, float g, float b)
{
	glColor3f(r, g, b);
}

void GLWrapper::drawLine(float x1, float y1, float x2, float y2)
{
	// Complete your code here, you can only use GLWrapper::drawPoint
	// To determine how many samples you want, You may need to calculate the pixel width
	int width, height;
	GLFWwindow *window=glfwGetCurrentContext();
	glfwGetWindowSize(window, &width, &height);

	float pixel_width=2.0f/(width>=height?width:height);

	int samples=1+(abs(x2-x1)>=abs(y2-y1)?abs(x2-x1)/pixel_width:abs(y2-y1)/pixel_width);
	
	float dx=(x2-x1)/samples;
	float dy=(y2-y1)/samples;

	for(int i=0; i<=samples; i++)
		drawPoint(x1+dx*i, y1+dy*i);
}

void GLWrapper::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	drawLine(x1, y1, x2, y2);
	drawLine(x1, y1, x3, y3);
	drawLine(x2, y2, x3, y3);
}