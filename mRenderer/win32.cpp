#include "win32.h"

static HWND         hwnd;
static HDC          memory_dc;
static HBITMAP      bitmap0 = NULL;
static bool         shouldClose = false;

static int w = mDevice::width;
static int h = mDevice::height;


static void processKeyMsg(WPARAM virtualKey, bool pressed) {
    KeyCode key;
    switch (virtualKey) {
    case 'A':       key = KeyCode::A;        break;
    case 'D':       key = KeyCode::D;        break;
    case 'E':       key = KeyCode::E;        break;
    case 'Q':       key = KeyCode::Q;        break;
    case 'S':       key = KeyCode::S;        break;
    case 'W':       key = KeyCode::W;        break;
    case VK_SPACE:  key = KeyCode::SPACE;    break;
    case VK_ESCAPE: key = KeyCode::ESC;      break;
    default:        key = KeyCode::KEY_NUM;  break;
    }

    if (key < KeyCode::KEY_NUM) {
        mDevice::keys[(uint)key] = pressed;
        if (mDevice::callbackfuncs.keyCallback) {
            mDevice::callbackfuncs.keyCallback(key, pressed);
        }
    }
}

static void processBtnMsg(MouseBtn btn, bool pressed) {
    mDevice::btns[(uint)btn] = pressed;
    if (mDevice::callbackfuncs.btnCallback)
        mDevice::callbackfuncs.btnCallback(btn, pressed);
}

static void processCursorMsg() {
    float xPos = -1;
    float yPos = -1;
    getCursorPos(xPos, yPos);
    if (mDevice::callbackfuncs.cursorCallback)
        mDevice::callbackfuncs.cursorCallback(xPos, yPos);
}

static void processScrollMsg(float offset) {
    if (mDevice::callbackfuncs.scrollCallback)
        mDevice::callbackfuncs.scrollCallback(offset);
}

static LRESULT CALLBACK processMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:          shouldClose = true;                     break;
    case WM_KEYDOWN:        processKeyMsg(wParam, true);            break;
    case WM_KEYUP:          processKeyMsg(wParam, false);           break;
    case WM_LBUTTONDOWN:    processBtnMsg(MouseBtn::L, true);       break;
    case WM_LBUTTONUP:      processBtnMsg(MouseBtn::L, false);      break;
    case WM_RBUTTONDOWN:    processBtnMsg(MouseBtn::R, true);       break;
    case WM_RBUTTONUP:      processBtnMsg(MouseBtn::R, false);      break;
    case WM_MOUSEMOVE:      processCursorMsg();                     break;
    case WM_MOUSEWHEEL:     processScrollMsg(GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
                                                                    break;
    default:                return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int mInitWindow() {

    static TCHAR szAppName[] = TEXT("mao");
    HDC         hdc1;
    WNDCLASS    wndclass;
    RECT        rect = { 0, 0, w, h };
    LPVOID      ptr;        // ptr 将指向bitmap的首地址

    wndclass.style          = CS_HREDRAW|CS_VREDRAW;
    wndclass.lpfnWndProc    = processMsg;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = 0;
    wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = szAppName;
    if (!RegisterClass(&wndclass))          return -1;

    hwnd = CreateWindow(szAppName, TEXT("mRenderer"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, wndclass.hInstance, NULL);
    if (hwnd==NULL)           return -2;

    hdc1 = GetDC(hwnd);                      // hdc1是当前的句柄
    memory_dc = CreateCompatibleDC(hdc1);    // 创建新句柄0
    ReleaseDC(hwnd, hdc1);

    BITMAPINFO bitmapinfo = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB, DWORD(w * h * 4), 0, 0, 0, 0 } };
    bitmap0 = CreateDIBSection(memory_dc, &bitmapinfo, DIB_RGB_COLORS, _Out_ &ptr, 0, 0);
    if (bitmap0 == NULL)      return -3;

    HBITMAP bitmap1 = (HBITMAP)SelectObject(memory_dc, bitmap0);   // 新的0给旧的1
    DeleteObject(bitmap1);
    // pScr = (uchar *)ptr;                                

    int wx, wy, sx, sy;
    AdjustWindowRect(&rect, GetWindowLong(hwnd, GWL_STYLE), 0);
    wx = rect.right - rect.left;
    wy = rect.bottom - rect.top;
    sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
    sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
    if (sy < 0) sy = 0;
    SetWindowPos(hwnd, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
    SetForegroundWindow(hwnd);

    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);

    memset(ptr, 0, w * h * 4);

    mDevice::framebuffer = (uint**)malloc(w * h * 4);
    for (int i = 0; i < h; i++)
        mDevice::framebuffer[i] = (uint*)((uchar *)ptr + w * 4 * i);

    return 0;
}

void mDispatch() {
    MSG msg;
    while (true) {
        if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
        if (!GetMessage(&msg, NULL, 0, 0)) break;
        DispatchMessage(&msg);
    }
}

void mUpdateWindow() {
    HDC hdc = GetDC(hwnd);
    BitBlt(hdc, 0, 0, w, h, memory_dc, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdc);
    mDispatch();
}

void destroyWindow() {
    ShowWindow(hwnd, SW_HIDE);

    DeleteDC(memory_dc);
    DestroyWindow(hwnd);

    free(mDevice::framebuffer);
    // todo: free zbuffer
}

bool windowShouldClose() {
    return shouldClose;
}

void getCursorPos(float & xPos, float & yPos) {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(hwnd, &point);
    xPos = (float)point.x;
    yPos = (float)point.y;
}

//  v > win95
double getNativeTime() {
    static double period = -1;
    LARGE_INTEGER counter;
    if (period < 0) {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        period = 1 / (double)frequency.QuadPart;
    }
    QueryPerformanceCounter(&counter);
    return counter.QuadPart * period;
}