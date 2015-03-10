#include "hw2.h"
#include "glwrapper.h"
#include <cmath>

#ifndef M_PI
#define M_PI    3.1415926535
#endif

void HW2::drawCircle(float x, float y, float r, int seg)
{
    float x1, x2, y1, y2;
    float gradient=2*M_PI/seg;
    for(int i=1; i<=seg; i++)
    {
        x1=x+r*std::cos((i-1)*gradient);
        y1=y+r*std::sin((i-1)*gradient);

        x2=x+r*std::cos(i*gradient);
        y2=y+r*std::sin(i*gradient);
        GLWrapper::drawLine(x1, y1, x2, y2);
    }
}
