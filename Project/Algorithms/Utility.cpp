

struct Point
{
    double x,y;
    Point(double x = 0.0, double y = 0.0):x(x),y(y) {}
};
//Utility
void swap(int &x1,int &y1, int &x2, int &y2){
    int tempX = x1, tempY = y1;
    x1 = x2;
    y1 = y2;
    x2 = tempX;
    y2 = tempY;
}

void swap(Point &p1,Point &p2){
    Point temp(p1.x,p1.y);
    p1.x = p2.x;
    p1.y = p2.y;
    p2.x = temp.x;
    p2.y = temp.y;
}

int Round(double e){
    return (int) (e+0.5);
}


Point operator*(double d, Point p){
    return Point(p.x *d, p.y *d);
}

Point operator+(Point p1, Point p2){
    return Point(p1.x + p2.x, p1.y + p2.y);
}
Point operator-(Point p1, Point p2){
    return Point(p1.x - p2.x, p1.y - p2.y);
}
