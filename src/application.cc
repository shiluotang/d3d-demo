#include <ctime>
#include <windows.h>

#include "window.h"
#include "renderer.h"
#include "application.h"

namespace org {

application::application(HINSTANCE hInstance)
    : _M_window(hInstance)
    , _M_d3d_ctx()
    , _M_renderer(NULL)
{
}

renderer* application::get_renderer() const {
    return _M_renderer;
}

void application::set_renderer(renderer *value) {
    _M_renderer = value;
}

window const& application::get_window() const {
    return _M_window;
}

window& application::get_window() {
    return _M_window;
}

d3d_context const& application::get_d3d_context() const {
    return _M_d3d_ctx;
}

d3d_context& application::get_d3d_context() {
    return _M_d3d_ctx;
}

void application::init() {
    _M_window.init();
    _M_d3d_ctx.init(_M_window);
    if (_M_renderer)
        _M_renderer->init(_M_window, _M_d3d_ctx);
}

application::~application() {
}

int application::run_forever() {
    _M_window.show();
    MSG msg;
    while (msg.message != WM_QUIT) {
        // There's a WM_TIMER after WM_QUIT, so this internal loop will quit
        // with msg.message == WM_TIMER which means never exit the outer loop.
        while (PeekMessage(
                    &msg,
                    NULL,
                    0,
                    0,
                    PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
            continue;
        if (_M_renderer)
            _M_renderer->update(_M_window, _M_d3d_ctx);
    }
    return msg.wParam;
}

int application::run_for(unsigned int secs) {
    _M_window.show();
    MSG msg;
    time_t t0 = std::time(NULL);
    time_t t1 = t0;
    while (msg.message != WM_QUIT
            && std::difftime(t1, t0) < secs) {
        // There's a WM_TIMER after WM_QUIT, so this internal loop will quit
        // with msg.message == WM_TIMER which means never exit the outer loop.
        while (PeekMessage(
                    &msg,
                    NULL,
                    0,
                    0,
                    PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
            continue;
        if (_M_renderer)
            _M_renderer->update(_M_window, _M_d3d_ctx);
        t1 = std::time(NULL);
    }
    return msg.wParam;
}

int application::run(int secs) {
    if (secs <= 0)
        return run_forever();
    return run_for(secs);
}

} // namespace org
