#include <windows.h>

const char CLASS_NAME[] = "CircleApp";
const int WINDOW_RADIUS = 100;
const int CIRCLE_RADIUS = 10;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HDC hdc;
    static HBRUSH brush;

    switch (uMsg) {
        case WM_CREATE:
            // Create a circular region for the window
            {
                HRGN region = CreateEllipticRgn(0, 0, WINDOW_RADIUS * 2, WINDOW_RADIUS * 2);
                SetWindowRgn(hwnd, region, TRUE);
            }
            break;

        case WM_LBUTTONDOWN:
            {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);

                hdc = GetDC(hwnd);
                brush = CreateSolidBrush(RGB(255, 0, 0));
                SelectObject(hdc, brush);
                Ellipse(hdc, x - CIRCLE_RADIUS, y - CIRCLE_RADIUS, x + CIRCLE_RADIUS, y + CIRCLE_RADIUS);
                DeleteObject(brush);
                ReleaseDC(hwnd, hdc);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST,        // extended style
        CLASS_NAME,                           // window class
        "Circle App",                         // window title
        WS_POPUP,                             // no border
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_RADIUS * 2, WINDOW_RADIUS * 2, // size
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    // Make the window transparent around the circular region
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}