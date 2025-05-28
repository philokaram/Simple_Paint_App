#include <Windows.h>
#include <math.h>
#include <iostream>
#include "Algorithms/Utility.cpp"
#include "Algorithms/LineAlgorithms.cpp"
#include "Algorithms/CircleAlgorithms.cpp"
#include "Algorithms/Filling/FloodFill.cpp"
#include "Algorithms/Filling/ConvexAndNonConvex.cpp"
#include "Algorithms/line/DDA.cpp"
#include "Algorithms/line/Bresenham.cpp"
#include "Algorithms/line/ImprovedBresenham.cpp"
#include "Algorithms/line/ParametricLine.cpp"

int min(int e1,int e2){
    if(e1 < e2)
        return e1;
    return e2;
}
HWND hStaticLabel;
HWND hDrawLineButton;
HWND hDrawCircle;
HWND hFill;
HWND hCombo;
HBRUSH hBlackBrush;



enum Action {None,DrawLine,DrawCircle,Fill};
Action currentAction = None;

enum lineAlgorithm {DirectLineAlgorithm,DDALineAlgorithm,MidpointLineAlgorithm,ModifiedMidpointLineAlgorithm};
int currentLineAlgorithm = DirectLineAlgorithm;

enum circleAlgorithm {DirectCircleAlgorithm,PolarCircleAlgorithm,IterativePolarCircleAlgorithm,MidpointCircleAlgorithm,ModifiedMidpointCircleAlgorithm1,ModifiedMidpointCircleAlgorithm2};
int currentCircleAlgorithm = DirectCircleAlgorithm;

enum fillingAlgorithms {RecursiveFloodFillAlgorithms,NonRecursiveFloodFillAlgorithms,ConvexFillAlgorithms,NonConvexFillAlgorithms};
int currentFillAlgorithm = RecursiveFloodFillAlgorithms;

enum buttonsID {drawLineButtonId=1,drawCircleButtonId,fillButtonId};
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
    static int x1,x2,y1,y2,x3,y3,count = 0;
    static int  windowWidth,windowHight;
	switch (m)
	{

    case WM_CREATE:
        // Create a combo box (drop-down list)
        hCombo = CreateWindowEx(
            0,                    // Extended style
            TEXT("COMBOBOX"),           // Class name
            NULL,                 // No window title
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
            260, 40, 200, 300,    // x, y, width, height
            hwnd,                 // Parent window handle
            (HMENU)0,             // Control ID
            ((LPCREATESTRUCT)lp)->hInstance,
            NULL
        );

        // Add items to combo box
        // SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Option 1");
        // SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Option 2");
        // SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Option 3");

        // Optionally, set default selected item
        SendMessage(hCombo, CB_SETCURSEL, 0, 0);
        
        hDrawLineButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("draw line"),
            WS_CHILD | WS_VISIBLE,
            50,5,
            100,30,
            hwnd,
            (HMENU)drawLineButtonId,
            nullptr,
            nullptr
        );
        hDrawCircle = CreateWindow(
            TEXT("BUTTON"),
            TEXT("draw circle"),
            WS_CHILD | WS_VISIBLE,
            50,40,
            100,30,
            hwnd,
            (HMENU)drawCircleButtonId,
            nullptr,
            nullptr
        );
        hFill = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Fill"),
            WS_CHILD | WS_VISIBLE,
            50,75,
            100,30,
            hwnd,
            (HMENU)fillButtonId,
            nullptr,
            nullptr
        );
		break;

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
        else if (LOWORD(wp) == fillButtonId && HIWORD(wp) == BN_CLICKED) {
            currentAction = Fill ;
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
        
        if (HIWORD(wp) == CBN_SELCHANGE) {
            HWND hCombo = (HWND)lp;
            int index = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (index != CB_ERR) {
                count = 0;
                if(currentAction == DrawLine){
                    currentLineAlgorithm = index;
                }else if(currentAction == DrawCircle){
                    currentCircleAlgorithm = index;
                }
                else if(currentAction == Fill){
                    currentFillAlgorithm = index;
                }
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
	case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lp);
        int y = HIWORD(lp);
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
                if(currentLineAlgorithm == DirectLineAlgorithm){
                    DirectLine(hdc,x1,y1, x2, y2,shapeColor);
                }else if ( currentLineAlgorithm == DDALineAlgorithm){
                    DrawLineDDA(hdc,x1,y1, x2, y2, shapeColor);
                }else if(currentLineAlgorithm == MidpointLineAlgorithm){
                    BresenhamLine(hdc,x1,y1, x2, y2,shapeColor);
                }
                else if(currentLineAlgorithm == ModifiedMidpointLineAlgorithm){
                    ImprovedBresenhamLine(hdc,x1,y1, x2, y2, shapeColor);
                }
                // else if(currentLineAlgorithm == parametercLine){
                    //     ParametrecLine(hdc,x1,y1, x2, y2, shapeColor);
                    // }
                    ReleaseDC(hwnd, hdc);
            }
        }else if(currentAction == DrawCircle){
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
        }else if( currentAction == Fill){
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
		break;
    }
	case WM_LBUTTONUP:
		break;
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