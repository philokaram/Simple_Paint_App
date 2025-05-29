#include <Windows.h>
#include <math.h>
#include <iostream>
#include "Algorithms/Utility.cpp"
#include "Algorithms/LineAlgorithms.cpp"
#include "Algorithms/CircleAlgorithms.cpp"
#include "Algorithms/Ellipse.cpp"
#include "Algorithms/CardinalSplineCurve.cpp"
#include "Algorithms/Filling/FloodFill.cpp"
#include "Algorithms/Filling/ConvexAndNonConvex.cpp"
#include "Algorithms/Filling/FillingShapes.cpp"
#include "Algorithms/Clipping/CircleClipping.cpp"
#include "Algorithms/Clipping/RectangleClipping.cpp"
#include <vector>

int min(int e1,int e2){
    if(e1 < e2)
        return e1;
    return e2;
}
HWND hStaticLabel;
HWND hWriteButton;
HWND hEraseButton;
HWND hClearButton;
HWND hDrawLineButton;
HWND hDrawCircle;
HWND hDrawEllipse;
HWND hDrawCardinalSplineCurveButton;
HWND hFill;
HWND hShapeFillButton;
HWND hShapeFillList;
HWND hClip;
HWND hSave;
HWND hLoad;
HWND hCombo;
HWND hClipWindow;

HBRUSH hBlackBrush;

HICON hWriteCursor;
HICON hEraseCursor;
HICON hFillCursor;
HICON hClipCursor;
HICON hDrawCursor;

bool isWrite = false;
bool isErase = false;


enum Action {None,Write,Erase,Clear,DrawLine,DrawCircle,DrawEllipse,DrawCardinalSplineCurve,ShapeFill,Fill,Clip};
Action currentAction = None;

enum lineAlgorithm {DirectLineAlgorithm,DDALineAlgorithm,MidpointLineAlgorithm,ModifiedMidpointLineAlgorithm,ParametricLineAlgorithm};
int currentLineAlgorithm = DirectLineAlgorithm;

enum circleAlgorithm {DirectCircleAlgorithm,PolarCircleAlgorithm,IterativePolarCircleAlgorithm,MidpointCircleAlgorithm,ModifiedMidpointCircleAlgorithm1,ModifiedMidpointCircleAlgorithm2};
int currentCircleAlgorithm = DirectCircleAlgorithm;

enum EllipseAlgorithm {DirectEllipseAlgorithm,PolarEllipseAlgorithm,MidpointEllipseAlgorithm};
int currentEllipseAlgorithm = DirectEllipseAlgorithm;

enum fillingAlgorithms {RecursiveFloodFillAlgorithms,NonRecursiveFloodFillAlgorithms,ConvexFillAlgorithms,NonConvexFillAlgorithms};
int currentFillAlgorithm = RecursiveFloodFillAlgorithms;

enum clippingAlgorithms {PointClipping,LineClipping,PolygonClipping};
int currentClipAlgorithm = PointClipping;

enum clipWindowShape{RectangleWindow,SquareWindow,CircleWindow};
int currentClipWindowShape = RectangleWindow;

enum shapeFileAlgorithm {CircleFillWithLines,CircleFillWithCircles,SquareFillWithHermiteCurve,RectangleFillWithBezierCurve};
int currentShapeFillAlgorithm = CircleFillWithLines;

enum buttonsID {comboListId,drawLineButtonId,drawCircleButtonId,fillButtonId,clipButtonId,comboClipWindowId,writeButtonId,eraseButtonId,clearButtonId,drawEllipseButtonId,drawCardinalSplineCurveButtonId,saveButtonId,loadButtonId,shapeFillButtonId,shapeFillListId};
//1. change Background
COLORREF backgroundColor = RGB(255, 255, 255); 

// top bar
RECT topBar = {0,0,400,200};
COLORREF topBarColor = RGB(50,50,50);

//background box
RECT backgroundBox = {0,5,0,0};
COLORREF backgroundBoxColor = RGB(255,255,255);

RECT backgroundColorBoxes[5] = {
    /*RECT = {left,top,right,bottom}*/
    {20, 50, 70, 70},      
    {80, 50, 130, 70},     
    {140, 50, 190, 70},   
    {200, 50, 250, 70},     
    {200, 50, 250, 70}     
};

COLORREF backgroundBoxColors[5] = {
    RGB(255, 0, 0),        // Red
    RGB(0, 255, 0),        // Green
    RGB(0, 0, 255),        // Blue
    RGB(0 , 0, 0),       //Black
    RGB(255 , 255, 255)       //Black
};


//shape color box
RECT shapeColorBox = {0,5,0,0};
COLORREF shapeColorBoxColor = RGB(255,255,255);
COLORREF shapeColor = RGB(0,0,0);

