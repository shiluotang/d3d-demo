#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <windows.h>

#include "window_properties.h"

namespace org {

class window {
    public:
        static
        LRESULT CALLBACK WindowProc(
                HWND hWnd,
                UINT message,
                WPARAM wParam,
                LPARAM lParam);

        explicit window(HINSTANCE hinstance);
        virtual ~window();
        window_properties const& get_properties() const;
        void set_properties(window_properties const &value);

        HWND get_handle() const;
        void set_handle(HWND value);

        void create();
        void run();
        void init();

        void set_visible(bool onoff);
        void show();
        void hide();

    protected:
        void register_class();
        void unregister_class();

        void register_current_window();

        static
        window* find_current_window(HWND hwnd);

        LRESULT handle_message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    private:
        HINSTANCE _M_hinstance;
        window_properties _M_props;
        HWND _M_hwnd;
        WNDCLASSEX _M_wndclass;
};

} // namespace org

#endif // WINDOW_H_INCLUDED
