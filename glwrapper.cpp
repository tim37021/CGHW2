#include <GLFW/glfw3.h>
#include "glwrapper.h"

#define abs(a) (a>=0?(a):-(a))

void GLWrapper::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLWrapper::drawPoint(float x, float y)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void GLWrapper::setPointColor(float r, float g, float b)
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