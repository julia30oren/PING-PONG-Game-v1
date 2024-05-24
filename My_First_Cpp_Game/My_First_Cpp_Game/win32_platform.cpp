#include <windows.h>

bool running = true;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    } break;
    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return  result;
}

int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    // Create Window Class ----------------------------------------------------------------------- 1 
    const wchar_t CLASS_NAME[] = L"Game Window Class"; // Register the window class.
    
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = CLASS_NAME;
    window_class.lpfnWndProc = window_callback;

    // Register Class ----------------------------------------------------------------------------- 2
    RegisterClass(&window_class);

    // Create Window
    HWND window = CreateWindowEx(
        0,                                          // Optional window styles.
        window_class.lpszClassName,                 // Window class
        L"My Firest Game!",                         // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,           // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,

        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        NULL        // Additional application data
    );

    if (window == NULL)
    {
        return 0;
    }

    while (running) {
        // Input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // Simulate

        // Render

    }
}