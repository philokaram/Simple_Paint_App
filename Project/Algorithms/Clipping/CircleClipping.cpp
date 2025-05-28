#include<Windows.h>
//point 

bool PointCircleClipping(int x, int y,int xc,int yc, int r){
    return pow(x - xc,2) + pow(y -yc ,2) - (r*r) > 0 ;
}


//line
void DrawLineClippedByCircle(HDC hdc, int x1, int y1, int x2, int y2, int xc, int yc, int r, COLORREF c) {
    std::cout << "drawing line clipped by circle\n";
    if (abs(x2 - x1) > abs(y2 - y1)) {
        if (x1 > x2) swap(x1, y1, x2, y2);

        int dx = x2 - x1, dy = y2 - y1;
        double m = (double)dy / dx;
        int x = x1;
        double y = y1;

        while (x <= x2) {
            int dxp = x - xc;
            int dyp = Round(y) - yc;
            if (dxp * dxp + dyp * dyp <= r * r) {
                SetPixel(hdc, x, Round(y), c);
            }
            x++;
            y = y1 + m * (x - x1);
        }
    } else {
        if (y1 > y2) swap(x1, y1, x2, y2);

        int dx = x2 - x1, dy = y2 - y1;
        double m = (double)dx / dy;
        int y = y1;
        double x = x1;

        while (y <= y2) {
            int dxp = Round(x) - xc;
            int dyp = y - yc;
            if (dxp * dxp + dyp * dyp <= r * r) {
                SetPixel(hdc, Round(x), y, c);
            }
            y++;
            x = x1 + m * (y - y1);
        }
    }
}
