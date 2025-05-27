#include <Windows.h>
#include "../Utility.cpp"

/*

//the given
    1. point1: x1 , y1
    2. point2: x2 , y2
    3. point2: x3 , y3
    4. point2: x4 , y4

//the equations 
    1. x  = a1 * t^3  + b1 * t^2  + g1 * t + d1   
    2. y  = a2 * t^3  + b2 * t^2  + g2 * t + d2   
    3. x' = 3 * a1 * t^2  + 2 * b1 * t  + g1 
    4. y' = 3 * a2 * t^2  + 2 * b2 * t  + g2 

    a1 = -1 * x1 + 3 * x2 - 3 * x3 + x4
    b1 =  3 * x1 - 6 * x2 + 3 * x3 
    g1 = -3 * x1 + 3 * x2
    d1 = x1
     _  _     _              _   _  _
    |    |   |                | |    |
    | a1 |   | -1   3  -3   1 | | x1 |
    | b1 | = |  3  -6   3   0 | | x2 |
    | g1 |   | -3   3   0   0 | | x3 |
    | d1 |   |  1   0   0   0 | | x4 |
    |_  _|   |_              _| |_  _|
*/
void BezierCurve(HDC hdc, int x1 , int y1 , int x2 , int y2,int x3 , int y3 ,int x4, int y4,COLORREF c){

}

// recursive bezier curve

Point BezierValue(double t,Point * points, int start , int end){
    if (start == end)
    {
        return points[start];
    }
    return (1-t) * BezierValue(t,points,start,end-1) + t * BezierValue(t,points,start+1,end);
    
}
void BezierCurveUsingRecursion(HDC hdc, int x1 , int y1 , int x2 , int y2,int x3 , int y3 ,int x4, int y4,COLORREF c){
    Point points[4] = {Point(x1,y1),Point(x2,y2),Point(x3,y3),Point(x4,y4)};
    double step = 1.0/ 1000;
    for (double t = 0; t <= 1; t += step)
    {
        Point p = BezierValue(t,points,0,4);
        SetPixel(hdc,Round(p.x),Round(p.y),c);
    }
     
}


//midpoint bezier curve
double distance(Point p1,Point p2){
    return sqrt(pow(p1.x - p2.x ,2) + pow(p1.y - p2.y , 2));
}

void MidpointBezierCurve(HDC hdc ,Point p1,Point p2,Point p3,Point p4,COLORREF c){
    if(distance(p1,p4) <= 1) return;
    Point q1 = 0.5 *(p2 - p1);
    Point q2 = 0.5 *(p3 - p2);
    Point q3 = 0.5 *(p4 - p3);
    Point r1 = 0.5 *(q2 - q1);
    Point r2 = 0.5 *(q3 - q2);
    Point midpoint = 0.5 *(r2 - r1);
    SetPixel(hdc,Round(midpoint.x),Round(midpoint.y),c);
    MidpointBezierCurve(hdc,p1,q1,r1,midpoint,c);
    MidpointBezierCurve(hdc,midpoint,r2,q3,p4,c);
}