#include <cassert>
#include <cstring>
#include <stdexcept>
#include <memory>
// #include <vector>
#include <deque>

#include <GL/gl.h>
#include <GL/glu.h>

#include "logx.h"

#include "enum_decl.h"

#include "glu_tess_raw.h"

namespace {

ENUM_BEGIN_REP(GLType, GLenum)
ENUM_ITEM_REP(GL_POINTS)
ENUM_ITEM_REP(GL_LINES)
ENUM_ITEM_REP(GL_LINE_LOOP)
ENUM_ITEM_REP(GL_LINE_STRIP)
ENUM_ITEM_REP(GL_TRIANGLES)
ENUM_ITEM_REP(GL_TRIANGLE_STRIP)
ENUM_ITEM_REP(GL_TRIANGLE_FAN)
ENUM_ITEM_REP(GL_QUADS)
ENUM_ITEM_REP(GL_QUAD_STRIP)
ENUM_ITEM_REP(GL_POLYGON)
ENUM_END_REP()

ENUM_BEGIN_REP(Tess, GLenum)
ENUM_ITEM_REP(GLU_TESS_BEGIN)
ENUM_ITEM_REP(GLU_TESS_VERTEX)
ENUM_ITEM_REP(GLU_TESS_END)
ENUM_ITEM_REP(GLU_TESS_ERROR)
ENUM_ITEM_REP(GLU_TESS_EDGE_FLAG)
ENUM_ITEM_REP(GLU_TESS_COMBINE)
ENUM_ITEM_REP(GLU_TESS_BEGIN_DATA)
ENUM_ITEM_REP(GLU_TESS_VERTEX_DATA)
ENUM_ITEM_REP(GLU_TESS_END_DATA)
ENUM_ITEM_REP(GLU_TESS_ERROR_DATA)
ENUM_ITEM_REP(GLU_TESS_EDGE_FLAG_DATA)
ENUM_ITEM_REP(GLU_TESS_COMBINE_DATA)

ENUM_ITEM_REP(GLU_TESS_WINDING_RULE)
ENUM_ITEM_REP(GLU_TESS_BOUNDARY_ONLY)
ENUM_ITEM_REP(GLU_TESS_TOLERANCE)
ENUM_END_REP()


template <typename T>
class array_view {
    public:
        typedef T value_type;
        typedef T *pointer;
        typedef T const *const_pointer;
        typedef T &reference;
        typedef T const &const_reference;

        array_view(void *p, size_t n)
            :_M_addr(static_cast<pointer>(p)), _M_size(n)
        {
        }

        T const& operator[](size_t n) const { return _M_addr[n]; }

        void print(std::ostream &out) const {
            out << "[";
            for (size_t i = 0, n = _M_size; i < n; ++i) {
                if (i != 0)
                    out << ", ";
                out << _M_addr[i];
            }
            out << "]";
        }

    protected:
    private:
        pointer _M_addr;
        size_t _M_size;
};

template <typename T>
std::ostream& operator<<(std::ostream &os, array_view<T> const &view) {
    view.print(os);
    return os;
}

struct arrays {
template <typename T, size_t N>
static
size_t countof(T (&a)[N]) {
    return N;
}

template <typename E>
static
array_view<E> view(size_t n, E *p) {
    return array_view<E>(p, n);
}
};

} // namespace anonymous

