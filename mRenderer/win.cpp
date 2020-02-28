#include "mBase.h"
#include "mDevice.h"
#include <windows.h>
#include <stdlib.h>

#ifdef UNICODE
static const wchar_t *WINDOW_CLASS_NAME = L"Class";
static const wchar_t *WINDOW_ENTRY_NAME = L"Entry";
#else
static const char *WINDOW_CLASS_NAME = "Class";
static const char *WINDOW_ENTRY_NAME = "Entry";
#endif


struct window {
    HWND handle;
    HDC memory_dc;
    mDevice *device;
    /* common data */
    bool shouldClose;
    char keys[KEY_NUM];
    char btns[BUTTON_NUM];
    CallBackFunc callbackfuncs;
    void *userdata;
};

static void processKeyMsg(window_t *window, WPARAM virtual_key, char pressed) {
    KeyCode key;
    switch (virtual_key) {
    case 'A':       key = KEY_A;        break;
    case 'D':       key = KEY_D;        break;
    case 'E':       key = KEY_E;        break;
    case 'Q':       key = KEY_Q;        break;
    case 'S':       key = KEY_S;        break;
    case 'W':       key = KEY_W;        break;
    case VK_SPACE:  key = KEY_SPACE;    break;
    case VK_ESCAPE: key = KEY_ESC;      break;
    default:        key = KEY_NUM;      break;
    }
    if (key < KEY_NUM) {
        window->keys[key] = pressed;
        if (window->callbackfuncs.keyCallback) {
            window->callbackfuncs.keyCallback(window, key, pressed);
        }
    }
}

static void processMouseBtnMsg(window_t *window, MouseButton btn, char pressed) {
    window->btns[btn] = pressed;
    if (window->callbackfuncs.btnCallback)
        window->callbackfuncs.btnCallback(window, btn, pressed);
}

static void processScrollMsg(window_t *window, float offset) {
    if (window->callbackfuncs.scrollCallback)
        window->callbackfuncs.scrollCallback(window, offset);
}

static LRESULT CALLBACK processMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    window_t *window = (window_t*)GetProp(hWnd, WINDOW_ENTRY_NAME);
    if (window == NULL) {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    } else if (uMsg == WM_CLOSE) {
        window->shouldClose = true;
        return 0;
    } else if (uMsg == WM_KEYDOWN) {
        processKeyMsg(window, wParam, 1);
        return 0;
    } else if (uMsg == WM_KEYUP) {
        processKeyMsg(window, wParam, 0);
        return 0;
    } else if (uMsg == WM_LBUTTONDOWN) {
        processMouseBtnMsg(window, BUTTON_L, 1);
        return 0;
    } else if (uMsg == WM_RBUTTONDOWN) {
        processMouseBtnMsg(window, BUTTON_R, 1);
        return 0;
    } else if (uMsg == WM_LBUTTONUP) {
        processMouseBtnMsg(window, BUTTON_L, 0);
        return 0;
    } else if (uMsg == WM_RBUTTONUP) {
        processMouseBtnMsg(window, BUTTON_R, 0);
        return 0;
    } else if (uMsg == WM_MOUSEWHEEL) {
        float offset = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
        processScrollMsg(window, offset);
        return 0;
    } else {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}


static void registerClass(void) {
    static int initialized = 0;
    if (initialized == 0) {
        ATOM class_atom;
        WNDCLASS wndclass;
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = processMsg;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = GetModuleHandle(NULL);
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = WINDOW_CLASS_NAME;
        class_atom = RegisterClass(&wndclass);
        mAssert(class_atom != 0);
        UNUSED_VAR(class_atom);
        initialized = 1;
    }
}


static HWND createWindow(const char *title_, int width, int height) {
    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT rect;
    HWND handle;

#ifdef UNICODE
    wchar_t title[LINE_SIZE];
    mbstowcs(title, title_, LINE_SIZE);
#else
    const char *title = title_;
#endif

    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, style, 0);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    handle = CreateWindow(WINDOW_CLASS_NAME, title, style,
                          CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                          NULL, NULL, GetModuleHandle(NULL), NULL);
    mAssert(handle != NULL);
    return handle;
}


static void createBitmapBuffer(HWND hwnd, int width, int height, 
    _Out_ uchar * pScreen, _Out_ HDC *out_memory_dc) {
    HDC                 window_dc;
    HDC                 memory_dc;
    HBITMAP             bitmap0;
    HBITMAP             bitmap1;
    LPVOID              pBitmap;        // ptr 将指向bitmap的首地址

    window_dc = GetDC(hwnd);                    // get 当前的句柄
    memory_dc = CreateCompatibleDC(window_dc);  // 创建新句柄
    ReleaseDC(hwnd, window_dc);                 // 有必要吗

    BITMAPINFO bitmapinfo = { { sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB, width * height * 4, 0, 0, 0, 0 } };
    bitmap0 = CreateDIBSection(memory_dc, &bitmapinfo, DIB_RGB_COLORS, _Out_ &pBitmap, 0, 0);
    mAssert(bitmap0 != NULL);

    bitmap1 = (HBITMAP)SelectObject(memory_dc, bitmap0);
    DeleteObject(bitmap1);

    pScreen = (uchar *)pBitmap;
    *out_memory_dc = memory_dc;
}



window_t *mInitWindow(uint ** & screen, int width, int height, const char *title = "mRenderer") {
    window_t *window;
    HWND hwnd;
    HDC memory_dc;
    uchar * ptr;
    mAssert(width > 0 && height > 0);

    registerClass();
    hwnd = createWindow(title, width, height);
    createBitmapBuffer(hwnd, width, height, _Out_ ptr, &memory_dc);

    window = (window_t*)malloc(sizeof(window_t));
    memset(window, 0, sizeof(window_t));
    window->handle = hwnd;
    window->memory_dc = memory_dc;

    memset(ptr, 0, width * height * 4);
    char *p = (char*)malloc(width * height * 4);
    // screen = (uint**)p;
    // todo: malloc zbuffer
    window->device->framebuffer = (uint**)p;
    for (int i = 0; i < height; i++)
        screen[i] = (uint*)(ptr + width * 4 * i);

    SetProp(hwnd, WINDOW_ENTRY_NAME, window);
    ShowWindow(hwnd, SW_SHOW);
    return window;
}

void destroyWindow(window_t *window) {
    ShowWindow(window->handle, SW_HIDE);
    RemoveProp(window->handle, WINDOW_ENTRY_NAME);

    DeleteDC(window->memory_dc);
    DestroyWindow(window->handle);

    free(window->device->framebuffer);
    // todo: free zbuffer
    free(window);
}

bool windowShouldClose(window_t *window) {
    return window->shouldClose;
}