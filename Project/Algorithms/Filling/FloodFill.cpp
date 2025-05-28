#include <Windows.h>
#include <stdio.h>
#include <stack>
//recursive
void RecursiveFloodFill(HDC hdc,int x, int y,COLORREF backgroundColor,COLORREF fillingColor){
    COLORREF currentColor = GetPixel(hdc, x, y);
    if (currentColor == fillingColor || currentColor != backgroundColor ) return;
    SetPixel(hdc,x,y,fillingColor);
    RecursiveFloodFill(hdc,x + 1 , y ,backgroundColor,fillingColor );  
    RecursiveFloodFill(hdc,x - 1 , y ,backgroundColor,fillingColor );  
    RecursiveFloodFill(hdc,x  , y + 1 ,backgroundColor,fillingColor );  
    RecursiveFloodFill(hdc,x  , y - 1 ,backgroundColor,fillingColor );  
}
void RecursiveFloodFill(HDC hdc,int x, int y,COLORREF fillingColor) {
    COLORREF backgroundColor = GetPixel(hdc, x, y);
    RecursiveFloodFill(hdc,x,y,backgroundColor,fillingColor);
}




//non recursive
void NonRecursiveFloodFill(HDC hdc,int x, int y,COLORREF fillingColor) {
    COLORREF backgroundColor = GetPixel(hdc, x, y);
    std::stack<Point> s;
    s.push(Point(x, y));
    while (!s.empty())
    {
        Point p = s.top();
        s.pop();
        COLORREF currentColor = GetPixel(hdc, p.x, p.y);
        if (currentColor == fillingColor || currentColor != backgroundColor ) continue;
        SetPixel(hdc,p.x,p.y,fillingColor);
        s.push(Point(p.x + 1 , p.y));
        s.push(Point(p.x - 1 , p.y));
        s.push(Point(p.x , p.y + 1));
        s.push(Point(p.x, p.y - 1));
    }
    
}

