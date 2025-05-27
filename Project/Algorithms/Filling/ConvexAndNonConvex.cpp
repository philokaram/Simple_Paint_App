#include<list>
#include<Windows.h>

//Convex







//Non Convex
struct Node{
    double x;
    double mi;
    int yMax;
    Node(double x = 0.0,double mi = 0.0 , int yMax = 0):x(x),mi(mi),yMax(yMax){}
};

bool operator<(Node n1 ,Node n2){
    return n1.x < n2.x;
}


typedef  std::list<Node> edgeTable[1000];
void Edge2Table(Point p1,Point p2, edgeTable &eTable){
    if( p1.y == p2.y) return;
    if(p1.y > p2.y) swap(p1,p2);
    eTable[(int)p1.y].push_back(Node(p1.x,(double)(p2.x - p1.x) / (double)(p2.y - p1.y),(int)p2.y));
}

void Polygon2Table(Point *points,int size,edgeTable & eTable){
    Point p1 = points[size-1];
    for(int i = 0 ; i < size ; i++){
        Point p2 = points[i];
        Edge2Table(p1,p2,eTable);
        p1 = p2;
    }
}

void Table2Scree(HDC hdc,edgeTable & eTable,COLORREF fillingColor){
    int y = 0;
    std::list<Node> activeList;
    while(eTable[y].empty()) y++;
    for (std::list<Node>::iterator it = eTable[y].begin(); it != eTable[y].end();it++)
    {
        activeList.push_back(*(it));
    }    
    while(!activeList.empty()){
        activeList.sort();
        
        for (std::list<Node>::iterator it = activeList.begin(); it != activeList.end(); it++)
        {
            int x1 = floor(it->x);
            it++;
            int x2 = ceil((it)->x);
            DirectLine(hdc,x1,y,x2 , y, fillingColor);
        }

        y++;
        
        for (std::list<Node>::iterator it = activeList.begin(); it != activeList.end();)
        {
            if(it->yMax == y){
                it = activeList.erase(it);
            }else{
                it++;
            }
        }

        for (std::list<Node>::iterator it = activeList.begin(); it != activeList.end();it++)
        {
            it->x += it->mi; 
        }

        if(!eTable[y].empty()){
            for (std::list<Node>::iterator it = eTable[y].begin(); it != eTable[y].end();it++)
            {
                activeList.push_back((*it));
            }
        }
    }
}

void NonConvexFilling(HDC hdc, Point *points,int size,COLORREF fillingColor){
    edgeTable eTable;
    Polygon2Table(points,size,eTable);
    // for (int i = 0; i < 1000; i++)
    // {
    //     if(!eTable[i].empty()){
    //         for (std::list<Node>::iterator it = eTable[i].begin(); it != eTable[i].end(); it++){
    //             std::cout<<"\nY: "<<i<<" x: "<<(*it).x<<" mi: "<<(*it).mi<<" Ymax: "<<(*it).yMax;
    //         }
    //     }
    // } 
    Table2Scree(hdc,eTable,fillingColor);
}