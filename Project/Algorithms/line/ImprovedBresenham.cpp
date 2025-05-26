#include<Windows.h>
void ImprovedBresenhamLine(HDC hdc, int x1, int y1 , int x2 , int y2 , COLORREF c){
    if(x2 < x1){
			int tempx = x1;
			int tempy = y1;
			x1 = x2;
			y1 = y2;
			x2 =  tempx;
			y2 =  tempy;
    }
    int dx = x2 - x1 ; 
    int dy = y2 - y1 ; 
    if(abs(dx) > abs(dy)){

        int d = dx- (2*dy);
        int d1 = (-2)*dy;
        int d2 = 2*(dx-dy);
        if(y2 >= y1){
            int x = x1;
            int y = y1;
            while(x < x2){
                if(d <= 0){
                    d +=d2;
                    y++;
                }else{
                    d+= d1;
                }
                x++;
                SetPixel(hdc,x,y,c);
            }
        }else{
            int x = x1;
            int y = y1;
            while(x < x2){
                if(d >= 0){
                    d += d2;
                    y--;
                }else{
                    d += d1;
                }
                x++;
                SetPixel(hdc,x,y,c);
            }
    
        }
    }else{
        int d = dy- (2*dx);
        int d1 = (-2)*dx;
        int d2 = 2*(dy-dx);
        if(y2 >= y1){
            int x = x1;
            int y = y1;
            while(y < y2){
                if(d <= 0){
                    d += d2;
                    x++;
                }else{
                    d += d1;
                }
                y++;
                SetPixel(hdc,x,y,c);
            }
        }else{
            int x = x1;
            int y = y1;
            while(y > y2){
                if(d >= 0){
                    d += d2;
                    x++;
                }else{
                    d += d1;
                }
                y--;
                SetPixel(hdc,x,y,c);
            }
    
        }
    }
}