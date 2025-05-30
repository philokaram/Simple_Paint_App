#include<Windows.h>
#include <vector>
//point 

bool PointRectangleClipping(int x, int y, int xLeft , int xRight, int yTop, int yBottom) {
    return x < xLeft || x > xRight || y < yTop || y > yBottom;
}


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

Point vIntersect(Point p1, Point p2, int x_edge) {
    Point result;

    result.x = x_edge;
    result.y = p1.y + (x_edge - p1.x) * ((double)(p2.y-p1.y)/(p2.x-p1.x));

    return result;

}

Point hIntersect(Point p1, Point p2, int y_edge) {
    Point result;

    result.y = y_edge;
    result.x = p1.x + (y_edge - p1.y) * ((double)(p2.x-p1.x)/(p2.y-p1.y));

    return result;

}


std::vector<Point> polygonClip(std::vector<Point> p, int xl, int xr, int yb, int yt) {

    int n = p.size();
    
    Point p1 = p[n-1];
    std::vector<Point> output1, output2, output3, output4;
    
    bool in1 = p1.x > xl;
    
    for (int i = 0; i < n; i++) {
        
        Point p2 = p[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.x > xl;
        
        if(in1 && in2) {
            output1.push_back(p2);
            
        } else if(in2) {
            Point newp = vIntersect(p1, p2, xl);
            output1.push_back(newp);
            output1.push_back(p2);
            
        } else if (in1) {
            Point newp = vIntersect(p1, p2, xl);
            output1.push_back(newp);
            
        }
        
        p1 = p2;
        in1 = in2;
        
    }

    in1 = p1.x < xr;
    
    n = output1.size();
    p1 = output1[n-1];
    for (int i = 0; i < n; i++) {
        
        Point p2 = output1[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.x < xr;
        
        if(in1 && in2) {
            output2.push_back(p2);
            
        } else if(in2) {
            Point newp = vIntersect(p1, p2, xr);
            output2.push_back(newp);
            output2.push_back(p2);
            
        } else if (in1) {
            Point newp = vIntersect(p1, p2, xr);
            output2.push_back(newp);
            
        }
        
        p1 = p2;
        in1 = in2;
        
    }
    
    in1 = p1.y > yb;
    
    n = output2.size();
    p1 = output2[n-1];
    for (int i = 0; i < n; i++) {
        
        Point p2 = output2[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.y > yb;
        
        if(in1 && in2) {
            output3.push_back(p2);
            
        } else if(in2) {
            Point newp = hIntersect(p1, p2, yb);
            output3.push_back(newp);
            output3.push_back(p2);
            
        } else if (in1) {
            Point newp = hIntersect(p1, p2, yb);
            output3.push_back(newp);
            
        }
        
        p1 = p2;
        in1 = in2;
        
    }
    
    in1 = p1.y < yt;
    
    n = output3.size();
    p1 = output3[n-1];
    for (int i = 0; i < n; i++) {
        
        Point p2 = output3[i];
        // cout << "in test: " << p2.x << " " << p2.y << endl;
        bool in2 = p2.y < yt;
        
        if(in1 && in2) {
            output4.push_back(p2);
            
        } else if(in2) {
            Point newp = hIntersect(p1, p2, yt);
            output4.push_back(newp);
            output4.push_back(p2);
            
        } else if (in1) {
            Point newp = hIntersect(p1, p2, yt);
            output4.push_back(newp);

        }
        
        p1 = p2;
        in1 = in2;

    }
    
    return output4;
    
    
}
