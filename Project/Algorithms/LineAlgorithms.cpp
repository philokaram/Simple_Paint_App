#include <Windows.h>
#include "utility.cpp"

//Direct
void DirectLine(HDC hdc,int x1, int y1, int x2,int y2, COLORREF c){
    if(abs(x2 - x1) > abs(y2 - y1)){
        if (x1 > x2)
        {
            swap(x1,y1,x2,y2);
        }
        
        int dx = x2 - x1 , dy = y2 - y1 ;
        int x = x1;
        double y = y1;
        double m = (double)dy/dx;
        SetPixel(hdc,x,y1,c);
        while (x < x2)
        {
            x++;
            y = y1 + (m*(x - x1));
            SetPixel(hdc,x,Round(y),c);
        }
    }
    else{
        if (y1 > y2)
        {
            swap(x1,y1,x2,y2);
        }
        
        int dx = x2 - x1 , dy = y2 - y1 ;
        double x = x1;
        int y = y1;
        double mi = (double)dx/dy;
        SetPixel(hdc,x1,y,c);
        while (y < y2)
        {
            y++;
            x = x1 + (mi*(y - y1));
            SetPixel(hdc,Round(x),y,c);
        }
    } 
}


//DDA




//Midpoint



//parametric


