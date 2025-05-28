#include<Windows.h>
//point 

bool PointCircleClipping(int x, int y,int xc,int yc, int r){
    return pow(x - xc,2) + pow(y -yc ,2) - (r*r) > 0 ;
}


//line