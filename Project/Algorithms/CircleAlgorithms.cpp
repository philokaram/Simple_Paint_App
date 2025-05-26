#include<Windows.h>

//Utility

void DrawPoints(HDC hdc, int xc ,int yc ,int x ,int y ,COLORREF c){
    SetPixel(hdc,xc+x,yc+y,c);
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc-x,yc-y,c);
    SetPixel(hdc,xc+y,yc+x,c);
    SetPixel(hdc,xc+y,yc-x,c);
    SetPixel(hdc,xc-y,yc+x,c);
    SetPixel(hdc,xc-y,yc-x,c);
    
}

//Direct
void DirectCircle(HDC hdc , int xc , int yc ,int r ,COLORREF c){
    int x = 0;
    double y = r;
    DrawPoints(hdc,xc,xc,x,y,c);
    while (x < y)
    {
        y = sqrt((r*r) -(x*x));
        DrawPoints(hdc,xc,yc,x,(int)(y+0.5),c);
        x++;
    }
    
}


//Polar







//Iterative Polar






//Midpoint 






//Modified Midpoint





