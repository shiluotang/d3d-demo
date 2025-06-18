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
#include "glu_tess.h"

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

class polygon_test_renderer
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

        polygon_test_renderer() {
        }

        virtual
        void create_triangles() {
            GLdouble quad[][3] = {
                // CW (clockwise)
                { -2, 0, 1 },
                { -2, 3, 1 },
                { 2, 3, 1 },
                { 2, 0, 1 },

                // CCW (counter clockwise)
                { -1, 1, 2 },
                { -1, 2, 2 },
                {  1, 2, 2 },
                { 1, 1, 2 },
            };

            org::glu_tess tess;
            tess.begin_polyon();
            tess.begin_contour();
            tess.vertex(quad[0], quad[0]);
            tess.vertex(quad[1], quad[1]);
            tess.vertex(quad[2], quad[2]);
            tess.vertex(quad[3], quad[3]);
            // contour automatically connect the last vertex to the first one
            tess.end_contour();
            tess.begin_contour();
            tess.vertex(quad[4], quad[4]);
            tess.vertex(quad[5], quad[5]);
            tess.vertex(quad[6], quad[6]);
            tess.vertex(quad[7], quad[7]);
            tess.end_contour();
            tess.end_polyon();
            typedef org::glu_tess::triangles_type triangles_type;
            typedef triangles_type::const_iterator const_iterator;

            triangles_type const &t = tess.get_triangles();
            int idx = 0;
            D3DCOLOR colors[] = {
                D3DCOLOR_XRGB(0, 0, 255),
                D3DCOLOR_XRGB(0, 255, 0),
                D3DCOLOR_XRGB(255, 0, 0),
            };

            for (const_iterator it = t.begin(), e = t.end(); it != e; ++it) {
                CUSTOMVERTEX v[3];
                for (int i = 0, n = 3; i < n; ++i) {
                    v[i].X = static_cast<GLdouble*>((*it)[i])[0];
                    v[i].Y = static_cast<GLdouble*>((*it)[i])[1];
                    v[i].Z = static_cast<GLdouble*>((*it)[i])[2];
                    v[i].COLOR = colors[(idx++ % 3)];
                    _M_vertices.push_back(v[i]);
                }
            }
        }

        virtual
        void init(org::window &wnd, org::d3d_context &ctx) {
            HRESULT rc = 0;
            index = 0;
            LPDIRECT3DDEVICE9 d3ddev = ctx.get_d3d_device();
            LPDIRECT3DVERTEXBUFFER9 vertexbuffer = NULL;
            this->create_triangles();
            // create a vertex buffer interface called v_buffer
            rc = d3ddev->CreateVertexBuffer(
                    sizeof(_M_vertices[0]) * _M_vertices.size(),
                    0,
                    CUSTOMFVF,
                    D3DPOOL_MANAGED,
                    &vertexbuffer,
                    NULL);
            LOGD("IDirect3DDevice9::CreateVertexBuffer("
                    << "this = " << d3ddev
                    << ", Length = " << sizeof(_M_vertices[0]) * _M_vertices.size()
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
            std::memcpy(pVoid, &_M_vertices[0], sizeof(_M_vertices[0]) * _M_vertices.size());
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
            LOGD("IDirect3DDevice9::Clear(Count = " << 0
                    << ", pRects = " << static_cast<void*>(NULL)
                    << ", Flags = " << D3DCLEAR_TARGET
                    << ", Color = " << D3DCOLOR_XRGB(0, 0, 0)
                    << ", Z = " << 1.0f
                    << ", Stencil = " << 0
                    << ")");
            d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
            LOGD("IDirect3DDevice9::Clear(Count = " << 0
                    << ", pRects = " << static_cast<void*>(NULL)
                    << ", Flags = " << D3DCLEAR_ZBUFFER
                    << ", Color = " << D3DCOLOR_XRGB(0, 0, 0)
                    << ", Z = " << 1.0f
                    << ", Stencil = " << 0
                    << ")");

            d3ddev->BeginScene();
            LOGD("IDirect3DDevice9::BeginScene()");

            // select which vertex format we are using
            d3ddev->SetFVF(CUSTOMFVF);
            LOGD("IDirect3DDevice9::SetFVF(FVF = " << CUSTOMFVF << ")");

            // set the view transform
            D3DXMATRIX matView;    // the view transform matrix
            D3DXVECTOR3 camPos(0.0f, 0.0f, 15.0f);
            D3DXVECTOR3 lookAt(0.0f, 0.0f, 0.0f);
            D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

            D3DXMatrixLookAtLH(&matView, &camPos, &lookAt, &up);
            d3ddev->SetTransform(D3DTS_VIEW, &matView);
            LOGD("IDirect3DDevice9::SetTransform(State = " << D3DTS_VIEW
                    << ", pMatrix = " << &matView
                    << ")");

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
            LOGD("IDirect3DDevice9::SetTransform(State = " << D3DTS_PROJECTION
                    << ", pMatrix = " << &matProjection
                    << ")");

            // select the vertex buffer to display
            d3ddev->SetStreamSource(
                    0,
                    ctx.get_d3d_vertex_buffer(),
                    0,
                    sizeof(CUSTOMVERTEX));
            LOGD("IDirect3DDevice9::SetStreamSource(StreamNumber = " << 0
                    << ", pStreamData = " << ctx.get_d3d_vertex_buffer()
                    << ", OffsetInBytes = " << 0
                    << ", Stride = " << sizeof(CUSTOMVERTEX)
                    << ")");

            // a matrix to store the translation for triangle A
            D3DXMATRIX matTranslateA;
            // a matrix to store the translation for triangle B
            D3DXMATRIX matTranslateB;
            // a matrix to store the rotation for each triangle
            D3DXMATRIX matRotateY;
            // an ever-increasing float value
            index += 0.005f;

            // build MULTIPLE matrices to translate the model and one to rotate
            D3DXMatrixTranslation(&matTranslateA, 0.0f, 0.0f, 2.0f);
            D3DXMatrixTranslation(&matTranslateB, 0.0f, 0.0f, -2.0f);
            // the front side
            D3DXMatrixRotationY(&matRotateY, index);

            // tell Direct3D about each world transform, and then draw another triangle
            D3DMATRIX matTemp(matTranslateA * matRotateY);
            d3ddev->SetTransform(D3DTS_WORLD, &matTemp);
            LOGD("IDirect3DDevice9::SetTransform(State = " << D3DTS_WORLD
                    << ", pMatrix = " << &matProjection
                    << ")");
            UINT ntriangles = _M_vertices.size() / 3;
            d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ntriangles);
            LOGD("IDirect3DDevice9::DrawPrimitive"
                    << "(PrimitiveType = " << D3DPT_TRIANGLELIST
                    << ", StartVertex = " << 0
                    << ", PrimitiveCount = " << ntriangles
                    <<  ")");

            matTemp = matTranslateB * matRotateY;
            d3ddev->SetTransform(D3DTS_WORLD, &matTemp);
            LOGD("IDirect3DDevice9::SetTransform(State = " << D3DTS_WORLD
                    << ", pMatrix = " << &matTemp
                    << ")");
            d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ntriangles);
            LOGD("IDirect3DDevice9::DrawPrimitive"
                    << "(PrimitiveType = " << D3DPT_TRIANGLELIST
                    << ", StartVertex = " << 0
                    << ", PrimitiveCount = " << ntriangles
                    <<  ")");

            d3ddev->EndScene();
            LOGD("IDirect3DDevice9::EndScene()");
            d3ddev->Present(NULL, NULL, NULL, NULL);
            LOGD("IDirect3DDevice9::Present"
                    << "(pSourceRect = " << static_cast<void*>(NULL)
                    << ", pDestRect = " << static_cast<void*>(NULL)
                    << ", hDestWindowOverride = " << static_cast<void*>(NULL)
                    << ", pDirtyRegion = " << static_cast<void*>(NULL)
                    << ")");
        }
    protected:
        std::vector<CUSTOMVERTEX> _M_vertices;
    private:
        float index;
};

