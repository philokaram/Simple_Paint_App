
//Utility
void swap(int &x1,int &y1, int &x2, int &y2){
    int tempX = x1, tempY = y1;
    x1 = x2;
    y1 = y2;
    x2 = tempX;
    y2 = tempY;
}

int Round(double e){
    return (int) (e+0.5);
}
