#include <windows.h>
#include <string>
#include <sstream>

using namespace std;

// Global Variables
HWND hEdit1, hEdit2, hButtonAdd, hButtonSub, hButtonMul, hButtonDiv;

HBRUSH hBrushBackground = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH hBrushStaticText = CreateSolidBrush(RGB(255, 255, 255));

// Function to Convert Text to Double
double GetValueFromEditBox(HWND hEdit) {
    char buffer[256];
    GetWindowText(hEdit, buffer, 256);
    return atof(buffer);
}

// Function to Display Result in Message Box
void ShowResult(double result) {
    char buffer[256];
    sprintf(buffer, "%.6f", result);
    MessageBox(NULL, buffer, "Result", MB_OK);
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Static Text (สีขาว)
            CreateWindow("STATIC", "Please input two numbers", WS_VISIBLE | WS_CHILD,
                         50, 20, 200, 20, hwnd, NULL, NULL, NULL);

            // ช่องกรอกตัวเลข
            hEdit1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                  50, 50, 200, 25, hwnd, NULL, NULL, NULL);
            hEdit2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                  50, 80, 200, 25, hwnd, NULL, NULL, NULL);

            // ปุ่มเครื่องหมาย +, -, *, /
            hButtonAdd = CreateWindow("BUTTON", "+", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      50, 120, 40, 30, hwnd, (HMENU)1, NULL, NULL);
            hButtonSub = CreateWindow("BUTTON", "-", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      100, 120, 40, 30, hwnd, (HMENU)2, NULL, NULL);
            hButtonMul = CreateWindow("BUTTON", "*", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      150, 120, 40, 30, hwnd, (HMENU)3, NULL, NULL);
            hButtonDiv = CreateWindow("BUTTON", "/", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      200, 120, 40, 30, hwnd, (HMENU)4, NULL, NULL);
            break;
        }

        case WM_COMMAND: {
            double num1 = GetValueFromEditBox(hEdit1);
            double num2 = GetValueFromEditBox(hEdit2);
            double result = 0.0;

            switch (LOWORD(wParam)) {
                case 1: result = num1 + num2; ShowResult(result); break;
                case 2: result = num1 - num2; ShowResult(result); break;
                case 3: result = num1 * num2; ShowResult(result); break;
                case 4: 
                    if (num2 != 0) {
                        result = num1 / num2;
                    } else {
                        MessageBox(NULL, "Error: Division by zero!", "Error", MB_OK | MB_ICONERROR);
                        return 0;
                    }
                    ShowResult(result); 
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: { // เปลี่ยนสีพื้นหลังของ Static Text เป็นสีขาว
            HDC hdcStatic = (HDC) wParam;
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // ตั้งค่าสีพื้นหลังเป็นขาว
            return (LRESULT) hBrushStaticText; // คืนค่า brush สีขาว
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Main Function (Entry Point)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Calculator";
    wc.hbrBackground = hBrushBackground;  // ใช้สีพื้นหลังสีชมพูเข้ม

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindow("Calculator", "My Calculator",
                             WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                             CW_USEDEFAULT, CW_USEDEFAULT, 300, 250, // ขนาดหน้าต่าง
                             NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
