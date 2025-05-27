#include<Windows.h>

//Cardinal Spline Curve

void CardinalSplineCurve(HDC hdc, Point *points,int size,COLORREF c){
    int c = 0.5;
    Point t0 = c * (points[2] - points[0]);
    for (int i = 2; i < size-1; i++)
    {
        Point t1 = c * (Points[i-1] - points[i+1]);
        HermiteCurve(hdc,points[i-1],t0,points[i],t1,c);
        t0 = t1;
    }
    
}