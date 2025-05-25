#include <windows.h>

const char CLASS_NAME[] = "ShapeDrawApp";
const int SHAPE_SIZE = 40;

enum ShapeType { NONE, CIRCLE, SQUARE, TRIANGLE };
ShapeType currentShape = NONE;

HWND btnCircle, btnSquare, btnTriangle;

void DrawShape(HDC hdc, int x, int y, ShapeType shape) {
    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, brush);

    switch (shape) {
        case CIRCLE:
            Ellipse(hdc, x - SHAPE_SIZE, y - SHAPE_SIZE,
                         x + SHAPE_SIZE, y + SHAPE_SIZE);
            break;

        case SQUARE:
            Rectangle(hdc, x - SHAPE_SIZE, y - SHAPE_SIZE,
                           x + SHAPE_SIZE, y + SHAPE_SIZE);
            break;

        case TRIANGLE:
            {
                POINT points[3] = {
                    { x, y - SHAPE_SIZE },
                    { x - SHAPE_SIZE, y + SHAPE_SIZE },
                    { x + SHAPE_SIZE, y + SHAPE_SIZE }
                };
                Polygon(hdc, points, 3);
            }
            break;
        default:
            break;
    }

    DeleteObject(brush);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            btnCircle = CreateWindow("BUTTON", "Draw Circle",
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                10, 10, 100, 30, hwnd, (HMENU)1, NULL, NULL);

            btnSquare = CreateWindow("BUTTON", "Draw Square",
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                120, 10, 100, 30, hwnd, (HMENU)2, NULL, NULL);

            btnTriangle = CreateWindow("BUTTON", "Draw Triangle",
                WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                230, 10, 100, 30, hwnd, (HMENU)3, NULL, NULL);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1:
                    currentShape = CIRCLE;
                    break;
                case 2:
                    currentShape = SQUARE;
                    break;
                case 3:
                    currentShape = TRIANGLE;
                    break;
            }
            break;

        case WM_LBUTTONDOWN:
            if (currentShape != NONE) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);

                if (y > 50) { // avoid clicking on buttons
                    HDC hdc = GetDC(hwnd);
                    DrawShape(hdc, x, y, currentShape);
                    ReleaseDC(hwnd, hdc);
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Shape Drawer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
