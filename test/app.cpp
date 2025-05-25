#include <windows.h>
#include <commdlg.h>  // For ChooseColor

// Global variables
HINSTANCE hInst;
HWND hCircleBtn, hSquareBtn, hTriangleBtn;
COLORREF bgColor = RGB(255, 255, 255); // default background color
int currentShape = 0; // 0 = none, 1 = circle, 2 = square, 3 = triangle

// Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawShape(HDC hdc, RECT rc, int shape);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    hInst = hInstance;

    // Register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("ShapeDrawerApp");
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("Shape Drawer"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Draw selected shape
void DrawShape(HDC hdc, RECT rc, int shape) {
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(RGB(100, 100, 255));
    SelectObject(hdc, hBrush);

    switch (shape) {
    case 1: // Circle
        Ellipse(hdc, w / 4, h / 4, w * 3 / 4, h * 3 / 4);
        break;
    case 2: // Square
        Rectangle(hdc, w / 4, h / 4, w * 3 / 4, h * 3 / 4);
        break;
    case 3: // Triangle
        {
            POINT pts[3] = {
                { w / 2, h / 4 },
                { w / 4, h * 3 / 4 },
                { w * 3 / 4, h * 3 / 4 }
            };
            Polygon(hdc, pts, 3);
        }
        break;
    }

    DeleteObject(hPen);
    DeleteObject(hBrush);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        // Create top bar (a static area for color and buttons)
        CreateWindow(TEXT("STATIC"), nullptr,
            WS_CHILD | WS_VISIBLE,
            0, 0, 600, 40,
            hwnd, nullptr, hInst, nullptr);

        // Buttons
        hCircleBtn = CreateWindow(TEXT("BUTTON"), TEXT("Circle"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 5, 80, 30,
            hwnd, (HMENU)101, hInst, nullptr);

        hSquareBtn = CreateWindow(TEXT("BUTTON"), TEXT("Square"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            100, 5, 80, 30,
            hwnd, (HMENU)102, hInst, nullptr);

        hTriangleBtn = CreateWindow(TEXT("BUTTON"), TEXT("Triangle"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            190, 5, 80, 30,
            hwnd, (HMENU)103, hInst, nullptr);

        // Color Picker Button
        CreateWindow(TEXT("BUTTON"), TEXT("Color"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            280, 5, 80, 30,
            hwnd, (HMENU)104, hInst, nullptr);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 101:
            currentShape = 1; InvalidateRect(hwnd, nullptr, TRUE); break;
        case 102:
            currentShape = 2; InvalidateRect(hwnd, nullptr, TRUE); break;
        case 103:
            currentShape = 3; InvalidateRect(hwnd, nullptr, TRUE); break;
        case 104: {
            // Color Picker
            CHOOSECOLOR cc = {};
            static COLORREF custColors[16] = {};
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hwnd;
            cc.lpCustColors = custColors;
            cc.rgbResult = bgColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            if (ChooseColor(&cc)) {
                bgColor = cc.rgbResult;
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            break;
        }
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Fill background with chosen color
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        HBRUSH bgBrush = CreateSolidBrush(bgColor);
        FillRect(hdc, &clientRect, bgBrush);
        DeleteObject(bgBrush);

        // Draw the shape
        RECT drawingArea = clientRect;
        drawingArea.top += 40; // below the toolbar
        DrawShape(hdc, drawingArea, currentShape);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