class concave_polygon_renderer
    : public polygon_test_renderer
{
    public:
        virtual
        void create_triangles() {
            GLdouble quad[][3] = {
                {-1,3,0},
                {0,0,1},
                {1,3,0},
                {0,2,1}
            };

            org::glu_tess tess;
            tess.begin_polyon();
            tess.begin_contour();
            tess.vertex(quad[0], quad[0]);
            tess.vertex(quad[1], quad[1]);
            tess.vertex(quad[2], quad[2]);
            tess.vertex(quad[3], quad[3]);
            // contour automatically connect the last vertex to the first one
            tess.end_contour();
            tess.end_polyon();
            typedef org::glu_tess::triangles_type triangles_type;
            typedef triangles_type::const_iterator const_iterator;

            triangles_type const &t = tess.get_triangles();
            int idx = 0;
            D3DCOLOR colors[] = {
                D3DCOLOR_XRGB(0, 0, 255),
                D3DCOLOR_XRGB(0, 255, 0),
                D3DCOLOR_XRGB(255, 0, 0),
            };

            for (const_iterator it = t.begin(), e = t.end(); it != e; ++it) {
                CUSTOMVERTEX v[3];
                for (int i = 0, n = 3; i < n; ++i) {
                    v[i].X = static_cast<GLdouble*>((*it)[i])[0];
                    v[i].Y = static_cast<GLdouble*>((*it)[i])[1];
                    v[i].Z = static_cast<GLdouble*>((*it)[i])[2];
                    v[i].COLOR = colors[(idx++ % 3)];
                    _M_vertices.push_back(v[i]);
                }
            }
        }
};

