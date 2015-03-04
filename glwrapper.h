#ifndef _GLWRAPPER_H_
#define _GLWRAPPER_H_

namespace GLWrapper
{
	void clearScreen();
	void drawPoint(float, float);
	void setPointColor(float r, float g, float b);
	void drawLine(float x1, float y1, float x2, float y2);
}

#endif