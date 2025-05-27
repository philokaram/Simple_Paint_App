#include <Windows.h>
/*

//the given
    1. point1: x1 , y1
    2. point2: x2 , y2
    3. slope1: u1 , v1
    4. slope2: u2 , v2

//the equations 
    1. x  = a1 * t^3  + b1 * t^2  + g1 * t + d1   
    2. y  = a2 * t^3  + b2 * t^2  + g2 * t + d2   
    3. x' = 3 * a1 * t^2  + 2 * b1 * t  + g1 
    4. y' = 3 * a2 * t^2  + 2 * b2 * t  + g2 

    a1 = 2  * x1 +     u1 - 2 * x2 + u2
    b1 = -3 * x1 - 2 * u1 + 3 * x2 - u2
    g1 = u1
    d1 = x1
     _  _     _              _   _  _
    |    |   |                | |    |
    | a1 |   |  2   1  -2   1 | | x1 |
    | b1 | = | -3  -2   3  -1 | | u1 |
    | g1 |   |  0   1   0   0 | | x2 |
    | d1 |   |  1   0   0   0 | | u2 |
    |_  _|   |_              _| |_  _|
*/
void HermiteCurve(HDC hdc, int x1 , int y1 , int x2 , int y2,int u1 , int v1 ,int u2, int v2,COLORREF c){

}