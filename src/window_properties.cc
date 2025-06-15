#include "window_properties.h"

namespace org {

window_properties::window_properties()
    : _M_x(0)
    , _M_y(0)
    , _M_width(800)
    , _M_height(600)
    , _M_title("Untitled")
{
}

int window_properties::get_x() const { return _M_x; }
int window_properties::get_y() const { return _M_y; }
int window_properties::get_width() const { return _M_width; }
int window_properties::get_height() const { return _M_height; }
std::string const& window_properties::get_title() const { return _M_title; }
void window_properties::set_x(int const &value) { _M_x = value; }
void window_properties::set_y(int const &value) { _M_y = value; }
void window_properties::set_width(int const &value) { _M_width = value; }
void window_properties::set_height(int const &value) { _M_height = value; }
void window_properties::set_title(std::string const &value) { _M_title = value; }

} // namespace org 