RECT shapeColorBoxes[5] = {
    /*RECT = {left,top,right,bottom}*/
    {20, 50, 70, 70},      
    {80, 50, 130, 70},     
    {140, 50, 190, 70},   
    {200, 50, 250, 70},     
    {200, 50, 250, 70}     
};

COLORREF shapeBoxColors[5] = {
    RGB(255, 0, 0),        // Red
    RGB(0, 255, 0),        // Green
    RGB(0, 0, 255),        // Blue
    RGB(0 , 0, 0),       //Black
    RGB(255 , 255, 255)       //Black
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
	HDC hdc;
    PAINTSTRUCT ps;
    static int x1,x2,y1,y2,x3,y3,xc1,yc1,xc2,yc2,r,count = 0,a,b,polygon_points_count, xl, xr, yb, yt;
    static std::vector<Point> p = {0, 0, 0, 0, 0};
    static int  windowWidth,windowHight;
	switch (m)
	{

    case WM_CREATE:
    {
        // Create a combo box (drop-down list)
        hCombo = CreateWindowEx(
            0,                    // Extended style
            TEXT("COMBOBOX"),           // Class name
            NULL,                 // No window title
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
            260, 10, 200, 300,    // x, y, width, height
            hwnd,                 // Parent window handle
            (HMENU)comboListId,             // Control ID
            ((LPCREATESTRUCT)lp)->hInstance,
            NULL
        );
        // Optionally, set default selected item

        SendMessage(hCombo, CB_SETCURSEL, 0, 0);
        ShowWindow(hCombo, SW_HIDE);
        
        hClipWindow = CreateWindowEx(
            0,                    // Extended style
            TEXT("COMBOBOX"),           // Class name
            NULL,                 // No window title
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD ,
            260, 40, 200, 300,    // x, y, width, height
            hwnd,                 // Parent window handle
            (HMENU)comboClipWindowId,             // Control ID
            ((LPCREATESTRUCT)lp)->hInstance,
            NULL
        );
        // Add items to combo box
        SendMessage(hClipWindow, CB_ADDSTRING, 0, (LPARAM)"Rectangle");
        SendMessage(hClipWindow, CB_ADDSTRING, 0, (LPARAM)"Square");
        SendMessage(hClipWindow, CB_ADDSTRING, 0, (LPARAM)"Circle");
        SendMessage(hClipWindow, CB_SETCURSEL, 0, 0);
        
        hShapeFillList = CreateWindowEx(
            0,                    // Extended style
            TEXT("COMBOBOX"),           // Class name
            NULL,                 // No window title
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD ,
            260, 70, 200, 300,    // x, y, width, height
            hwnd,                 // Parent window handle
            (HMENU)shapeFillListId,             // Control ID
            ((LPCREATESTRUCT)lp)->hInstance,
            NULL
        );
        // Add items to combo box
        SendMessage(hShapeFillList, CB_ADDSTRING, 0, (LPARAM)"Circle Fill with Lines");
        SendMessage(hShapeFillList, CB_ADDSTRING, 0, (LPARAM)"Circle Fill with Circles");
        SendMessage(hShapeFillList, CB_ADDSTRING, 0, (LPARAM)"Square Fill with Hermite Curve");
        SendMessage(hShapeFillList, CB_ADDSTRING, 0, (LPARAM)"Rectangle Fill With Bezier Curve");
        SendMessage(hShapeFillList, CB_SETCURSEL, 0, 0);
        ShowWindow(hShapeFillList, SW_HIDE);
        
        hWriteButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Wirte"),
            WS_CHILD | WS_VISIBLE | BS_ICON| BS_FLAT,
            5,5,
            40,30,
            hwnd,
            (HMENU)writeButtonId,
            nullptr,
            nullptr
        );
        hEraseButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Erase"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            5,40,
            40,30,
            hwnd,
            (HMENU)eraseButtonId,
            nullptr,
            nullptr
        );
        hClearButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Clear"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            5,73,
            40,20,
            hwnd,
            (HMENU)clearButtonId,
            nullptr,
            nullptr
        );
        hDrawLineButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Draw Line"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            50,5,
            100,30,
            hwnd,
            (HMENU)drawLineButtonId,
            nullptr,
            nullptr
        );
        hDrawCircle = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Draw Circle"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            50,40,
            100,30,
            hwnd,
            (HMENU)drawCircleButtonId,
            nullptr,
            nullptr
        );
        hDrawEllipse = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Draw Ellipse"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            50,73,
            100,20,
            hwnd,
            (HMENU)drawEllipseButtonId,
            nullptr,
            nullptr
        );
        hDrawCardinalSplineCurveButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Draw Cardinal Spline Curve"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            155,73,
            100,20,
            hwnd,
            (HMENU)drawCardinalSplineCurveButtonId,
            nullptr,
            nullptr
        );
        hFill = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Fill"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            155,5,
            100,30,
            hwnd,
            (HMENU)fillButtonId,
            nullptr,
            nullptr
        );

        hClip = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Clip"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            155,40,
            100,30,
            hwnd,
            (HMENU)clipButtonId,
            nullptr,
            nullptr
        );

        hSave = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Save"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            465, 40, 
            100, 30,
            hwnd,
            (HMENU)saveButtonId, 
            ((LPCREATESTRUCT)lp)->hInstance,
            NULL
        );
        hLoad = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Load"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            465, 5,
            100, 30,
            hwnd,
            (HMENU)loadButtonId, // Use the new ID
            ((LPCREATESTRUCT)lp)->hInstance,
            NULL
        );
        
        hShapeFillButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Shape Fill"),
            WS_CHILD | WS_VISIBLE| BS_ICON,
            465, 75,
            100, 30,
            hwnd,
            (HMENU)shapeFillButtonId, // Use the new ID
            ((LPCREATESTRUCT)lp)->hInstance,
            NULL
        );

        //icons 
         // Load icon from file 
        HICON hPencilIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/pencil.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hPencilIcon) {
            SendMessage(hWriteButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hPencilIcon);
        }
       
        HICON hEraseIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/erase.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hEraseIcon) {
            SendMessage(hEraseButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hEraseIcon);
        }

        HICON hClearIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/clear.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hClearIcon) {
            SendMessage(hClearButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hClearIcon);
        }
        HICON hDrawLineIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/line.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hDrawLineIcon) {
            SendMessage(hDrawLineButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hDrawLineIcon);
        }
        HICON hDrawCircleIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/circle.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hDrawCircleIcon) {
            SendMessage(hDrawCircle, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hDrawCircleIcon);
        }
        HICON hDrawEllipseIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/ellipse.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hDrawEllipseIcon) {
            SendMessage(hDrawEllipse, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hDrawEllipseIcon);
        }
        HICON hFillIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/fill.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hFillIcon) {
            SendMessage(hFill, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hFillIcon);
        }
        HICON hclipIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/clip.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hclipIcon) {
            SendMessage(hClip, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hclipIcon);
        }
        HICON hSaveIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/save.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hSaveIcon) {
            SendMessage(hSave, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hSaveIcon);
        }
        HICON hLoadIcon = (HICON)LoadImage(
            nullptr,
            TEXT("Icons/load.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

        if (hLoadIcon) {
            SendMessage(hLoad, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hLoadIcon);
        }

         hWriteCursor = (HICON)LoadImage(
            nullptr,
            TEXT("Cursors/write.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );
         hEraseCursor = (HICON)LoadImage(
            nullptr,
            TEXT("Cursors/erase.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );
         hFillCursor = (HICON)LoadImage(
            nullptr,
            TEXT("Cursors/fill.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );
         hClipCursor = (HICON)LoadImage(
            nullptr,
            TEXT("Cursors/clip.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );
         hDrawCursor = (HICON)LoadImage(
            nullptr,
            TEXT("Cursors/point.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            20, 20,
            LR_LOADFROMFILE
        );

		break;
    }
    case WM_PAINT: {
        //change background//
        hdc = BeginPaint(hwnd, &ps);

        // Fill background
        HBRUSH bgBrush = CreateSolidBrush(backgroundColor);
        FillRect(hdc, &ps.rcPaint, bgBrush);
        DeleteObject(bgBrush);
        
        // Draw top bar
        HBRUSH tbBrush = CreateSolidBrush(topBarColor);
        FillRect(hdc, &topBar, tbBrush);
        DeleteObject(tbBrush);
       
        // Draw background box
        HBRUSH bbBrush = CreateSolidBrush(backgroundBoxColor);
        FillRect(hdc, &backgroundBox, bbBrush);
        DeleteObject(bbBrush);
        
        // Draw background color boxes
        for (int i = 0; i < 5; ++i) {
            HBRUSH boxBrush = CreateSolidBrush(backgroundBoxColors[i]);
            FillRect(hdc, &backgroundColorBoxes[i], boxBrush);
            FrameRect(hdc, &backgroundColorBoxes[i], (HBRUSH)GetStockObject(BLACK_BRUSH));
            DeleteObject(boxBrush);
        }

        // Draw background box
        bbBrush = CreateSolidBrush(shapeColorBoxColor);
        FillRect(hdc, &shapeColorBox, bbBrush);
        DeleteObject(bbBrush);
        
        // Draw shape color boxes
        for (int i = 0; i < 5; ++i) {
            HBRUSH boxBrush = CreateSolidBrush(shapeBoxColors[i]);
            FillRect(hdc, &shapeColorBoxes[i], boxBrush);
            FrameRect(hdc, &shapeColorBoxes[i], (HBRUSH)GetStockObject(BLACK_BRUSH));
            DeleteObject(boxBrush);
        }
        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        TextOutW(hdc, windowWidth-  190, 20,L"Background Color ", 17);
        TextOutW(hdc, windowWidth-  410, 20,L"Shape Color          ", 17);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_SETCURSOR: {
        POINT pt;
        GetCursorPos(&pt);           // Get global (screen) coordinates
        ScreenToClient(hwnd, &pt);   // Convert to client area
        int y = pt.y;
        if(y > 0 && y < topBar.bottom){
            SetCursor(LoadCursor(NULL, IDC_HAND)); 
        }else if (y > topBar.bottom)
        {
            if (currentAction == Write) {
                SetCursor(hWriteCursor);
            } else if (currentAction == Erase) {
                SetCursor(hEraseCursor);
            }else if (currentAction == Fill) {
                SetCursor(hFillCursor);
            }else if (currentAction == Clip) {
                SetCursor(hClipCursor);
            }else if (currentAction == DrawLine || currentAction == DrawCircle|| currentAction == DrawEllipse) {
                SetCursor(hDrawCursor);
            } else {
                SetCursor(LoadCursor(NULL, IDC_ARROW)); // Default cursor
            }
        }else {
            SetCursor(LoadCursor(NULL, IDC_ARROW)); // Default cursor
        }
        return TRUE;
        break;
    }

    case WM_CTLCOLORSTATIC:
    {
        // HDC hdcStatic = (HDC)wp;
        // SetBkColor(hdcStatic, RGB(0, 0, 255)); // blue background
        // SetTextColor(hdcStatic, RGB(255, 255, 255)); // white text
        // static HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
        // return (INT_PTR)hBrush;
        HDC hdcStatic = (HDC)wp;
        HWND hStatic = (HWND)lp;
        int ctrlId = GetDlgCtrlID(hStatic);

        if (ctrlId == 1) {
            SetBkColor(hdcStatic, RGB(0, 0, 0));       // background color
            SetTextColor(hdcStatic, RGB(255, 255, 255)); // text color
            return (INT_PTR)hBlackBrush;
        }
        
    }
    case WM_COMMAND:
    {
        // When button with ID 2 is clicked
        if (LOWORD(wp) == drawLineButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = DrawLine;
            count = 0;
            ShowWindow(hClipWindow, SW_HIDE);
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hCombo, SW_SHOW);
            
            // Clear existing items
            SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
            
            // Add new items dynamically
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Direct");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"DDA");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Midpoint");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Modified Midpoint");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Parameterc Line");

            // Set default selection
            SendMessage(hCombo, CB_SETCURSEL, 0, 0);
        }
        else if (LOWORD(wp) == drawCircleButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = DrawCircle;
            count = 0;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hClipWindow, SW_HIDE);
            ShowWindow(hCombo, SW_SHOW);
            // Clear existing items
            SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
            
            // Add new items dynamically
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Direct");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"polar");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"iterative polar");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Midpoint");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Modified Midpoint 1");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Modified Midpoint 2");
            
            // Set default selection
            SendMessage(hCombo, CB_SETCURSEL, 0, 0);
        }
        else if (LOWORD(wp) == drawEllipseButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = DrawEllipse;
            count = 0;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hClipWindow, SW_HIDE);
            ShowWindow(hCombo, SW_SHOW);
            // Clear existing items
            SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
            
            // Add new items dynamically
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Direct");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"polar");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Midpoint");
            
            // Set default selection
            SendMessage(hCombo, CB_SETCURSEL, 0, 0);
        }
        else if (LOWORD(wp) == drawCardinalSplineCurveButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = DrawCardinalSplineCurve;
            count = 0;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hClipWindow, SW_HIDE);
            ShowWindow(hCombo, SW_HIDE);
        }
        else if (LOWORD(wp) == fillButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = Fill ;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hClipWindow, SW_HIDE);
            ShowWindow(hCombo, SW_SHOW);
            // Clear existing items
            SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
            
            // Add new items dynamically
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Recursive Flood Fill");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Non-Recursive Flood Fill");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Convex Fill");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Non Convex Fill");
            
            // Set default selection
            SendMessage(hCombo, CB_SETCURSEL, 0, 0);
        }
        else if (LOWORD(wp) == shapeFillButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = ShapeFill ;
            ShowWindow(hShapeFillList, SW_SHOW);
            ShowWindow(hClipWindow, SW_HIDE);
            ShowWindow(hCombo, SW_HIDE);
        }
        else if (LOWORD(wp) == clipButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = Clip ;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hCombo, SW_SHOW);
            ShowWindow(hClipWindow, SW_SHOW);
            // Clear existing items
            SendMessage(hCombo, CB_RESETCONTENT, 0, 0);
            
            // Add new items dynamically
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Point Clipping");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Line Clipping");
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Polygon Clipping");
            
            // Set default selection
            SendMessage(hCombo, CB_SETCURSEL, 0, 0);
            SendMessage(hClipWindow, CB_SETCURSEL, 0, 0);
        }
        else if (LOWORD(wp) == writeButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = Write ;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hCombo, SW_HIDE);
            ShowWindow(hClipWindow, SW_HIDE);
        }
        else if (LOWORD(wp) == eraseButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = Erase ;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hCombo, SW_HIDE);
            ShowWindow(hClipWindow, SW_HIDE);
        }
        else if (LOWORD(wp) == clearButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = Clear;
            ShowWindow(hShapeFillList, SW_HIDE);
            ShowWindow(hCombo, SW_HIDE);
            ShowWindow(hClipWindow, SW_HIDE);

            backgroundColor = backgroundColor;
            InvalidateRect(hwnd, NULL, TRUE);
            std::cout << "cleared" << std::endl;

        }
        else if (LOWORD(wp) == saveButtonId && HIWORD(wp) == BN_CLICKED) {
            // Get client dimensions
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int capture_width = clientRect.right;
            int capture_height = clientRect.bottom;
            std::vector<std::vector<COLORREF>> capturedPixels = GetPixels(hwnd, capture_width, capture_height, topBar.bottom);
            saveAsBMP(capturedPixels, "savedBoard.bmp");
            MessageBox(hwnd, TEXT("Image saved as savedBoard.bmp!"), TEXT("Save Successful"), MB_OK | MB_ICONINFORMATION);
        }
        else if (LOWORD(wp) == loadButtonId && HIWORD(wp) == BN_CLICKED) {
            std::vector<std::vector<COLORREF>> loadedPixels = loadBMP("savedBoard.bmp"); 
            if (!loadedPixels.empty()) {
                HDC hdc = GetDC(hwnd); // 
                if (hdc) {
                    // Clear the drawing area first (optional, but good practice)
                    RECT clientRect;
                    GetClientRect(hwnd, &clientRect);
                    // Adjust the rectangle to only clear the drawing area, not the top bar
                    clientRect.top = topBar.bottom;
                    HBRUSH bgBrush = CreateSolidBrush(backgroundColor);
                    FillRect(hdc, &clientRect, bgBrush);
                    DeleteObject(bgBrush);

                    // Draw the loaded pixels onto the screen
                    int imgHeight = loadedPixels.size();
                    int imgWidth = (imgHeight > 0) ? loadedPixels[0].size() : 0;

                    for (int y = 0; y < imgHeight; ++y) {
                        for (int x = 0; x < imgWidth; ++x) {
                            // Ensure pixels are drawn within the visible drawing area
                            if ((x + 0) < windowWidth && (y + topBar.bottom) < windowHight) {
                                SetPixel(hdc, x, y + topBar.bottom, loadedPixels[y][x]);
                            }
                        }
                    }
                    ReleaseDC(hwnd, hdc);
                    MessageBox(hwnd, TEXT("Image loaded from savedBoard.bmp!"), TEXT("Load Successful"), MB_OK | MB_ICONINFORMATION);
                } else {
                    MessageBox(hwnd, TEXT("Failed to get device context for drawing."), TEXT("Load Error"), MB_OK | MB_ICONERROR);
                }
            } else {
                MessageBox(hwnd, TEXT("Failed to load image or image is empty."), TEXT("Load Error"), MB_OK | MB_ICONERROR);
            }
        }
        //   event     == select item event
        if (HIWORD(wp) == CBN_SELCHANGE) { 
            int controlID = LOWORD(wp);  // Which control sent the message?
            HWND hCombo = (HWND)lp;
            int index = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            count = 0;
            if(controlID == comboListId){
                if (index != CB_ERR) {
                    if(currentAction == DrawLine){
                        currentLineAlgorithm = index;
                    }else if(currentAction == DrawCircle){
                        currentCircleAlgorithm = index;
                    } else if(currentAction == DrawEllipse) {
                        currentEllipseAlgorithm = index;
                    }
                    else if(currentAction == Fill){
                        currentFillAlgorithm = index;
                    }
                    else if(currentAction == Clip){
                        currentClipAlgorithm = index;
                        
                    }
                }
            }
            else if (controlID == comboClipWindowId){
                currentClipWindowShape = index;
            }
            else if (controlID == shapeFillListId){
                currentShapeFillAlgorithm = index;
            }
        }
    }
    case WM_CTLCOLORBTN:
        return (LRESULT)hBlackBrush;
    case WM_SIZE: {
        // Resize static bar to match width of main window
        windowWidth = LOWORD(lp);
        windowHight = HIWORD(lp);
        //size top bar
        int barHeight = min(windowHight*20/100,105); 
        topBar.right = windowWidth;
        topBar.bottom = barHeight;

        //size background box
        backgroundBox.left = windowWidth - 250;
        backgroundBox.right = windowWidth - 5;
        backgroundBox.bottom = barHeight - 10;

        
        int start = windowWidth - 220;
        for (int i = 0 ; i < 5 ; i++){
            backgroundColorBoxes[i].bottom = min(barHeight - 15,85);
            backgroundColorBoxes[i].left = start ;
            backgroundColorBoxes[i].right = backgroundColorBoxes[i].left + 35;
            start += 40;
        }
        
        //size shape color box
        shapeColorBox.left = windowWidth - 500;
        shapeColorBox.right = windowWidth -255;
        shapeColorBox.bottom = barHeight - 10;

        start = windowWidth - 470;
        for (int i = 0 ; i < 5 ; i++){
            shapeColorBoxes[i].bottom = min(barHeight - 15,85);
            shapeColorBoxes[i].left = start ;
            shapeColorBoxes[i].right = shapeColorBoxes[i].left + 35;
            start += 40;
        }
        break;
    }
    case WM_MOUSEMOVE:{
        if(currentAction == Write && isWrite){
            int x = LOWORD(lp);
            int y = HIWORD(lp);
            if(currentClipAlgorithm == PointClipping){
                if(currentClipWindowShape == CircleWindow){
                    r = sqrt(pow(xc1 - xc2,2) + pow(yc1 - yc2,2));
                    if(PointCircleClipping(x,y,xc1,yc1,r)){
                        break;
                    }
                }
            }
            hdc = GetDC(hwnd);
            SetPixel(hdc,x,y,shapeColor);
            ReleaseDC(hwnd, hdc);
        }
        else if(currentAction == Erase && isErase){
            int x = LOWORD(lp);
            int y = HIWORD(lp);
            hdc = GetDC(hwnd);
            DirectCircle(hdc,x,y,3,backgroundColor);
            SetPixel(hdc,x,y,backgroundColor);
            ReleaseDC(hwnd, hdc);
        }
        break;
    }
    case WM_LBUTTONUP:{
        isWrite = false;
        isErase = false;
        break;
    }
	case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lp);
        int y = HIWORD(lp);
        // if(currentAction == Clear){
        //     backgroundColor = backgroundColor;
        //     InvalidateRect(hwnd, NULL, TRUE);
        //     std::cout << "cleared" << std::endl;

        // }
        if(y < topBar.bottom){
            // check background buttons
            for (int i = 0; i < 5; ++i) {
                if (PtInRect(&backgroundColorBoxes[i], POINT{ x, y })) {
                    backgroundColor = backgroundBoxColors[i];
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
            }

            for (int i = 0; i < 5; ++i) {
                if (PtInRect(&shapeColorBoxes[i], POINT{ x, y })) {
                    shapeColor = shapeBoxColors[i];
                    break;
                }
            }
        }
        else if(currentAction == Write){
            isWrite = true;
        }
        else if(currentAction == Erase){
            isErase = true;
        }
        else if(currentAction == DrawLine){
            if(count == 0){
                x1 = x;
                y1 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x1,y1,shapeColor);
                ReleaseDC(hwnd, hdc);
                count++;
            }else{
                x2 = x;
                y2 = y;
                count = 0;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x2,y2,shapeColor);
                if(currentClipAlgorithm == LineClipping && currentClipWindowShape == RectangleWindow){
                    Point p1(x1,y1);
                    Point p2(x2,y2);
                    if(LineRectangleClipping(p1,p2,xc1,xc2,yc1,yc2)){
                        break;
                    }
                    x1 = p1.x;
                    y1 = p1.y;
                    x2 = p2.x;
                    y2 = p2.y;

                } else if(currentClipAlgorithm == LineClipping && currentClipWindowShape == CircleWindow) {
                    r = sqrt(pow(xc1 - xc2,2) + pow(yc1 - yc2,2));
                    DrawLineClippedByCircle(hdc, x1, y1, x2, y2, xc1, yc1, r, shapeColor);
                    std::cout << currentClipAlgorithm << "\n";
                    std::cout << currentClipWindowShape << "\n";
                    break;
                }

                if(currentLineAlgorithm == DirectLineAlgorithm){
                    DirectLine(hdc,x1,y1, x2, y2,shapeColor);
                }else if ( currentLineAlgorithm == DDALineAlgorithm){
                    // DrawLineDDA(hdc,x1,y1, x2, y2, shapeColor);
                    BresenhamsEfficientDDA(hdc, x1, y1, x2, y2, shapeColor);
                }else if(currentLineAlgorithm == MidpointLineAlgorithm){

                }
                else if(currentLineAlgorithm == ModifiedMidpointLineAlgorithm){

                }
                else if(currentLineAlgorithm == ParametricLineAlgorithm){
                    ParametricLine(hdc,x1,y1, x2, y2,shapeColor);
                }
                    ReleaseDC(hwnd, hdc);
            }
        }
        else if(currentAction == DrawCircle){
            if(count == 0){
                x1 = x;
                y1 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x1,y1,shapeColor);
                ReleaseDC(hwnd, hdc);
                count++;
            }else{
                x2 = x;
                y2 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x2,y2,shapeColor);
                ReleaseDC(hwnd, hdc);
                count = 0;
		        int r = sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
                hdc = GetDC(hwnd);
                if (currentCircleAlgorithm == DirectCircleAlgorithm)
                {
                    DirectCircle(hdc,x1,y1,r,shapeColor);
                }
                else if (currentCircleAlgorithm == PolarCircleAlgorithm)
                {
                    CirclePolarEff(hdc, x1, y1, r, shapeColor);
                    
                }
                else if (currentCircleAlgorithm == IterativePolarCircleAlgorithm)
                {
                    
                }
                else if (currentCircleAlgorithm == MidpointCircleAlgorithm)
                {
                    MidpointCircle(hdc,x1,y1,r,shapeColor);
                }
                else if (currentCircleAlgorithm == ModifiedMidpointCircleAlgorithm1)
                {
                    ModifiedMidpointCircle1(hdc,x1,y1,r,shapeColor);
                }
                else if (currentCircleAlgorithm == ModifiedMidpointCircleAlgorithm2)
                {
                    ModifiedMidpointCircle2(hdc,x1,y1,r,shapeColor);
                    
                }
                ReleaseDC(hwnd, hdc);
            }
        } else if(currentAction == DrawEllipse){
            if(count == 0){
                x1 = x;
                y1 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x1,y1,shapeColor);
                ReleaseDC(hwnd, hdc);
                count++;
            }else{
                x2 = x;
                y2 = y;

                a = abs(x2-x1);
                b = abs(y2-y1);

                hdc = GetDC(hwnd);
                SetPixel(hdc,x2,y2,shapeColor);
                ReleaseDC(hwnd, hdc);
                count = 0;
                hdc = GetDC(hwnd);

                if (currentEllipseAlgorithm == DirectEllipseAlgorithm)
                {

                    DirectEllipse(hdc, x1, y1, a, b, shapeColor);
                }
                else if (currentEllipseAlgorithm == PolarEllipseAlgorithm)
                {
                    EllipsePolar(hdc, x1, y1, a, b, shapeColor);
                    
                }
                else if (currentEllipseAlgorithm == MidpointEllipseAlgorithm)
                {
                    
                }
                
                ReleaseDC(hwnd, hdc);
            }
        }
        else if(currentAction == DrawCardinalSplineCurve){
            if(count < 4){
                if(count == 0){
                    p.resize(0);
                }
                p.push_back(Point(x,y));
                hdc = GetDC(hwnd);
                SetPixel(hdc,x,y,shapeColor);
                ReleaseDC(hwnd, hdc);
                count++;
            }
            
            if(count == 4){
                hdc = GetDC(hwnd);
                CardinalSplineCurve(hdc,p,4,shapeColor);
                ReleaseDC(hwnd, hdc);
                count = 0;
            }
        }
        else if( currentAction == Fill){
            hdc = GetDC(hwnd);
            if (currentFillAlgorithm == RecursiveFloodFillAlgorithms)
            {
                RecursiveFloodFill(hdc,x,y,shapeColor);
            }
            else if (currentFillAlgorithm == NonRecursiveFloodFillAlgorithms)
            {
                NonRecursiveFloodFill(hdc,x,y,shapeColor);
            }
            else if (currentFillAlgorithm == ConvexFillAlgorithms)
            {
                
            }
            else if (currentFillAlgorithm == NonConvexFillAlgorithms)
            {
                if(count == 0){
                    x1 = x;
                    y1 = y;
                    hdc = GetDC(hwnd);
                    SetPixel(hdc,x1,y1,shapeColor);
                    ReleaseDC(hwnd, hdc);
                    count++;
                }
                else if(count == 1){
                    x2 = x;
                    y2 = y;
                    hdc = GetDC(hwnd);
                    SetPixel(hdc,x2,y2,shapeColor);
                    ReleaseDC(hwnd, hdc);
                    count++;
                }
                else if(count == 2){
                    x3 = x;
                    y3 = y;
                    hdc = GetDC(hwnd);
                    SetPixel(hdc,x3,y3,shapeColor);
                    ReleaseDC(hwnd, hdc);
                    count++;
                }
                else {
                    Point points[4] = {Point(x1,y1),Point(x2,y2),Point(x3,y3),Point(x,y)};
                    hdc = GetDC(hwnd);
                    NonConvexFilling(hdc,points,4,shapeColor);
                    ReleaseDC(hwnd, hdc);
                    count = 0;
                }
            }
            else if (currentFillAlgorithm == None)
            {
                
            }
            else if (currentFillAlgorithm == None)
            {
                
            }
            ReleaseDC(hwnd, hdc);
            
        }
        else if (currentAction == ShapeFill){
            if(currentShapeFillAlgorithm == CircleFillWithLines){

            }else if(currentShapeFillAlgorithm == CircleFillWithCircles){
                if(count == 0){
                x1 = x;
                y1 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x1,y1,shapeColor);
                ReleaseDC(hwnd, hdc);
                count++;
                }else if (count == 1){
                    x2 = x;
                    y2 = y;
                    r = sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
                    hdc = GetDC(hwnd);
                    ModifiedMidpointCircle2(hdc,x1,y1,r,shapeColor);
                    ReleaseDC(hwnd, hdc);
                    count++;
                }else{
                    hdc = GetDC(hwnd);
                    CircleQuarterFilling(hdc,x1,y1,r,x,y,shapeColor);
                    ReleaseDC(hwnd, hdc);
                    count = 0;
                }
            }else if(currentShapeFillAlgorithm == SquareFillWithHermiteCurve){
                
            }else if(currentShapeFillAlgorithm == RectangleFillWithBezierCurve){
                
            }
        }
        else if (currentAction == Clip){
            if(count == 0){
                xc1 = x;
                yc1 = y;

                polygon_points_count = 0;
                
            }else if(count >= 1){
                
                if(count == 1) {
                    xc2 = x;
                    yc2 = y;
                    hdc = GetDC(hwnd);
                    if(currentClipWindowShape == RectangleWindow || currentClipWindowShape == SquareWindow ){
                        DirectLine(hdc,xc1,yc1,xc1,yc2,shapeColor);
                        DirectLine(hdc,xc1,yc1,xc2,yc1,shapeColor);
                        DirectLine(hdc,xc2,yc2,xc1,yc2,shapeColor);
                        DirectLine(hdc,xc2,yc2,xc2,yc1,shapeColor);
                        
                    } else if(currentClipWindowShape == CircleWindow){
                        int r = sqrt(pow(xc1 - xc2,2) + pow(yc1 - yc2,2));
                        ModifiedMidpointCircle2(hdc,xc1,yc1,r,shapeColor);
                    }
                    ReleaseDC(hwnd,hdc);

                }
                
                // count = 0;
                hdc = GetDC(hwnd);
                if(count > 1) {
                    p[polygon_points_count] = Point(x, y);
                    polygon_points_count++;

                    if(currentClipWindowShape == RectangleWindow || currentClipWindowShape == SquareWindow ){

                        if(polygon_points_count == 5) {
                            if(currentClipAlgorithm == PolygonClipping) {
                                std::cout << "polygon clipping" << std::endl;
                                // std::cout << p[0].x << " " << p[1].x << " " << p[2].x << " " << p[3].x << " " << p[4].x << " " << std::endl;
                                // std::cout << p[0].y << " " << p[1].y << " " << p[2].y << " " << p[3].y << " " << p[4].y << " " << std::endl;

                                // p[0] = Point(140, 110);
                                // p[1] = Point(230, 200);
                                // p[2] = Point(230, 230);
                                // p[3] = Point(170, 200);
                                // p[4] = Point(130, 170);

                                // std::cout << xc1 << " " << yc1 << std::endl;
                                // std::cout << xc2 << " " << yc2 << std::endl;
                                
                                xl = xc1;
                                xr = xc2;
                                yb = yc1;
                                yt = yc2;

                                std::vector<Point> resultPoly = polygonClip(p, xl, xr, yb, yt);

                                for (int i = 0; i <= resultPoly.size(); i++) {
                                    BresenhamsEfficientDDA(hdc, resultPoly[i%resultPoly.size()].x, resultPoly[i%resultPoly.size()].y,
                                                                resultPoly[(i+1)%resultPoly.size()].x, resultPoly[(i+1)%resultPoly.size()].y, shapeColor);
                                }
                            }

                        }
                        
                        
                    }else if(currentClipWindowShape == CircleWindow){
                        
                    }
                }

                ReleaseDC(hwnd,hdc);
            }
            count ++;
        }
		break;
    }
	case WM_CLOSE:
		DestroyWindow(hwnd); break;
	case WM_DESTROY:
		PostQuitMessage(0); break;
	default:return DefWindowProc(hwnd, m, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh)

{
	WNDCLASSW wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hi;
	RegisterClassW(&wc);
	HWND hwnd = CreateWindowW(L"MyClass", L"Graphics App", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hi, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}