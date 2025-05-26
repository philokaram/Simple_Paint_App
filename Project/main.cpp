#include <Windows.h>
#include <math.h>
#include "Algorithms/line/DDA.cpp"
#include "Algorithms/circle/Breaznham.cpp"
HWND hStaticLabel;
HWND hDrawLineButton;
HWND hDrawCircle;
HBRUSH hBlackBrush;
enum Shape {None,Line,Circle};
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
        // hStaticLabel = CreateWindow(
        //     TEXT("STATIC"),
        //     TEXT("                Background Color"),
        //     WS_CHILD | WS_VISIBLE,
        //     600,20,
        //     200,50,
        //     hwnd,
        //     (HMENU)1,
        //     nullptr,
        //     nullptr
        // );
        
        hDrawLineButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("draw line"),
            WS_CHILD | WS_VISIBLE,
            50,50,
            200,40,
            hwnd,
            (HMENU)2,
            nullptr,
            nullptr
        );
        hDrawCircle = CreateWindow(
            TEXT("BUTTON"),
            TEXT("draw circle"),
            WS_CHILD | WS_VISIBLE,
            50,5,
            200,40,
            hwnd,
            (HMENU)3,
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
        TextOutW(hdc, windowWidth-  190, 20,L"Background Color", 17);
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
        if (LOWORD(wp) == 2) {
                currentShape = Line;
        } else if (LOWORD(wp) == 3) {
                currentShape = Circle;
        }else{
            currentShape = None;
        }
    }
    case WM_CTLCOLORBTN:
        return (LRESULT)hBlackBrush;
    case WM_SIZE: {
        // Resize static bar to match width of main window
        windowWidth = LOWORD(lp);
        windowHight = HIWORD(lp);
        //size top bar
        int barHeight = windowHight*20/100; 
        topBar.right = windowWidth;
        topBar.bottom = barHeight;

        //size background box
        backgroundBox.left = windowWidth - 250;
        backgroundBox.right = windowWidth - 5;
        backgroundBox.bottom = barHeight - 10;

        int start = windowWidth - 230;
        for (int i = 0 ; i < 5 ; i++){
            colorBoxes[i].bottom = barHeight - 20;
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
                count++;
            }else{
                x2 = x;
                y2 = y;
                count = 0;
            hdc = GetDC(hwnd);
            DrawLineDDA(hdc,x1,y1, x2, y2, RGB(0,0,0));
			ReleaseDC(hwnd, hdc);
            }
        }else if(currentShape == Circle){
            if(count == 0){
                x1 = x;
                y1 = y;
                count++;
            }else{
                x2 = x;
                y2 = y;
                count = 0;
		        int r = sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
                hdc = GetDC(hwnd);
                DrawCircleBres(hdc,x1, y1, r, RGB(0,0,0));
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