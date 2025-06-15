#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

namespace org {

class window;
class d3d_context;

class renderer {
    public:
        virtual
        void init(window &wnd, d3d_context &ctx);

        virtual
        void update(window &wnd, d3d_context &ctx);
    protected:
    private:
};

} // namespace org

#endif // RENDERER_H_INCLUDED
