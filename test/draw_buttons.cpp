#include <windows.h>

const char CLASS_NAME[] = "CircleDrawApp";
const int CIRCLE_RADIUS = 20;

bool canDraw = false;
HWND buttonEnable, buttonDisable;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Create Enable Draw button
            buttonEnable = CreateWindow("BUTTON", "Enable Draw",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                10, 10, 100, 30, hwnd, (HMENU)1, NULL, NULL);

            // Create Disable Draw button
            buttonDisable = CreateWindow("BUTTON", "Disable Draw",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                120, 10, 100, 30, hwnd, (HMENU)2, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                canDraw = true;
            } else if (LOWORD(wParam) == 2) {
                canDraw = false;
            }
            break;

        case WM_LBUTTONDOWN:
            if (canDraw) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);

                // Avoid drawing on the buttons
                if (y > 50) {
                    HDC hdc = GetDC(hwnd);
                    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
                    SelectObject(hdc, brush);
                    Ellipse(hdc, x - CIRCLE_RADIUS, y - CIRCLE_RADIUS,
                                  x + CIRCLE_RADIUS, y + CIRCLE_RADIUS);
                    DeleteObject(brush);
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
        0, CLASS_NAME, "Circle Drawer",
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
