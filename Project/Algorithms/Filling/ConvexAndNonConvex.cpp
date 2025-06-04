#include<list>
#include<Windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
using namespace std;

//Convex
const int MAX_SCANLINE_HEIGHT = 1080;
const int POS_INF_INT = INT_MAX;
const int NEG_INF_INT = INT_MIN;

struct ScanlineEntry
{
    int xLeft;
    int xRight;
    ScanlineEntry() : xLeft(POS_INF_INT), xRight(NEG_INF_INT) {}
};

ScanlineEntry g_edgeTable[MAX_SCANLINE_HEIGHT];
vector<POINT> g_polygonVertices;
bool g_drawingPolygon = true;

void DrawPixel(HDC hdc, int x, int y, COLORREF color)
{
    if (x >= 0 && y >= 0 && x < GetDeviceCaps(hdc, HORZRES) && y < GetDeviceCaps(hdc, VERTRES))
    {
        SetPixel(hdc, x, y, color);
    }
}
void DrawHorizontalLine(HDC hdc, int x1, int x2, int y, COLORREF color)
{
    if (x1 > x2)
    {
        swap(x1, x2);
    }
    for (int x = x1; x <= x2; ++x)
    {
        DrawPixel(hdc, x, y, color);
    }
}

void InitEdgeTable()
{
    for (int i = 0; i < MAX_SCANLINE_HEIGHT; ++i)
    {
        g_edgeTable[i] = ScanlineEntry();
    }
}

void EdgeToTable(POINT p1, POINT p2)
{
    if (p1.y > p2.y)
    {
        swap(p1, p2);
    }
    if (p1.y == p2.y)
    {
        return;
    }
    double dx_dy = static_cast<double>(p2.x - p1.x) / (p2.y - p1.y);
    for (int y = p1.y; y < p2.y; ++y)
    {
        if (y >= 0 && y < MAX_SCANLINE_HEIGHT)
        {
            double x_intersect = static_cast<double>(p1.x) + (y - p1.y) * dx_dy;
            int current_x_left = static_cast<int>(ceil(x_intersect));
            int current_x_right = static_cast<int>(floor(x_intersect));
            g_edgeTable[y].xLeft = min(g_edgeTable[y].xLeft, current_x_left);
            g_edgeTable[y].xRight = max(g_edgeTable[y].xRight, current_x_right);
        }
    }
}

void TableToScreen(HDC hdc, COLORREF fillColor)
{
    int minY = POS_INF_INT;
    int maxY = NEG_INF_INT;
    if (!g_polygonVertices.empty())
    {
        minY = g_polygonVertices[0].y;
        maxY = g_polygonVertices[0].y;
        for (const auto& p : g_polygonVertices)
        {
            minY = min<int>(minY, p.y);
            maxY = max<int>(maxY, p.y);
        }
    }
    else
    {
        return;
    }
    minY = max(0, minY);
    maxY = min(MAX_SCANLINE_HEIGHT - 1, maxY);
    for (int y = minY; y <= maxY; ++y)
    {
        if (g_edgeTable[y].xLeft != POS_INF_INT &&
            g_edgeTable[y].xRight != NEG_INF_INT)
        {
            DrawHorizontalLine(hdc, g_edgeTable[y].xLeft, g_edgeTable[y].xRight, y, fillColor);
        }
    }
}

void FillConvexPolygon(HDC hdc, const vector<POINT>& vertices, COLORREF fillColor)
{
    if (vertices.empty() || vertices.size() < 3)
    {
        return;
    }
    InitEdgeTable();
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        POINT p1 = vertices[i];
        POINT p2 = vertices[(i + 1) % vertices.size()];
        EdgeToTable(p1, p2);
    }
    TableToScreen(hdc, fillColor);
}





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