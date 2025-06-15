#include <windows.h>

#include "messages.h"
#include "logx.h"

#include "window.h"

namespace {

int USERDATA_INDEX = GWLP_USERDATA;

} // namespace anonymous

namespace org {

LRESULT CALLBACK window::WindowProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam)  {
    LRESULT rc = 0;
    window *wnd = find_current_window(hWnd);
    if (wnd) {
        rc = wnd->handle_message(message, wParam, lParam);
        return rc;
    } else {
        switch(message) {
            case WM_DESTROY: {
                PostQuitMessage(0);
                LOGD("PostQuitMessage(nExitCode = 0)");
                break;
            }
            default:
                rc = DefWindowProc(hWnd, message, wParam, lParam);
                LOGD("DefWindowProc(hWnd = " << hWnd
                        << ", Msg = " << nameOfMsg(message)
                        << ", wParam = " << wParam
                        << ", lParam = " << lParam
                        << ") = " << rc);
                break;
        }
    }
    // FIXME what's the default proper value for LRESULT
    return rc;
}

window::window(HINSTANCE hinstance)
    : _M_hinstance(hinstance)
    , _M_props()
    , _M_hwnd(0)
{
}

window_properties const& window::get_properties() const {
    return _M_props;
}

void window::set_properties(window_properties const &value) {
    _M_props = value;
}


HWND window::get_handle() const {
    return _M_hwnd;
}

void window::set_handle(HWND value) {
    _M_hwnd = value;
}

void window::create() {
    HWND hwnd;
    WNDCLASSEX wc;
    MSG msg;

    LPCSTR lpClassName = "WindowsClass";
    memset(&wc,0,sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = &window::WindowProc;
    wc.hInstance     = _M_hinstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszClassName = lpClassName;
    wc.hIcon         = NULL;
    wc.hIconSm       = NULL;
    ATOM rc = RegisterClassEx(&wc);
    LOGD("RegisterClassEx(wndClass = " << &wc << ") = " << rc);

    DWORD dwExStyle = 0;
    LPCSTR lpWindowName = _M_props.get_title().c_str();
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    int x = 0;
    int y = 0;
    int nWidth = _M_props.get_width();
    int nHeight = _M_props.get_height();
    HWND hWndParent = NULL;
    HMENU hMenu = NULL;
    LPVOID lpParam = NULL;
    hwnd = CreateWindowEx(
            dwExStyle,
            lpClassName,
            lpWindowName,
            dwStyle,
            x, y,
            nWidth, nHeight,
            hWndParent,
            hMenu,
            _M_hinstance,
            lpParam);
    LOGD("CreateWindowEx(dwExStyle = " << reinterpret_cast<void*>(dwExStyle)
            << ", lpClassName = " << lpClassName
            << ", dwStyle = " << reinterpret_cast<void*>(dwStyle)
            << ", X = " << x
            << ", Y = " << y
            << ", nWidth = " << nWidth
            << ", nHeight = " << nHeight
            << ", hWndParent = " << hWndParent
            << ", hMenu = " << hMenu
            << ", hInstance = " << _M_hinstance
            << ", lpParam = " << lpParam
            << ") = " << hwnd);
    _M_hwnd = hwnd;
    this->register_current_window();
}

void window::init() {
    this->create();
}

void window::set_visible(bool onoff) {
    if (!_M_hwnd)
        return;
    int nCmdShow = onoff ? SW_SHOW : SW_HIDE;
    WINBOOL rc = ShowWindow(_M_hwnd, nCmdShow);
    LOGD("ShowWindow(hWnd = " << _M_hwnd
            << ", nCmdShow = " << nCmdShow
            << ") = " << rc);
}

void window::show() {
    set_visible(true);
}

void window::hide() {
    set_visible(false);
}

void window::register_current_window() {
    if (!_M_hwnd)
        return;
    LONG_PTR rc = SetWindowLongPtr(
            _M_hwnd,
            USERDATA_INDEX,
            reinterpret_cast<LONG_PTR>(this));
    LOGD("SetWindowLongPtr(hWnd = " << _M_hwnd
            << ", nIndex = " << USERDATA_INDEX
            << ", dwNewLong = " << reinterpret_cast<LONG_PTR>(this)
            << ") = " << rc);
}

window* window::find_current_window(HWND hwnd) {
    LONG_PTR rc = GetWindowLongPtr(hwnd, USERDATA_INDEX);
    // LOGD("GetWindowLongPtr(hWnd = " << hwnd
    //         << ", nIndex = " << USERDATA_INDEX
    //         << ") = " << rc);
    return reinterpret_cast<window*>(rc);
}

LRESULT window::handle_message(UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT rc = 0;
    bool handled = true;
    switch (message) {
        case WM_CREATE:
            break;
        case WM_MOVE:
            break;
        case WM_SIZE:
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            LOGD("PostQuitMessage(nExitCode = " << 0 << ")");
            break;
        default:
            handled = false;
            break;
    }
    if (!handled) {
        rc = DefWindowProc(_M_hwnd, message, wParam, lParam);
        LOGD("DefWindowProc(hWnd = " << _M_hwnd
                << ", Msg = " << nameOfMsg(message)
                << ", wParam = " << wParam
                << ", lParam = " << lParam
                << ") = " << rc);
    }
    return rc;
}

} // namespace org
