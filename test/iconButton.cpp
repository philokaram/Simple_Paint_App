#include <windows.h>

// Global variables
HINSTANCE hInst;
HWND hButton;

// Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // Register the window class
    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("SimpleWinApp");
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("Windows App with Button"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
    {
        // Create the button
        hButton = CreateWindow(
            TEXT("BUTTON"),
            TEXT("Click Me"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_ICON | BS_CENTER,
            150, 120, 30, 30,
            hwnd,
            (HMENU)1,
            hInst,
            nullptr
        );

        // Load icon from file (change path to your own .ico file)
        HICON hIcon = (HICON)LoadImage(
            nullptr,
            TEXT("review_on_marketing_letter_feedback_rating_icon_265644.ico"),  // <- Change this to a real icon path
            IMAGE_ICON,
            32, 32,
            LR_LOADFROMFILE
        );

        if (hIcon) {
            SendMessage(hButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
        }

        // // Set custom font
        // HFONT hFont = CreateFont(
        //     20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        //     ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        //     DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Segoe UI")
        // );
        // SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            MessageBox(hwnd, TEXT("Button was clicked!"), TEXT("Info"), MB_OK | MB_ICONINFORMATION);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
