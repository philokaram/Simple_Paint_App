#include<Windows.h>
void BresenhamLine(HDC hdc, int x1, int y1 , int x2 , int y2 , COLORREF c){
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

        if(y2 >= y1){
            int x = x1;
            int y = y1;
            int d;
            while(x < x2){
                d = dx*(2*(y-y1)+1) -2*dy*(x-x1+1);
                if(d <= 0){
                    y++;
                }
                x++;
                SetPixel(hdc,x,y,c);
            }
        }else{
            int x = x1;
            int y = y1;
            int d;
            while(x < x2){
                d = dx*(2*(y-y1)-1) -2*dy*(x-x1+1);
                if(d >= 0){
                    y--;
                }
                x++;
                SetPixel(hdc,x,y,c);
            }
    
        }
    }else{
        if(y2 >= y1){
            int x = x1;
            int y = y1;
            int d;
            while(y < y2){
                d = dy*(2*(x-x1)+1) - 2*dx*(y-y1+1);
                if(d <= 0){
                    x++;
                }
                y++;
                SetPixel(hdc,x,y,c);
            }
        }else{
            int x = x1;
            int y = y1;
            int d;
            while(y > y2){
                d = dy*(2*(x-x1)+1) - 2*dx*(y-y1-1);
                if(d >= 0){
                    x++;
                }
                y--;
                SetPixel(hdc,x,y,c);
            }
    
        }
    }
}