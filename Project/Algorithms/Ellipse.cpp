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
void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc - x, yc - y, c);
}
void EllipseMidPoint(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
    
    // point at which |slope| = 1
    int stoppingX = (a * a) / sqrt((a * a) + (b * b));
    int stoppingY = (b * b) / sqrt((a * a) + (b * b)); 
    
    int d = b*b - a*a * b;
    int d1 = 3 * b*b;
    int d2 = 3 * b*b + a*a *(2 - 2 * b);
    int d11 = 2 * b*b;
    int d12 = 2 * b*b;
    int d21 = 2 * b*b;
    int d22 = 2 * b*b + 2 * a*a;
    
    int x = 0, y = b;

    while (x <= stoppingX) {
        Draw4Points(hdc, xc, yc, x, y, c);
        if (d < 0) {
            ++x;
            d += d1;
            d1 += d11;
            d2 += d21;
        } else {
            ++x, --y;
            d += d2;
            d1 += d12;
            d2 += d22;
        }
    }

    x = a, y = 0;

    int tmp = a;
    a = b;
    b = tmp;

    // copy-paste from above
    d = b*b - a*a * b;
    d1 = 3 * b*b;
    d2 = 3 * b*b + a*a *(2 - 2 * b);
    d11 = 2 * b*b;
    d12 = 2 * b*b;
    d21 = 2 * b*b;
    d22 = 2 * b*b + 2 * a*a;
    

    while (y <= stoppingY) {
        Draw4Points(hdc, xc, yc, x, y, c);
        if (d < 0) {
            ++y;
            d += d1;
            d1 += d11;
            d2 += d21;
        } else {
            --x, ++y;
            d += d2;
            d1 += d12;
            d2 += d22;
        }
    }   
}