namespace org {

glu_tess_raw::tess_triangle::tess_triangle(
        tess_point const &a,
        tess_point const &b,
        tess_point const &c)
    : _M_a(a)
    , _M_b(b)
    , _M_c(c)
{
}

glu_tess_raw::tess_point
glu_tess_raw::tess_triangle::operator[](size_t n) const {
    switch (n) {
        case 0: return _M_a;
        case 1: return _M_b;
        case 2: return _M_c;
        default: break;
    }
    throw std::out_of_range("n");
}

void glu_tess_raw::tess_triangle::print(std::ostream &out) const {
    out << "[" << _M_a << ", " << _M_b << ", " << _M_c << "]";
}

glu_tess_raw::tess_line::tess_line(
        tess_point const &a,
        tess_point const &b)
    : _M_a(a)
    , _M_b(b)
{
}

glu_tess_raw::tess_point
glu_tess_raw::tess_line::operator[](size_t n) const {
    switch (n) {
        case 0: return _M_a;
        case 1: return _M_b;
        default: break;
    }
    throw std::out_of_range("n");
}

void glu_tess_raw::tess_line::print(std::ostream &out) const {
    out << "[" << _M_a << ", " << _M_b << "]";
}

glu_tess_raw::collector::collector(glu_tess_raw *tess)
    : _M_tess(tess)
{
}

void glu_tess_raw::collector::begin() {
}

void glu_tess_raw::collector::end() {
}

void glu_tess_raw::collector::collect(tess_point const &tp) {
}

glu_tess_raw::triangle_fan_collector::triangle_fan_collector(glu_tess_raw *tess)
    : collector(tess)
{
}

void glu_tess_raw::triangle_fan_collector::collect(tess_point const &p) {
    if (_M_triangle_points.size() < 3)
        _M_triangle_points.push_back(p);
    if (_M_triangle_points.size() == 3) {
        tess_triangle t(
                _M_triangle_points[0],
                _M_triangle_points[1],
                _M_triangle_points[2]);
        _M_tess->handle_triangle_collected(t);
        tess_point tp = _M_triangle_points.back();
        _M_triangle_points.pop_back();
        _M_triangle_points.pop_back();
        _M_triangle_points.push_back(tp);
    }
}

glu_tess_raw::triangle_strip_collector::triangle_strip_collector(
        glu_tess_raw *tess) :collector(tess)
{
}

void glu_tess_raw::triangle_strip_collector::end() {
    assert(_M_triangle_points.size() < 3);
}

void glu_tess_raw::triangle_strip_collector::collect(tess_point const &p) {
    if (_M_triangle_points.size() < 3)
        _M_triangle_points.push_back(p);
    if (_M_triangle_points.size() == 3) {
        tess_triangle t(
                _M_triangle_points[0],
                _M_triangle_points[1],
                _M_triangle_points[2]);
        _M_tess->handle_triangle_collected(t);
        _M_triangle_points.pop_front();
    }
}

glu_tess_raw::triangles_collector::triangles_collector(glu_tess_raw *tess)
    : collector(tess) {
}

void glu_tess_raw::triangles_collector::end() {
    assert(_M_triangle_points.size() == 0);
}

void glu_tess_raw::triangles_collector::collect(tess_point const &p) {
    if (_M_triangle_points.size() < 3)
        _M_triangle_points.push_back(p);
    if (_M_triangle_points.size() == 3) {
        tess_triangle t(
                _M_triangle_points[0],
                _M_triangle_points[1],
                _M_triangle_points[2]);
        _M_tess->handle_triangle_collected(t);
        _M_triangle_points.clear();
    }
}

glu_tess_raw::line_loop_collector::line_loop_collector(glu_tess_raw *tess)
    : collector(tess) {
}

void glu_tess_raw::line_loop_collector::begin() {
    _M_starting = true;
}

void glu_tess_raw::line_loop_collector::end() {
    if (_M_line_points.size() == 1) {
        // connect to the first point
        tess_line line(_M_line_points[0], _M_p0);
        _M_tess->handle_line_collected(line);
    }
}

void glu_tess_raw::line_loop_collector::collect(tess_point const &p) {
    if (_M_starting) {
        _M_p0 = p;
        _M_starting = false;
    }
    if (_M_line_points.size() < 2)
        _M_line_points.push_back(p);
    if (_M_line_points.size() == 2) {
        tess_line line(_M_line_points[0], _M_line_points[1]);
        _M_tess->handle_line_collected(line);
        _M_line_points.pop_front();
    }
}

glu_tess_raw::glu_tess_raw()
    :_M_tess(NULL)
{
    _M_tess = gluNewTess();
    LOGD("gluNewTess() = " << _M_tess);
    if (!_M_tess)
        throw std::runtime_error("gluNewTess");
    this->setup_callbacks();
}

glu_tess_raw::~glu_tess_raw() {
    teardown_callbacks();
    if (_M_tess) {
        gluDeleteTess(_M_tess);
        LOGD("gluDeleteTess(" << _M_tess << ")");
    }
    _M_tess = NULL;
}

void CALLBACK glu_tess_raw::tess_begin_callback(GLenum type, void *data) {
    glu_tess_raw *t = reinterpret_cast<glu_tess_raw*>(data);
    t->on_tess_begin(type);
}

void CALLBACK glu_tess_raw::tess_end_callback(void *data) {
    glu_tess_raw *t = reinterpret_cast<glu_tess_raw*>(data);
    t->on_tess_end();
}

void CALLBACK glu_tess_raw::tess_vertex_callback(void *vertex, void *data) {
    glu_tess_raw *t = reinterpret_cast<glu_tess_raw*>(data);
    t->on_tess_vertex(vertex);
}

void CALLBACK glu_tess_raw::tess_error_callback(GLenum ecode, void *data) {
    glu_tess_raw *t = reinterpret_cast<glu_tess_raw*>(data);
    t->on_tess_error(ecode);
}

void CALLBACK glu_tess_raw::tess_combine_callback(
        GLdouble coords[3],
        void *vertex_data[4],
        GLfloat weight[4],
        void **outData,
        void *polygon_data) {
    glu_tess_raw *t = reinterpret_cast<glu_tess_raw*>(polygon_data);
    t->on_tess_combine(coords, vertex_data, weight, outData);
}

void glu_tess_raw::set_callback0(GLenum which, tess_callback cb) {
    gluTessCallback(_M_tess, which, cb);
    LOGD("gluTessCallback(tess = " << _M_tess
            << ", which = " << nameOfTess(which)
            << ", fn = " << reinterpret_cast<void*>(cb)
            << ")");
}

void glu_tess_raw::set_property(GLenum which, GLdouble value) {
    gluTessProperty(_M_tess, which, value);
    LOGD("gluTessProperty(tess = " << _M_tess
            << ", which = " << nameOfTess(which)
            << ", value = " << value
            << ")");
}

void glu_tess_raw::setup_callbacks() {
    if (!_M_tess)
        return;
    LOGD(__PRETTY_FUNCTION__);
    set_callback(GLU_TESS_BEGIN_DATA, &tess_begin_callback);
    set_callback(GLU_TESS_VERTEX_DATA, &tess_vertex_callback);
    set_callback(GLU_TESS_END_DATA, &tess_end_callback);
    set_callback(GLU_TESS_ERROR_DATA, &tess_error_callback);
    set_callback(GLU_TESS_COMBINE_DATA, &tess_combine_callback);
}

void glu_tess_raw::teardown_callbacks() {
    if (!_M_tess)
        return;
    LOGD(__PRETTY_FUNCTION__);
    set_callback(GLU_TESS_BEGIN_DATA, NULL);
    set_callback(GLU_TESS_VERTEX_DATA, NULL);
    set_callback(GLU_TESS_END_DATA, NULL);
    set_callback(GLU_TESS_ERROR_DATA, NULL);
    set_callback(GLU_TESS_COMBINE_DATA, NULL);
}

void glu_tess_raw::on_tess_begin(GLenum type) {
    LOGD("glu_tess_raw::on_tess_begin(type = " << nameOfGLType(type) << ")");
    switch (type) {
        case GL_TRIANGLE_STRIP:
            _M_collector.reset(new triangle_strip_collector(this));
            break;
        case GL_TRIANGLE_FAN:
            _M_collector.reset(new triangle_fan_collector(this));
            break;
        case GL_TRIANGLES:
            _M_collector.reset(new triangles_collector(this));
            break;
        case GL_LINE_LOOP:
            _M_collector.reset(new line_loop_collector(this));
            break;
        default:
            LOGW("unsupported " << nameOfGLType(type));
            break;
    }
}

void glu_tess_raw::on_tess_end() {
    LOGD(__PRETTY_FUNCTION__);
    _M_collector->end();
    _M_collector.reset();
}

void glu_tess_raw::on_tess_vertex(tess_point vertex) {
    LOGD("glu_tess_raw::on_tess_vertex"
            << "(vertex = " << vertex << arrays::view<GLdouble>(3, static_cast<GLdouble*>(vertex)) << ")");
    _M_collector->collect(tess_point(vertex));
}

void glu_tess_raw::on_tess_error(GLenum ecode) {
    LOGE("glu_tess: " << gluErrorString(ecode));
}

void glu_tess_raw::on_tess_combine(
        GLdouble coords[3],
        // 4 neighbours for interp
        void *vertex_data[4],
        GLfloat weight[4],
        void **out_data) {
    LOGD("glu_tess_raw::on_tess_combine"
            << "(coords = " << coords << "(" << arrays::view(3, coords) << ")"
            << ", vertex_data = " << vertex_data
            << ", weight = " << weight << "(" << arrays::view(4, weight) << ")"
            << ", out_data = " << out_data
            << ")");
}

void glu_tess_raw::begin_polyon() {
    gluTessBeginPolygon(_M_tess, this);
    LOGD("gluTessBeginPolygon(tess = " << _M_tess
            << ", polygon_data = " << this
            << ")");
}

void glu_tess_raw::end_polyon() {
    gluTessEndPolygon(_M_tess);
    LOGD("gluTessEndPolygon(tess = " << _M_tess << ")");
}

void glu_tess_raw::begin_contour() {
    gluTessBeginContour(_M_tess);
    LOGD("gluTessBeginContour(tess = " << _M_tess << ")");
}

void glu_tess_raw::end_contour() {
    gluTessEndContour(_M_tess);
    LOGD("gluTessEndContour(tess = " << _M_tess << ")");
}

void glu_tess_raw::vertex(GLdouble *coords, void *data) {
    // coords is contignous 3d coordinates for tessellation only
    // data can be more than coordinates such as color, normal, UV coordinates
    gluTessVertex(_M_tess, coords, data);
    LOGD("gluTessVertex(tess = " << _M_tess
            << ", coords = " << coords << "(" << arrays::view(3, coords) << ")"
            << ", data = " << data
            << ")");
}

void glu_tess_raw::set_winding_rule(winding_rule value) {
    set_property(GLU_TESS_WINDING_RULE, value);
}

void glu_tess_raw::set_boundary_only(bool value) {
    set_property(
            GLU_TESS_BOUNDARY_ONLY,
            value ? GL_TRUE : GL_FALSE);
}

void glu_tess_raw::set_tolerance(double value) {
    set_property(
            GLU_TESS_TOLERANCE,
            value);
}

void glu_tess_raw::set_normal(GLdouble x, GLdouble y, GLdouble z) {
    gluTessNormal(_M_tess, x, y, z);
    LOGD("gluTessNormal(tess = " << _M_tess
            << ", x = " << x
            << ", y = " << y
            << ", z = " << z);
}

glu_tess_raw::triangles_type const&
glu_tess_raw::get_triangles() const {
    return _M_triangles;
}

glu_tess_raw::lines_type const&
glu_tess_raw::get_lines() const {
    return _M_lines;
}

void glu_tess_raw::handle_triangle_collected(tess_triangle const &v) {
    LOGD("triangle = " << v);
    _M_triangles.push_back(v);
}

void glu_tess_raw::handle_line_collected(tess_line const &v) {
    LOGD("line = " << v);
    _M_lines.push_back(v);
}

std::ostream& operator<<(
        std::ostream &os,
        glu_tess_raw::tess_triangle const& obj) {
    obj.print(os);
    return os;
}

std::ostream& operator<<(
        std::ostream &os,
        glu_tess_raw::tess_line const& obj) {
    obj.print(os);
    return os;
}

} // namespace org
