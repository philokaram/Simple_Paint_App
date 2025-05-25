#include <Windows.h>
#include <math.h>
#include "Algorithms/line/DDA.cpp"
#include "Algorithms/circle/Breaznham.cpp"
HWND hStaticBar1;
HWND hDrawLineButton;
HWND hDrawCircle;
HBRUSH hBlueBrush;
enum Shape {None,Line,Circle};
Shape currentShape = None;
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
	HDC hdc;
    static int x1,x2,y1,y2,count = 0;
	switch (m)
	{
    case WM_CREATE:
        hStaticBar1 = CreateWindow(
            TEXT("STATIC"),
            nullptr,
            WS_CHILD | WS_VISIBLE,
            50,50,
            600,40,
            hwnd,
            (HMENU)1,
            nullptr,
            nullptr
        );
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
            250,50,
            200,40,
            hwnd,
            (HMENU)3,
            nullptr,
            nullptr
        );
        hBlueBrush = CreateSolidBrush(RGB(0, 0, 255)); // Pure blue
		break;
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
            SetBkColor(hdcStatic, RGB(0, 0, 255));       // background color
            SetTextColor(hdcStatic, RGB(255, 255, 255)); // text color
            return (INT_PTR)hBlueBrush;
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
        return (LRESULT)hBlueBrush;
    case WM_SIZE: {
        // Resize static bar to match width of main window
        int width = LOWORD(lp);
        int height = HIWORD(lp);

        int barHeight = height*15/100; 
        MoveWindow(hStaticBar1, 0, 0, width, barHeight, TRUE);
        break;
    }
	case WM_LBUTTONDOWN:
    {
        if(currentShape == Line){
            if(count == 0){
                x1 = LOWORD(lp);
                y1 = HIWORD(lp);
                count++;
            }else{
                x2 = LOWORD(lp);
                y2 = HIWORD(lp);
                count = 0;
            hdc = GetDC(hwnd);
            DrawLineDDA(hdc,x1,y1, x2, y2, RGB(0,0,0));
			ReleaseDC(hwnd, hdc);
            }
        }else if(currentShape == Circle){
            if(count == 0){
                x1 = LOWORD(lp);
                y1 = HIWORD(lp);
                count++;
            }else{
                x2 = LOWORD(lp);
                y2 = HIWORD(lp);
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