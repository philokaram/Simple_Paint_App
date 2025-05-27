#include <Windows.h>
#include <math.h>
#include <iostream>
#include "Algorithms/LineAlgorithms.cpp"
#include "Algorithms/CircleAlgorithms.cpp"
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
HWND hCombo;
HBRUSH hBlackBrush;
enum Shape {None,Line,Circle};
enum buttonsID {drawLineButtonId=1,drawCircleButtonId};
enum lineAlgorithm {DirectLineAlgorithm,DDALineAlgorithm,MidpointLineAlgorithm,ModifiedMidpointLineAlgorithm};
enum circleAlgorithm {DirectCircleAlgorithm,PolarCircleAlgorithm,IterativePolarCircleAlgorithm,MidpointCircleAlgorithm,ModifiedMidpointCircleAlgorithm1,ModifiedMidpointCircleAlgorithm2};
int currentLineAlgorithm = DirectLineAlgorithm;
int currentCircleAlgorithm = DirectCircleAlgorithm;
Shape currentShape = None;
//1. change Background
COLORREF backgroundColor = RGB(255, 255, 255); 

// top bar
RECT topBar = {0,0,400,200};
COLORREF topBarColor = RGB(50,50,50);

//background box
RECT backgroundBox = {0,5,0,0};
COLORREF backgroundBoxColor = RGB(255,255,255);

RECT colorBoxes[5] = {
/*RECT = {left,top,right,bottom}*/
    {20, 50, 70, 70},      
    {80, 50, 130, 70},     
    {140, 50, 190, 70},   
    {200, 50, 250, 70},     
    {200, 50, 250, 70}     
};

COLORREF boxColors[5] = {
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
    static int x1,x2,y1,y2,count = 0;
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
            200,40,
            hwnd,
            (HMENU)drawLineButtonId,
            nullptr,
            nullptr
        );
        hDrawCircle = CreateWindow(
            TEXT("BUTTON"),
            TEXT("draw circle"),
            WS_CHILD | WS_VISIBLE,
            50,50,
            200,40,
            hwnd,
            (HMENU)drawCircleButtonId,
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

        // Draw color boxes
        for (int i = 0; i < 5; ++i) {
            HBRUSH boxBrush = CreateSolidBrush(boxColors[i]);
            FillRect(hdc, &colorBoxes[i], boxBrush);
            FrameRect(hdc, &colorBoxes[i], (HBRUSH)GetStockObject(BLACK_BRUSH));
            DeleteObject(boxBrush);
        }
        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        TextOutW(hdc, windowWidth-  190, 20,L"Background Color ", 17);
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
            currentShape = Line;
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
            currentShape = Circle;
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
        
        if (HIWORD(wp) == CBN_SELCHANGE) {
            HWND hCombo = (HWND)lp;
            int index = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
            if (index != CB_ERR) {
                if(currentShape == Line){
                    currentLineAlgorithm = index;
                }else if(currentShape == Circle){
                    currentCircleAlgorithm = index;
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
            colorBoxes[i].bottom = min(barHeight - 15,85);
            colorBoxes[i].left = start ;
            colorBoxes[i].right = colorBoxes[i].left + 35;
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
                if (PtInRect(&colorBoxes[i], POINT{ x, y })) {
                    backgroundColor = boxColors[i];
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
            }
        }
        else if(currentShape == Line){
            if(count == 0){
                x1 = x;
                y1 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x1,y1,RGB(0,0,0));
                ReleaseDC(hwnd, hdc);
                count++;
            }else{
                x2 = x;
                y2 = y;
                count = 0;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x2,y2,RGB(255,0,0));
                if(currentLineAlgorithm == DirectLineAlgorithm){
                    DirectLine(hdc,x1,y1, x2, y2, RGB(0,0,0));
                }else if ( currentLineAlgorithm == DDALineAlgorithm){
                    DrawLineDDA(hdc,x1,y1, x2, y2, RGB(0,0,0));
                }else if(currentLineAlgorithm == MidpointLineAlgorithm){
                    BresenhamLine(hdc,x1,y1, x2, y2, RGB(0,0,0));
                }
            else if(currentLineAlgorithm == ModifiedMidpointLineAlgorithm){
                ImprovedBresenhamLine(hdc,x1,y1, x2, y2, RGB(0,0,0));
            }
            // else if(currentLineAlgorithm == parametercLine){
                //     ParametrecLine(hdc,x1,y1, x2, y2, RGB(0,0,0));
                // }
                ReleaseDC(hwnd, hdc);
            }
        }else if(currentShape == Circle){
            if(count == 0){
                x1 = x;
                y1 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x1,y1,RGB(0,0,0));
                ReleaseDC(hwnd, hdc);
                count++;
            }else{
                x2 = x;
                y2 = y;
                hdc = GetDC(hwnd);
                SetPixel(hdc,x2,y2,RGB(255,0,0));
                ReleaseDC(hwnd, hdc);
                count = 0;
		        int r = sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
                hdc = GetDC(hwnd);
                if (currentCircleAlgorithm == DirectCircleAlgorithm)
                {
                    DirectCircle(hdc,x1,y1,r,RGB(0,0,0));
                }
                else if (currentCircleAlgorithm == PolarCircleAlgorithm)
                {

                }
                else if (currentCircleAlgorithm == IterativePolarCircleAlgorithm)
                {
                    
                }
                else if (currentCircleAlgorithm == MidpointCircleAlgorithm)
                {
                    
                }
                else if (currentCircleAlgorithm == ModifiedMidpointCircleAlgorithm1)
                {
                    ModifiedMidpointCircle1(hdc,x1,y1,r,RGB(0,0,0));
                }
                else if (currentCircleAlgorithm == ModifiedMidpointCircleAlgorithm2)
                {
                    ModifiedMidpointCircle2(hdc,x1,y1,r,RGB(0,0,0));
                    
                }
                ReleaseDC(hwnd, hdc);
            }
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