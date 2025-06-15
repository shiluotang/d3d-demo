#ifndef D3D_CONTEXT_H_INCLUDED
#define D3D_CONTEXT_H_INCLUDED

#include <d3d9.h>

namespace org {

class window;
class d3d_context {
    public:
        d3d_context();
        virtual ~d3d_context();
        void init(window const&);

        LPDIRECT3D9 get_d3d() const;
        LPDIRECT3DDEVICE9 get_d3d_device() const;
        LPDIRECT3DVERTEXBUFFER9 get_d3d_vertex_buffer() const;
        D3DPRESENT_PARAMETERS const& get_d3d_present_parameters() const;

        void set_d3d(LPDIRECT3D9 value);
        void set_d3d_device(LPDIRECT3DDEVICE9 value);
        void set_d3d_vertex_buffer(LPDIRECT3DVERTEXBUFFER9 value);
        void set_d3d_present_parameters(D3DPRESENT_PARAMETERS const &value);
    protected:
        // the pointer to our Direct3D interface
        LPDIRECT3D9 _M_d3d;
        // the pointer to the device class
        LPDIRECT3DDEVICE9 _M_d3d_dev;
        // the pointer to the vertex buffer
        LPDIRECT3DVERTEXBUFFER9 _M_d3d_vertexbuffer;
        D3DPRESENT_PARAMETERS _M_d3d_present_parameters;
    private:
        d3d_context(d3d_context const&);
        d3d_context& operator=(d3d_context const&);
};

} // namespace org

#endif // D3D_CONTEXT_H_INCLUDED
