#include<Windows.h>
void SimpleLine(HDC hdc,int x1, int y1, int x2, int y2,COLORREF c){
    if(x2 < x1){
        int tempx = x1;
        int tempy = y1;
        x1 = x2;
        y1 = y2;
        x2 =  tempx;
        y2 =  tempy;
    }
    if(abs(x2-x1) > abs(y2-y1)){
        int x = x1;
        double y = y1;
        double slope = (double)(y2-y1) / (double) (x2-x1);
        while (x < x2)
        {
            y= y1 + slope*(x-x1);
            SetPixel(hdc,x,Round(y),c);
            x++;
        }
        
    }else{
        double x = x1;
        int y = y1;
        double slope = (double) (x2-x1) /(double)(y2-y1) ;
        int add = 1;
        if(y2< y1){
            add = -1;
        }
        while (x < x2)
        {
            x= x1 + slope*(y-y1);
            SetPixel(hdc,Round(x),y,c);
            y += add;
        }

    }
    
}