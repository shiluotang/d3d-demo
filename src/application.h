#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <windows.h>

#include "window.h"
#include "d3d_context.h"

namespace org {

class renderer;
class application {
    public:
        explicit application(HINSTANCE hInstance);
        virtual ~application();

        renderer* get_renderer() const;
        void set_renderer(renderer *value);

        window const& get_window() const;
        window& get_window();
        d3d_context const& get_d3d_context() const;
        d3d_context& get_d3d_context();

        void init();
        int run(int secs = 0);
    protected:
        int run_forever();
        int run_for(unsigned int secs);
    private:
        window _M_window;
        d3d_context _M_d3d_ctx;
        renderer *_M_renderer;

        application(application const&);
        application& operator=(application const&);
};

} // namespace org

#endif // APPLICATION_H_INCLUDED
