#include <cstring>

#include <d3d9.h>

#include "enum_decl.h"
#include "window.h"
#include "logx.h"

#include "d3d_context.h"

namespace org {

ENUM_BEGIN_REP(PrimitiveType, D3DPRIMITIVETYPE)
ENUM_ITEM_REP(D3DPT_POINTLIST)
ENUM_ITEM_REP(D3DPT_LINELIST)
ENUM_ITEM_REP(D3DPT_LINESTRIP)
ENUM_ITEM_REP(D3DPT_TRIANGLELIST)
ENUM_ITEM_REP(D3DPT_TRIANGLESTRIP)
ENUM_ITEM_REP(D3DPT_TRIANGLEFAN)
ENUM_END_REP()

ENUM_BEGIN_REP(TransformState, D3DTRANSFORMSTATETYPE)
ENUM_ITEM_REP(D3DTS_VIEW)
ENUM_ITEM_REP(D3DTS_PROJECTION)
ENUM_ITEM_REP(D3DTS_TEXTURE0)
ENUM_ITEM_REP(D3DTS_TEXTURE1)
ENUM_ITEM_REP(D3DTS_TEXTURE2)
ENUM_ITEM_REP(D3DTS_TEXTURE3)
ENUM_ITEM_REP(D3DTS_TEXTURE4)
ENUM_ITEM_REP(D3DTS_TEXTURE5)
ENUM_ITEM_REP(D3DTS_TEXTURE6)
ENUM_ITEM_REP(D3DTS_TEXTURE7)

ENUM_ITEM_REP(D3DTS_WORLD)
ENUM_ITEM_REP(D3DTS_WORLD1)
ENUM_ITEM_REP(D3DTS_WORLD2)
ENUM_ITEM_REP(D3DTS_WORLD3)

ENUM_END_REP()

ENUM_BEGIN_REP(RenderState, D3DRENDERSTATETYPE)
    ENUM_ITEM_REP(D3DRS_ZENABLE                   )
    ENUM_ITEM_REP(D3DRS_FILLMODE                  )
    ENUM_ITEM_REP(D3DRS_SHADEMODE                 )
    ENUM_ITEM_REP(D3DRS_ZWRITEENABLE              )
    ENUM_ITEM_REP(D3DRS_ALPHATESTENABLE           )
    ENUM_ITEM_REP(D3DRS_LASTPIXEL                 )
    ENUM_ITEM_REP(D3DRS_SRCBLEND                  )
    ENUM_ITEM_REP(D3DRS_DESTBLEND                 )
    ENUM_ITEM_REP(D3DRS_CULLMODE                  )
    ENUM_ITEM_REP(D3DRS_ZFUNC                     )
    ENUM_ITEM_REP(D3DRS_ALPHAREF                  )
    ENUM_ITEM_REP(D3DRS_ALPHAFUNC                 )
    ENUM_ITEM_REP(D3DRS_DITHERENABLE              )
    ENUM_ITEM_REP(D3DRS_ALPHABLENDENABLE          )
    ENUM_ITEM_REP(D3DRS_FOGENABLE                 )
    ENUM_ITEM_REP(D3DRS_SPECULARENABLE            )
    ENUM_ITEM_REP(D3DRS_FOGCOLOR                  )
    ENUM_ITEM_REP(D3DRS_FOGTABLEMODE              )
    ENUM_ITEM_REP(D3DRS_FOGSTART                  )
    ENUM_ITEM_REP(D3DRS_FOGEND                    )
    ENUM_ITEM_REP(D3DRS_FOGDENSITY                )
    ENUM_ITEM_REP(D3DRS_RANGEFOGENABLE            )
    ENUM_ITEM_REP(D3DRS_STENCILENABLE             )
    ENUM_ITEM_REP(D3DRS_STENCILFAIL               )
    ENUM_ITEM_REP(D3DRS_STENCILZFAIL              )
    ENUM_ITEM_REP(D3DRS_STENCILPASS               )
    ENUM_ITEM_REP(D3DRS_STENCILFUNC               )
    ENUM_ITEM_REP(D3DRS_STENCILREF                )
    ENUM_ITEM_REP(D3DRS_STENCILMASK               )
    ENUM_ITEM_REP(D3DRS_STENCILWRITEMASK          )
    ENUM_ITEM_REP(D3DRS_TEXTUREFACTOR             )
    ENUM_ITEM_REP(D3DRS_WRAP0                     )
    ENUM_ITEM_REP(D3DRS_WRAP1                     )
    ENUM_ITEM_REP(D3DRS_WRAP2                     )
    ENUM_ITEM_REP(D3DRS_WRAP3                     )
    ENUM_ITEM_REP(D3DRS_WRAP4                     )
    ENUM_ITEM_REP(D3DRS_WRAP5                     )
    ENUM_ITEM_REP(D3DRS_WRAP6                     )
    ENUM_ITEM_REP(D3DRS_WRAP7                     )
    ENUM_ITEM_REP(D3DRS_CLIPPING                  )
    ENUM_ITEM_REP(D3DRS_LIGHTING                  )
    ENUM_ITEM_REP(D3DRS_AMBIENT                   )
    ENUM_ITEM_REP(D3DRS_FOGVERTEXMODE             )
    ENUM_ITEM_REP(D3DRS_COLORVERTEX               )
    ENUM_ITEM_REP(D3DRS_LOCALVIEWER               )
    ENUM_ITEM_REP(D3DRS_NORMALIZENORMALS          )
    ENUM_ITEM_REP(D3DRS_DIFFUSEMATERIALSOURCE     )
    ENUM_ITEM_REP(D3DRS_SPECULARMATERIALSOURCE    )
    ENUM_ITEM_REP(D3DRS_AMBIENTMATERIALSOURCE     )
    ENUM_ITEM_REP(D3DRS_EMISSIVEMATERIALSOURCE    )
    ENUM_ITEM_REP(D3DRS_VERTEXBLEND               )
    ENUM_ITEM_REP(D3DRS_CLIPPLANEENABLE           )
    ENUM_ITEM_REP(D3DRS_POINTSIZE                 )
    ENUM_ITEM_REP(D3DRS_POINTSIZE_MIN             )
    ENUM_ITEM_REP(D3DRS_POINTSPRITEENABLE         )
    ENUM_ITEM_REP(D3DRS_POINTSCALEENABLE          )
    ENUM_ITEM_REP(D3DRS_POINTSCALE_A              )
    ENUM_ITEM_REP(D3DRS_POINTSCALE_B              )
    ENUM_ITEM_REP(D3DRS_POINTSCALE_C              )
    ENUM_ITEM_REP(D3DRS_MULTISAMPLEANTIALIAS      )
    ENUM_ITEM_REP(D3DRS_MULTISAMPLEMASK           )
    ENUM_ITEM_REP(D3DRS_PATCHEDGESTYLE            )
    ENUM_ITEM_REP(D3DRS_DEBUGMONITORTOKEN         )
    ENUM_ITEM_REP(D3DRS_POINTSIZE_MAX             )
    ENUM_ITEM_REP(D3DRS_INDEXEDVERTEXBLENDENABLE  )
    ENUM_ITEM_REP(D3DRS_COLORWRITEENABLE          )
    ENUM_ITEM_REP(D3DRS_TWEENFACTOR               )
    ENUM_ITEM_REP(D3DRS_BLENDOP                   )
    ENUM_ITEM_REP(D3DRS_POSITIONDEGREE            )
    ENUM_ITEM_REP(D3DRS_NORMALDEGREE              )
    ENUM_ITEM_REP(D3DRS_SCISSORTESTENABLE         )
    ENUM_ITEM_REP(D3DRS_SLOPESCALEDEPTHBIAS       )
    ENUM_ITEM_REP(D3DRS_ANTIALIASEDLINEENABLE     )
    ENUM_ITEM_REP(D3DRS_MINTESSELLATIONLEVEL      )
    ENUM_ITEM_REP(D3DRS_MAXTESSELLATIONLEVEL      )
    ENUM_ITEM_REP(D3DRS_ADAPTIVETESS_X            )
    ENUM_ITEM_REP(D3DRS_ADAPTIVETESS_Y            )
    ENUM_ITEM_REP(D3DRS_ADAPTIVETESS_Z            )
    ENUM_ITEM_REP(D3DRS_ADAPTIVETESS_W            )
    ENUM_ITEM_REP(D3DRS_ENABLEADAPTIVETESSELLATION)
    ENUM_ITEM_REP(D3DRS_TWOSIDEDSTENCILMODE       )
    ENUM_ITEM_REP(D3DRS_CCW_STENCILFAIL           )
    ENUM_ITEM_REP(D3DRS_CCW_STENCILZFAIL          )
    ENUM_ITEM_REP(D3DRS_CCW_STENCILPASS           )
    ENUM_ITEM_REP(D3DRS_CCW_STENCILFUNC           )
    ENUM_ITEM_REP(D3DRS_COLORWRITEENABLE1         )
    ENUM_ITEM_REP(D3DRS_COLORWRITEENABLE2         )
    ENUM_ITEM_REP(D3DRS_COLORWRITEENABLE3         )
    ENUM_ITEM_REP(D3DRS_BLENDFACTOR               )
    ENUM_ITEM_REP(D3DRS_SRGBWRITEENABLE           )
    ENUM_ITEM_REP(D3DRS_DEPTHBIAS                 )
    ENUM_ITEM_REP(D3DRS_WRAP8                     )
    ENUM_ITEM_REP(D3DRS_WRAP9                     )
    ENUM_ITEM_REP(D3DRS_WRAP10                    )
    ENUM_ITEM_REP(D3DRS_WRAP11                    )
    ENUM_ITEM_REP(D3DRS_WRAP12                    )
    ENUM_ITEM_REP(D3DRS_WRAP13                    )
    ENUM_ITEM_REP(D3DRS_WRAP14                    )
    ENUM_ITEM_REP(D3DRS_WRAP15                    )
    ENUM_ITEM_REP(D3DRS_SEPARATEALPHABLENDENABLE  )
    ENUM_ITEM_REP(D3DRS_SRCBLENDALPHA             )
    ENUM_ITEM_REP(D3DRS_DESTBLENDALPHA            )
    ENUM_ITEM_REP(D3DRS_BLENDOPALPHA              )
ENUM_END_REP()


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
            << ", State = " << nameOfRenderState(D3DRS_LIGHTING)
            << ", Value = " << false
            << ") = " << rc);
    // both sides of the triangles
    rc = d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    LOGD("IDirect3DDevice9::SetRenderState("
            << "this = " << d3ddev
            << ", State = " << nameOfRenderState(D3DRS_CULLMODE)
            << ", Value = " << D3DCULL_NONE
            << ") = " << rc);
    // turn on the z-buffer
    rc = d3ddev->SetRenderState(D3DRS_ZENABLE,  true);
    LOGD("IDirect3DDevice9::SetRenderState("
            << "this = " << d3ddev
            << ", State = " << nameOfRenderState(D3DRS_ZENABLE)
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