class hole_polygon_renderer
    : public polygon_test_renderer
{
    public:
        virtual
        void create_triangles() {
            GLdouble quad[][3] = {
                // CW (clockwise)
                { -2, 0, 1 },
                { -2, 3, 1 },
                { 2, 3, 1 },
                { 2, 0, 1 },

                // CCW (counter clockwise)
                { -1, 1, 2 },
                { -1, 2, 2 },
                {  1, 2, 2 },
                { 1, 1, 2 },
            };

            org::glu_tess tess;
            tess.begin_polyon();
            tess.begin_contour();
            tess.vertex(quad[0], quad[0]);
            tess.vertex(quad[1], quad[1]);
            tess.vertex(quad[2], quad[2]);
            tess.vertex(quad[3], quad[3]);
            // contour automatically connect the last vertex to the first one
            tess.end_contour();
            tess.begin_contour();
            tess.vertex(quad[4], quad[4]);
            tess.vertex(quad[5], quad[5]);
            tess.vertex(quad[6], quad[6]);
            tess.vertex(quad[7], quad[7]);
            tess.end_contour();
            tess.end_polyon();
            typedef org::glu_tess::triangles_type triangles_type;
            typedef triangles_type::const_iterator const_iterator;

            triangles_type const &t = tess.get_triangles();
            int idx = 0;
            D3DCOLOR colors[] = {
                D3DCOLOR_XRGB(0, 0, 255),
                D3DCOLOR_XRGB(0, 255, 0),
                D3DCOLOR_XRGB(255, 0, 0),
            };

            for (const_iterator it = t.begin(), e = t.end(); it != e; ++it) {
                CUSTOMVERTEX v[3];
                for (int i = 0, n = 3; i < n; ++i) {
                    v[i].X = static_cast<GLdouble*>((*it)[i])[0];
                    v[i].Y = static_cast<GLdouble*>((*it)[i])[1];
                    v[i].Z = static_cast<GLdouble*>((*it)[i])[2];
                    v[i].COLOR = colors[(idx++ % 3)];
                    _M_vertices.push_back(v[i]);
                }
            }
        }
};

class self_intersect_polygon_renderer
    : public polygon_test_renderer
{
    public:
        virtual
        void create_triangles() {
            // x,y,z,r,g,b
            GLdouble star[][6] = {
                {0.0, 3.0, 0,  1, 0, 0},
                {-1.0, 0.0, 0,  0, 1, 0},
                {1.6, 1.9, 0,  1, 0, 1},
                {-1.6, 1.9, 0,  1, 1, 0},
                {1.0, 0.0, 0,  0, 0, 1}
            };


            org::glu_tess tess;
            tess.begin_polyon();
            tess.begin_contour();
            tess.vertex(star[0], star[0]);
            tess.vertex(star[1], star[1]);
            tess.vertex(star[2], star[2]);
            tess.vertex(star[3], star[3]);
            tess.vertex(star[4], star[4]);
            // contour automatically connect the last vertex to the first one
            tess.end_contour();
            tess.end_polyon();
            typedef org::glu_tess::triangles_type triangles_type;
            typedef triangles_type::const_iterator const_iterator;

            triangles_type const &t = tess.get_triangles();
            int idx = 0;
            D3DCOLOR colors[] = {
                D3DCOLOR_XRGB(0, 0, 255),
                D3DCOLOR_XRGB(0, 255, 0),
                D3DCOLOR_XRGB(255, 0, 0),
            };

            for (const_iterator it = t.begin(), e = t.end(); it != e; ++it) {
                CUSTOMVERTEX v[3];
                for (int i = 0, n = 3; i < n; ++i) {
                    v[i].X = static_cast<GLdouble*>((*it)[i])[0];
                    v[i].Y = static_cast<GLdouble*>((*it)[i])[1];
                    v[i].Z = static_cast<GLdouble*>((*it)[i])[2];
                    v[i].COLOR = colors[(idx++ % 3)];
                    _M_vertices.push_back(v[i]);
                }
            }
        }
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
    // demo_renderer renderer;
    // concave_polygon_renderer renderer;
    // hole_polygon_renderer renderer;
    self_intersect_polygon_renderer renderer;

    application app(hInstance);
    app.set_renderer(&renderer);
    app.get_window().set_properties(props);
    app.init();
    int rc = app.run();
    return rc;
}
