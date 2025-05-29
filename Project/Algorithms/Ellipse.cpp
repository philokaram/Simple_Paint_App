#include <windef.h>
#include <math.h>
#include <algorithm>
#include <wingdi.h>
//Direct
void DirectEllipse(HDC hdc , int xc,int yc,int a,int b,COLORREF c){
    int a2 = a*a ;
    int b2 = b*b ;
    if(a > b){
        int x = 0 ;
        double y = b;
        SetPixel(hdc,xc+x,yc+Round(y),c);
        SetPixel(hdc,xc+x,yc-Round(y),c);
        SetPixel(hdc,xc-x,yc+Round(y),c);
        SetPixel(hdc,xc-x,yc-Round(y),c);
        while (x < a)
        {
            x++;
            y = sqrt((double)b2 - ((double)((double)b2/a2)*((double)x*x)));
            SetPixel(hdc,xc+x,yc+Round(y),c);
            SetPixel(hdc,xc+x,yc-Round(y),c);
            SetPixel(hdc,xc-x,yc+Round(y),c);
            SetPixel(hdc,xc-x,yc-Round(y),c);
        }
    }
    else{
        double x = a;
        int y = 0;
        SetPixel(hdc,xc+Round(x),yc+y,c);
        SetPixel(hdc,xc+Round(x),yc-y,c);
        SetPixel(hdc,xc-Round(x),yc+y,c);
        SetPixel(hdc,xc-Round(x),yc-y,c);
        while (y < b)
        {
            y++;
            x = sqrt((double)a2 - ((double)((double)a2/b2)*(y*y)));
            SetPixel(hdc,xc+Round(x),yc+y,c);
            SetPixel(hdc,xc+Round(x),yc-y,c);
            SetPixel(hdc,xc-Round(x),yc+y,c);
            SetPixel(hdc,xc-Round(x),yc-y,c);
        }

    }
    
}



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

