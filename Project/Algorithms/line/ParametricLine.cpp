#include<Windows.h>

int max(int e1, int e2){
    if(e1 > e2)
        return e1;
    return e2;
}

void ParametrecLine(HDC hdc,int x1, int y1 , int x2, int y2,COLORREF c){
    double step = 1.0/max(abs(x2-x1),abs(y2-y1));
    int alpha1 = x2 - x1;
    int alpha2 = y2 - y1;
    for (double t = 0; t <= 1; t+= step)
    {
        double x = alpha1*t +x1;
        double y = alpha2*t +y1;
        SetPixel(hdc,(int)(x+0.5),(int)(y+0.5),c);
    }
    
}