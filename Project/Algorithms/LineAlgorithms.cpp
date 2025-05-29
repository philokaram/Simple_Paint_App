#include <Windows.h>
int max(int e1,int e2){
    if(e1 > e2)
        return e1;
    return e2;
}
//Direct
void DirectLine(HDC hdc,int x1, int y1, int x2,int y2, COLORREF c){
    if(abs(x2 - x1) > abs(y2 - y1)){
        if (x1 > x2)
        {
            swap(x1,y1,x2,y2);
        }
        
        int dx = x2 - x1 , dy = y2 - y1 ;
        int x = x1;
        double y = y1;
        double m = (double)dy/dx;
        SetPixel(hdc,x,y1,c);
        while (x < x2)
        {
            x++;
            y = y1 + (m*(x - x1));
            SetPixel(hdc,x,Round(y),c);
        }
    }
    else{
        if (y1 > y2)
        {
            swap(x1,y1,x2,y2);
        }
        
        int dx = x2 - x1 , dy = y2 - y1 ;
        double x = x1;
        int y = y1;
        double mi = (double)dx/dy;
        SetPixel(hdc,x1,y,c);
        while (y < y2)
        {
            y++;
            x = x1 + (mi*(y - y1));
            SetPixel(hdc,Round(x),y,c);
        }
    } 
}


//DDA

void BresenhamsEfficientDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int x = x1;
	int y = y1;

    double m = (double)dy / dx;

	//int d1 = (dx * (2 * y + 1 - 2 * y1)) - (2 * dy * (x + 1 - x1));
    int d1 = dx - 2*dy;
	int d1c1 = -2 * dy;
	int d1c2 = 2 * (dx - dy);

    int d2 = 2*dx - dy;
    int d2c1 = 2*dx;
    int d2c2 = 2 * (dx - dy);

	SetPixel(hdc, x, y, c);

	if(m <= 1) {
        if (x2 >= x1 && y2 >= y1) {
            while(x < x2) {
                if(d1 > 0) {
                    x++;
                    d1 += d1c1;
                } else {
                    x++;
                    y++;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if(x2 < x1 && y2 > y1) {
            while(x > x2) {
                if(d1 > 0) {
                    x--;
                    d1 += d1c1;
                } else {
                    x--;
                    y++;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 > x1 && y2 < y1) {
            while(x < x2) {
                if(d1 > 0) {
                    x++;
                    d1 += d1c1;
                } else {
                    x++;
                    y--;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 <= x1 && y2 <= y1) {
            while(x > x2) {
                if(d1 > 0) {
                    x--;
                    d1 += d1c1;
                } else {
                    x--;
                    y--;
                    d1 += d1c2;
                }
                SetPixel(hdc, x, y, c);
            }
        }

    } else {
        if (x2 >= x1 && y2 >= y1) {
            while(y < y2) {
                if(d2 < 0) {
                    y++;
                    d2 += d2c1;
                } else {
                    x++;
                    y++;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if(x2 < x1 && y2 > y1) {
            while(y < y2) {
                if(d2 < 0) {
                    y++;
                    d2 += d2c1;
                } else {
                    x--;
                    y++;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 > x1 && y2 < y1) {
            while(y > y2) {
                if(d2 < 0) {
                    y--;
                    d2 += d2c1;
                } else {
                    x++;
                    y--;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }

        } else if (x2 <= x1 && y2 <= y1) {
            while(y > y2) {
                if(d2 < 0) {
                    y--;
                    d2 += d2c1;
                } else {
                    x--;
                    y--;
                    d2 += d2c2;
                }
                SetPixel(hdc, x, y, c);
            }
        }
    }
}




//Midpoint

void DrawLineBresenham(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c) {
    int dx = xe - xs;
    int dy = ye - ys;

    bool steep = abs(dy) > abs(dx);

    // If steep, swap x and y coordinates
    if (steep) {
        int temp = xs; xs = ys; ys = temp;
        temp = xe; xe = ye; ye = temp;
        dx = xe - xs;
        dy = ye - ys;
    }
    if (dx < 0) {
        int temp = xs; xs = xe; xe = temp;
        temp = ys; ys = ye; ye = temp;
        dx = -dx;
        dy = -dy;
    }
    int x = xs, y = ys;
    int d = dx - 2 * abs(dy);
    int d1 = 2 * dx - 2 * abs(dy);
    int d2 = -2 * abs(dy);
    int y_step = (dy > 0) ? 1 : -1;
    if (steep) {
        SetPixel(hdc, y, x, c);
    }
    else {
        SetPixel(hdc, x, y, c);
    }
    while (x < xe) {
        if (d < 0) {
            y += y_step;
            d += d1;
        }
        else {
            d += d2;
        }
        x++;
        if (steep) {
            SetPixel(hdc, y, x, c);
        }
        else {
            SetPixel(hdc, x, y, c);
        }
    }
}

//parametric

void ParametricLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int alpha1 = x2 - x1;
    int alpha2 = y2 - y1;
    int x, y;
    double step = 1.0 / max(abs(alpha1), abs(alpha2));
    for (double t = 0; t <= 1; t += step) {
        x = alpha1 * t + x1;
        y = alpha2 * t + y1;
        SetPixel(hdc, round(x), round(y), c);
    }
}
