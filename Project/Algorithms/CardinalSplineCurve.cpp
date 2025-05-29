#include<Windows.h>
#include "temp/HermiteCurve.cpp"


//Cardinal Spline Curve

void CardinalSplineCurve(HDC hdc, std::vector<Point> points,int size,COLORREF c){
    Point t0 = (points[2] - points[0]);
    for (int i = 2; i < size-1; i++)
    {
        // std::cout<<"\np2.x: "<<points[i+1].x<<" p2.y: "<<points[i+1].y<<" p2.x: "<<points[i-1].x<<" p2.y: "<<points[i-1].y;
        Point t1 = (points[i+1] - points[i-1]);
        // std::cout<<"\nt0.x: "<<t0.x<<" t0.y: "<<t0.y<<" t1.x: "<<t1.x<<" t1.y: "<<t1.y;
        HermiteCurve(hdc,points[i-1].x,points[i-1].y,points[i].x,points[i].y,t0.x,t0.y,t1.x,t1.y,c);
        t0 = t1;
    }
    
}