#include<Windows.h>
//point 




//line 
OutCode GetOutCode(Point p ,int xLeft , int xRight, int yTop, int yBottom){
    OutCode result;
    result.all = 0;
    if(p.x < xLeft) result.left = 1;
    if(p.x > xRight) result.right = 1;
    if(p.y < yTop) result.top = 1;
    if(p.y > yBottom) result.bottom = 1;
    return result;
}

Point vIntersection(Point p1, Point p2 ,int xEdge ){
    // if(p1.y > p2.y) swap(p1,p2);
    Point result;
    result.x = xEdge;
    result.y = p1.y +(xEdge - p1.x) * ((double)(p2.y - p1.y) /(p2.x - p1.x));
    return result;
}
Point hIntersection(Point p1, Point p2 ,int yEdge ){
    // if(p1.x < p2.x) swap(p1,p2);
    Point result;
    result.y = yEdge;
    result.x = p1.x + (yEdge - p1.y) * ((double)(p2.x - p1.x) /(p2.y - p1.y));
    return result;
}

bool LineRectangleClipping(Point &p1 , Point &p2,int xLeft , int xRight, int yTop, int yBottom){
    OutCode out1 = GetOutCode(p1,xLeft,xRight,yTop,yBottom);
    OutCode out2 = GetOutCode(p2,xLeft,xRight,yTop,yBottom);
    while(true){
        if (out1.all == 0 && out2.all == 0) return false;
        else if((out1.all & out2.all) != 0)   return true;
        else if(out1.all){
            if (out1.left)        p1 = vIntersection(p1,p2,xLeft);
            else if (out1.right)  p1 = vIntersection(p1,p2,xRight);
            else if (out1.top)    p1 = hIntersection(p1,p2,yTop);
            else if (out1.bottom) p1 = hIntersection(p1,p2,yBottom);
            out1 = GetOutCode(p1,xLeft,xRight,yTop,yBottom);
        }
        else{
            if (out2.left)        p2 = vIntersection(p1,p2,xLeft);
            else if (out2.right)  p2 = vIntersection(p1,p2,xRight);
            else if (out2.top)    p2 = hIntersection(p1,p2,yTop);
            else if (out2.bottom) p2 = hIntersection(p1,p2,yBottom);
            out2 = GetOutCode(p2,xLeft,xRight,yTop,yBottom);
        }
    }
    
}

//polygon
