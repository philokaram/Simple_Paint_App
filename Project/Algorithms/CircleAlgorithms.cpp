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
    int y = r;
    int r2 = r*r;
    DrawPoints(hdc,xc,yc,x,y,c);
    while (x < y)
    {
        y = (int)(sqrt( r2 -(x*x))+0.5);
        DrawPoints(hdc,xc,yc,x,y,c);
        x++;
    }
    
}


//Polar







//Iterative Polar






//Midpoint 






//Modified Midpoint





