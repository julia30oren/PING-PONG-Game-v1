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
#include "platform_common.cpp"
#include "sim_game.cpp"

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
    const wchar_t CLASS_NAME[] = L"Game Window Class"; // Register the window class.
    
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = CLASS_NAME;
    window_class.lpfnWndProc = window_callback;

    RegisterClass(&window_class);

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

    Input input = {};

    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }

    while (running) {
        // Input
        MSG message;

        for (int i=0; i < BUTTON_COUNT;i++) {
            input.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message)
            {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);

                #define process_button(b,vk)\
                case vk:{\
                    input.buttons[b].changed = is_down != input.buttons[b].is_down;\
                    input.buttons[b].is_down = is_down;\
                    } break;

                    switch (vk_code)
                    {
                        process_button(BUTTON_UP, VK_UP);
                        process_button(BUTTON_DOWN, VK_DOWN);
                        process_button(BUTTON_W, 'W');
                        process_button(BUTTON_S, 'S');
                        process_button(BUTTON_LEFT, VK_LEFT);
                        process_button(BUTTON_RIGHT, VK_RIGHT);
                        process_button(BUTTON_ENTER, VK_RETURN);
                    }
            } break;

            default:
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
                
            }
            
        }

        // Simulate
        simulate_game(&input, delta_time);

        // Render
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);


        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;
    }
}