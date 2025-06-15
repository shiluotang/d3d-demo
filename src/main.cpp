// make sure to link to the libraries libd3d9.a and libd3dx9_43.a
// - copy the two files from the lib directory of your compiler
// - open project options >> parameters and add both using "Add Library or Object"

#include <cstring>
#include <fstream>

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "logx.h"

#include "renderer.h"
#include "d3d_context.h"
#include "application.h"

namespace {

class demo_renderer
    : public org::renderer
{
    public:
        struct CUSTOMVERTEX {
            float X;
            float Y;
            float Z;
            DWORD COLOR;
        };

        static const int CUSTOMFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

        virtual
        void init(org::window &wnd, org::d3d_context &ctx) {
            HRESULT rc = 0;
            index = 0;
            LPDIRECT3DDEVICE9 d3ddev = ctx.get_d3d_device();
            LPDIRECT3DVERTEXBUFFER9 vertexbuffer = NULL;
            // create the vertices using the CUSTOMVERTEX struct
            CUSTOMVERTEX vertices[] =
            {
                { 3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255)},
                { 0.0f,  3.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0)},
                {-3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0)},
            };

            // create a vertex buffer interface called v_buffer
            rc = d3ddev->CreateVertexBuffer(
                    3 * sizeof(CUSTOMVERTEX),
                    0,
                    CUSTOMFVF,
                    D3DPOOL_MANAGED,
                    &vertexbuffer,
                    NULL);
            LOGD("IDirect3DDevice9::CreateVertexBuffer("
                    << "this = " << d3ddev
                    << ", Length = " << 3 * sizeof(CUSTOMVERTEX)
                    << ", Usage = " << 0
                    << ", FVF = " << CUSTOMFVF
                    << ", Pool = " << D3DPOOL_MANAGED
                    << ", ppVertexBuffer = " << &vertexbuffer
                    << "(" << vertexbuffer << ")"
                    << ", pSharedHandle = " << static_cast<void*>(NULL)
                    << ") = " << rc);

            VOID* pVoid;
            // lock v_buffer and load the vertices into it
            rc = vertexbuffer->Lock(
                    0,
                    0,
                    reinterpret_cast<void**>(&pVoid),
                    0);
            LOGD("IDirect3DVertexBuffer9::Lock("
                    << "this = " << vertexbuffer
                    << ", OffsetToLock = " << 0
                    << ", SizeToLock = " << 0
                    << ", ppbData = " << &pVoid
                    << "(" << pVoid << ")"
                    << ", Flags = " << 0
                    << ") = " << rc);
            std::memcpy(pVoid, vertices, sizeof(vertices));
            rc = vertexbuffer->Unlock();
            ctx.set_d3d_vertex_buffer(vertexbuffer);
            LOGD("IDirect3DVertexBuffer9::Unlock("
                    << "this = " << vertexbuffer
                    << ") = " << rc);
        }

        virtual
        void update(org::window &wnd, org::d3d_context &ctx) {
            LPDIRECT3DDEVICE9 d3ddev = ctx.get_d3d_device();
            d3ddev->Clear(0, NULL, D3DCLEAR_TARGET,  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
            d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

            d3ddev->BeginScene();

            // select which vertex format we are using
            d3ddev->SetFVF(CUSTOMFVF);

            // set the view transform
            D3DXMATRIX matView;    // the view transform matrix
            D3DXVECTOR3 camPos(0.0f, 0.0f, 15.0f);
            D3DXVECTOR3 lookAt(0.0f, 0.0f, 0.0f);
            D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

            D3DXMatrixLookAtLH(&matView, &camPos, &lookAt, &up);
            d3ddev->SetTransform(D3DTS_VIEW, &matView);

            // set the projection transform
            // the projection transform matrix
            D3DXMATRIX matProjection;
            org::window_properties const &props = wnd.get_properties();
            double fovy = D3DXToRadian(45);
            double aspect_ratio = static_cast<double>(props.get_width()) / props.get_height();
            double znear = 1;
            double zfar = 100;
            D3DXMatrixPerspectiveFovLH(
                    &matProjection,
                    fovy,
                    aspect_ratio,
                    znear,
                    zfar);
            d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

            // select the vertex buffer to display
            d3ddev->SetStreamSource(
                    0,
                    ctx.get_d3d_vertex_buffer(),
                    0,
                    sizeof(CUSTOMVERTEX));

            // a matrix to store the translation for triangle A
            D3DXMATRIX matTranslateA;
            // a matrix to store the translation for triangle B
            D3DXMATRIX matTranslateB;
            // a matrix to store the rotation for each triangle
            D3DXMATRIX matRotateY;
            // an ever-increasing float value
            index += 0.05f;

            // build MULTIPLE matrices to translate the model and one to rotate
            D3DXMatrixTranslation(&matTranslateA, 0.0f, 0.0f, 2.0f);
            D3DXMatrixTranslation(&matTranslateB, 0.0f, 0.0f, -2.0f);
            // the front side
            D3DXMatrixRotationY(&matRotateY, index);

            // tell Direct3D about each world transform, and then draw another triangle
            D3DMATRIX matTemp(matTranslateA * matRotateY);
            d3ddev->SetTransform(D3DTS_WORLD, &matTemp);
            d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

            matTemp = matTranslateB * matRotateY;
            d3ddev->SetTransform(D3DTS_WORLD, &matTemp);
            d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

            d3ddev->EndScene();
            d3ddev->Present(NULL, NULL, NULL, NULL);
        }
    protected:
    private:
        float index;
};

} // namespace anonymous


std::ofstream outfile("dx.log");
// the entry point for any Windows program
int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow) {
    using org::application;
    using org::window_properties;
    logx::_S_logstream = &outfile;

    window_properties props;
    props.set_title("MyTest");
    demo_renderer renderer;

    application app(hInstance);
    app.set_renderer(&renderer);
    app.get_window().set_properties(props);
    app.init();
    int rc = app.run();
    return rc;
}
