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

void CirclePolarEff(HDC hdc, int xc, int yc, int r, COLORREF c) {

    double x = r;
    double y = 0;
    double dTheta = 1.0/r;

    DrawPoints(hdc, xc, yc, x, y, c);

    while (x > y) {
        int xtemp = x;
        x = x * cos(dTheta) - y * sin(dTheta);
        y = xtemp * sin(dTheta) + y * cos(dTheta);

        DrawPoints(hdc, xc, yc, round(x), round(y), c);
    }


}






//Iterative Polar

void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF c) {
    double x = R, y = 0;
    double dTheta = 1.0 / R;
    double cdt = cos(dTheta), sdt = sin(dTheta);

    DrawPoints(hdc, xc, yc, (int)round(x), (int)round(y), c);
    while (x > y) {
        double xTemp = x * cdt - y * sdt;
        y = y * cdt + x * sdt;
        x = xTemp;
        DrawPoints(hdc, xc, yc, (int)round(x), (int)round(y), c);
    }
}




//Midpoint 

void MidpointCircle(HDC hdc, int xc , int yc, int r , COLORREF c){
    int x = 0, y = r ,d;
    DrawPoints(hdc,xc,yc,x,y,c);
    while(x < y){
        d = pow(x + 1, 2) + pow(y - 0.5, 2) - r * r;
        if(d > 0) y--;
        x++;
        DrawPoints(hdc,xc,yc,x,y,c);
    }
}




//Modified Midpoint 1

void ModifiedMidpointCircle1(HDC hdc, int xc , int yc, int r , COLORREF c){
    int x = 0, y = r ,d = 1 -r;
    DrawPoints(hdc,xc,yc,x,y,c);
    while(x < y){
        if(d < 0){
            d += 2*x +3;
        }
        else{
            d += 2*(x-y) +5;
            y--;
        }
        x++;
        DrawPoints(hdc,xc,yc,x,y,c);
    }
}



//Modified Midpoint 2 


void ModifiedMidpointCircle2(HDC hdc, int xc , int yc, int r , COLORREF c){
    int x = 0, y = r ,d = 1 - r , d1 = 3 , d2 = 5 - 2*r ;
    DrawPoints(hdc,xc,yc,x,y,c);
    while(x < y){
        if(d < 0){
            d +=d1;
            d2 += 2;
        }
        else{
            d += d2;
            d2 += 4;
            y--;
        }
        d1 += 2;
        x++;
        DrawPoints(hdc,xc,yc,x,y,c);
    }
}