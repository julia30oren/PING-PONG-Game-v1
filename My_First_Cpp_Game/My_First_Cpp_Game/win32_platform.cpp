#include "utils.cpp"
#include <windows.h>

global_variable bool running = true;

struct Render_State
{
    int height, width; 
    void* memory;

    BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "renderer.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        } break;
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;

            int size = render_state.width * render_state.height * sizeof(unsigned int);

            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            // A BITMAPINFOHEADER structure that contains information about the dimensions of color format.
            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader); // Specifies the number of bytes required by the structure
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;                        // Specifies the width of the bitmap, in pixels
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;                                  // Specifies the number of planes for the target device
            render_state.bitmap_info.bmiHeader.biBitCount = 32;                               // Specifies the number of bits per pixel (bpp)
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;                        // For compressed video and YUV formats, this member is a FOURCC code, specified as a DWORD in little-endian order
        }
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

    // Create Window ------------------------------------------------------------------------------ 3
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

    HDC hdc = GetDC(window);

    // --------------------------------------------------------------------------------------------- 4
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
        //render_background();
        clear_screen(0xfff7d5);
        draw_rect(0, 0, 1, 1, 0xffe370);
        draw_rect(20, 20, 5, 1, 0xffe370);
        draw_rect(-40, -10, 2, 6, 0xffe370);

        // Render
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

    }
}