#include "../temp/BezierCurve.cpp"
//Filling Circle with lines after taking filling quarter from user








//Filling Circle with other circles after taking filling quarter from user

void CircleQuarterFilling(HDC hdc,int xc,int yc,int r,int xf,int yf,COLORREF fillingColor){
    r--;
    int signX = 1;
    int signY = -1;
    if(xf > xc && yf < yc){
        signX = 1;
        signY = -1;
    }
    else if(xf < xc && yf < yc){
        signX = -1;
        signY = -1;
    }
    else if(xf < xc && yf > yc){
        signX = -1;
        signY = 1;
    }
    else if(xf > xc && yf > yc){
        signX = 1;
        signY = 1;
    }
    while (r > 0)
    {
        int x = 0, y = r ,d = 1 - r , d1 = 3 , d2 = 5 - 2*r ;
        SetPixel(hdc,xc+(signX*x),yc+(signY*y),fillingColor);
        SetPixel(hdc,xc+(signX*y),yc+(signY*x),fillingColor);
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
            SetPixel(hdc,xc+(signX*x),yc+(signY*y),fillingColor);
            SetPixel(hdc,xc+(signX*y),yc+(signY*x),fillingColor);
    }
    r--;
}
}





//Filling Square with Hermit Curve [Vertical]



// Filling Rectangle with Bezier Curve [horizontal]

void RectangleFillWithBezierCurveFunction(HDC hdc, int xLeft,int xRight,int yTop, int yBottom,COLORREF fillingColor){
    for (int y = yTop; y <= yBottom; y++)
    {
        BezierCurve(hdc,xLeft,y,xLeft,y,xRight,y,xRight,y,fillingColor);
    }
    
}
