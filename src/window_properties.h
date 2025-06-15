#ifndef WINDOW_PROPERTIES_H_INCLUDED
#define WINDOW_PROPERTIES_H_INCLUDED

#include <string>

namespace org {

class window_properties {
    public:
        window_properties();
        int get_x() const;
        int get_y() const;
        int get_width() const;
        int get_height() const;
        std::string const& get_title() const;
        void set_x(int const &value);
        void set_y(int const &value);
        void set_width(int const &value);
        void set_height(int const &value);
        void set_title(std::string const &value);
    protected:
    private:
        int _M_x;
        int _M_y;
        int _M_width;
        int _M_height;
        std::string _M_title;
};

} // namespace org

#endif // WINDOW_PROPERTIES_H_INCLUDED
