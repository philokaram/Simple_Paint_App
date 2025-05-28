#include <windef.h>
#include <math.h>
#include <algorithm>
#include <wingdi.h>
//Direct




//Polar
void EllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
    double theta = 0;
    double dTheta = 1.0 / std::max(a, b); 

    int x = round(a * cos(theta));
    int y = round(b * sin(theta));

    while (theta < 2 * 3.14159265) {
        x = round(a * cos(theta));
        y = round(b * sin(theta));

        SetPixel(hdc, xc + x, yc + y, c);  // Just plot one point per theta
        theta += dTheta;
    }
}




//Midpoint

