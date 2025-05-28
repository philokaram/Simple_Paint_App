#include<Windows.h>
//point 

bool PointCircleClipping(int x, int y,int xc,int yc, int r){
    return (xc*xc) + (yc*yc) - (r*r) > 0 ;
}


//line