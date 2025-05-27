#include <Windows.h>
#include <stdio.h>
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




//non recursive


