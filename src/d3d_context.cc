#include <cstring>

#include <d3d9.h>

#include "window.h"
#include "logx.h"
#include "d3d_context.h"

namespace org {

d3d_context::d3d_context()
    : _M_d3d(NULL)
    , _M_d3d_dev(NULL)
    , _M_d3d_vertexbuffer(NULL)
    , _M_d3d_present_parameters()
{
}

d3d_context::~d3d_context() {
    // close and release the vertex buffer
    if (_M_d3d_vertexbuffer)
        _M_d3d_vertexbuffer->Release();
    _M_d3d_vertexbuffer = NULL;
    // close and release the 3D device
    if (_M_d3d_dev)
        _M_d3d_dev->Release();
    _M_d3d_dev = NULL;
    // close and release Direct3D
    if (_M_d3d)
        _M_d3d->Release();
    _M_d3d = NULL;
}

void d3d_context::init(window const &wnd) {
    HRESULT rc = 0;
    LPDIRECT3D9 d3d = NULL;
    LPDIRECT3DDEVICE9 d3ddev = NULL;
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    LOGD("Direct3DCreate9(SDKVersion = " << D3D_SDK_VERSION << ") = " << d3d);

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed                  = true;
    d3dpp.SwapEffect                = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow             = wnd.get_handle();
    d3dpp.BackBufferFormat          = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth           = wnd.get_properties().get_width();
    d3dpp.BackBufferHeight          = wnd.get_properties().get_height();
    // Manage SetDepthStencil for us
    d3dpp.EnableAutoDepthStencil    = true;
    // 16-bit pixel format for the z-buffer
    d3dpp.AutoDepthStencilFormat    = D3DFMT_D16;

    // create a device class using this information and the info from the d3dpp stuct
    rc = d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            wnd.get_handle(),
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp,
            &d3ddev);
    LOGD("IDirect3DDevice9::CreateDevice("
            << "this = " << d3d
            << ", Adapter = " << D3DADAPTER_DEFAULT
            << ", DeviceType = " << D3DDEVTYPE_HAL
            << ", hFocusWindow = " << wnd.get_handle()
            << ", BehaviorFlags = " << D3DCREATE_SOFTWARE_VERTEXPROCESSING
            << ", pPresentationParameters = " << &d3dpp
            << ", ppReturnedDeviceInterface = " << &d3ddev
            << "(" << d3ddev << ")"
            << ") = " << rc);
    _M_d3d = d3d;
    _M_d3d_dev = d3ddev;
    std::memcpy(&_M_d3d_present_parameters, &d3dpp, sizeof(d3dpp));

    // Create resources
    // init_graphics();

    // turn off the 3D lighting
    rc = d3ddev->SetRenderState(D3DRS_LIGHTING, false);
    LOGD("IDirect3DDevice9::SetRenderState("
            << "this = " << d3ddev
            << ", State = " << D3DRS_LIGHTING
            << ", Value = " << false
            << ") = " << rc);
    // both sides of the triangles
    rc = d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    LOGD("IDirect3DDevice9::SetRenderState("
            << "this = " << d3ddev
            << ", State = " << D3DRS_CULLMODE
            << ", Value = " << D3DCULL_NONE
            << ") = " << rc);
    // turn on the z-buffer
    rc = d3ddev->SetRenderState(D3DRS_ZENABLE,  true);
    LOGD("IDirect3DDevice9::SetRenderState("
            << "this = " << d3ddev
            << ", State = " << D3DRS_ZENABLE
            << ", Value = " << true
            << ") = " << rc);
}

LPDIRECT3D9
d3d_context::get_d3d() const {
    return _M_d3d;
}

LPDIRECT3DDEVICE9
d3d_context::get_d3d_device() const {
    return _M_d3d_dev;
}

LPDIRECT3DVERTEXBUFFER9
d3d_context::get_d3d_vertex_buffer() const {
    return _M_d3d_vertexbuffer;
}

D3DPRESENT_PARAMETERS const&
d3d_context::get_d3d_present_parameters() const {
    return _M_d3d_present_parameters;
}

void d3d_context::set_d3d(LPDIRECT3D9 value) {
    _M_d3d = value;
}

void d3d_context::set_d3d_device(LPDIRECT3DDEVICE9 value) {
    _M_d3d_dev = value;
}

void d3d_context::set_d3d_vertex_buffer(LPDIRECT3DVERTEXBUFFER9 value) {
    _M_d3d_vertexbuffer = value;
}

void d3d_context::set_d3d_present_parameters(
        D3DPRESENT_PARAMETERS const &value) {
    std::memcpy(&_M_d3d_present_parameters, &value, sizeof(value));
}

} // namespace org
