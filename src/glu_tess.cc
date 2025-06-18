#include <cassert>
#include <stdexcept>
#include <memory>
#include <vector>
#include <deque>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <GL/glext.h>

#include "logx.h"

#include "enum_decl.h"

#include "glu_tess.h"

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

template <typename E>
array_view<E> view(size_t n, E *p) {
    return array_view<E>(p, n);
}

template <typename T>
std::ostream& operator<<(std::ostream &os, array_view<T> const &view) {
    view.print(os);
    return os;
}

} // namespace anonymous

namespace org {

glu_tess::tess_triangle::tess_triangle(
        tess_point const &a,
        tess_point const &b,
        tess_point const &c)
    : _M_a(a)
    , _M_b(b)
    , _M_c(c)
{
}

glu_tess::tess_point
glu_tess::tess_triangle::operator[](size_t n) const {
    switch (n) {
        case 0: return _M_a;
        case 1: return _M_b;
        case 2: return _M_c;
        default: break;
    }
    throw std::out_of_range("n");
}

void glu_tess::tess_triangle::print(std::ostream &out) const {
    out << "[" << _M_a << ", " << _M_b << ", " << _M_c << "]";
}

glu_tess::collector::collector(glu_tess *tess)
    : _M_tess(tess)
{
}

void glu_tess::collector::begin() {
}

void glu_tess::collector::end() {
}

void glu_tess::collector::collect(tess_point const &tp) {
}

glu_tess::triangle_fan_collector::triangle_fan_collector(glu_tess *tess)
    : collector(tess)
{
}

void glu_tess::triangle_fan_collector::collect(tess_point const &p) {
    if (_M_triangle_points.size() < 3)
        _M_triangle_points.push_back(p);
    if (_M_triangle_points.size() == 3) {
        tess_triangle t(
                _M_triangle_points[0],
                _M_triangle_points[1],
                _M_triangle_points[2]);
        LOGD("triangle = " << t);
        _M_tess->_M_triangles.push_back(t);
        tess_point tp = _M_triangle_points.back();
        _M_triangle_points.pop_back();
        _M_triangle_points.pop_back();
        _M_triangle_points.push_back(tp);
    }
}

glu_tess::triangle_strip_collector::triangle_strip_collector(
        glu_tess *tess) :collector(tess)
{
}

void glu_tess::triangle_strip_collector::end() {
    assert(_M_triangle_points.size() < 3);
}

void glu_tess::triangle_strip_collector::collect(tess_point const &p) {
    if (_M_triangle_points.size() < 3)
        _M_triangle_points.push_back(p);
    if (_M_triangle_points.size() == 3) {
        tess_triangle t(
                _M_triangle_points[0],
                _M_triangle_points[1],
                _M_triangle_points[2]);
        LOGD("triangle = " << t);
        _M_tess->_M_triangles.push_back(t);
        _M_triangle_points.pop_front();
    }
}

glu_tess::triangles_collector::triangles_collector(glu_tess *tess)
    : collector(tess) {
}

void glu_tess::triangles_collector::end() {
    assert(_M_triangle_points.size() == 0);
}

void glu_tess::triangles_collector::collect(tess_point const &p) {
    if (_M_triangle_points.size() < 3)
        _M_triangle_points.push_back(p);
    if (_M_triangle_points.size() == 3) {
        tess_triangle t(
                _M_triangle_points[0],
                _M_triangle_points[1],
                _M_triangle_points[2]);
        LOGD("triangle = " << t);
        _M_tess->_M_triangles.push_back(t);
        _M_triangle_points.clear();
    }
}

glu_tess::glu_tess()
    :_M_tess(NULL)
{
    _M_tess = gluNewTess();
    LOGD("gluNewTess() = " << _M_tess);
    if (!_M_tess)
        throw std::runtime_error("gluNewTess");
    this->setup_callbacks();
}

glu_tess::~glu_tess() {
    teardown_callbacks();
    if (_M_tess) {
        gluDeleteTess(_M_tess);
        LOGD("gluDeleteTess(" << _M_tess << ")");
    }
    _M_tess = NULL;
}

void CALLBACK glu_tess::tess_begin_callback(GLenum type, void *data) {
    glu_tess *t = reinterpret_cast<glu_tess*>(data);
    t->on_tess_begin(type);
}

void CALLBACK glu_tess::tess_end_callback(void *data) {
    glu_tess *t = reinterpret_cast<glu_tess*>(data);
    t->on_tess_end();
}

void CALLBACK glu_tess::tess_vertex_callback(void *vertex, void *data) {
    glu_tess *t = reinterpret_cast<glu_tess*>(data);
    t->on_tess_vertex(vertex);
}

void CALLBACK glu_tess::tess_error_callback(GLenum ecode, void *data) {
    glu_tess *t = reinterpret_cast<glu_tess*>(data);
    t->on_tess_error(ecode);
}

void CALLBACK glu_tess::tess_combine_callback(
        GLdouble coords[3],
        void *vertex_data[4],
        GLfloat weight[4],
        void **outData,
        void *polygon_data) {
    glu_tess *t = reinterpret_cast<glu_tess*>(polygon_data);
    t->on_tess_combine(coords, vertex_data, weight, outData);
}

void glu_tess::set_callback0(GLenum which, tess_callback cb) {
    gluTessCallback(_M_tess, which, cb);
    LOGD("gluTessCallback(tess = " << _M_tess
            << ", which = " << nameOfTess(which)
            << ", fn = " << reinterpret_cast<void*>(cb)
            << ")");
}

void glu_tess::setup_callbacks() {
    if (!_M_tess)
        return;
    LOGD(__PRETTY_FUNCTION__);
    set_callback(GLU_TESS_BEGIN_DATA, &tess_begin_callback);
    set_callback(GLU_TESS_VERTEX_DATA, &tess_vertex_callback);
    set_callback(GLU_TESS_END_DATA, &tess_end_callback);
    set_callback(GLU_TESS_ERROR_DATA, &tess_error_callback);
    set_callback(GLU_TESS_COMBINE_DATA, &tess_combine_callback);
}

void glu_tess::teardown_callbacks() {
    if (!_M_tess)
        return;
    LOGD(__PRETTY_FUNCTION__);
    set_callback(GLU_TESS_BEGIN_DATA, NULL);
    set_callback(GLU_TESS_VERTEX_DATA, NULL);
    set_callback(GLU_TESS_END_DATA, NULL);
    set_callback(GLU_TESS_ERROR_DATA, NULL);
}

void glu_tess::on_tess_begin(GLenum type) {
    LOGD("glu_tess::on_tess_begin(type = " << nameOfGLType(type) << ")");
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
        default:
            LOGW("unsupported " << nameOfGLType(type));
            break;
    }
}

void glu_tess::on_tess_end() {
    LOGD(__PRETTY_FUNCTION__);
    _M_collector->end();
    _M_collector.reset();
}

void glu_tess::on_tess_vertex(tess_point vertex) {
    LOGD("glu_tess::on_tess_vertex"
            << "(vertex = " << vertex << ")");
    _M_collector->collect(tess_point(vertex));
}

void glu_tess::on_tess_error(GLenum ecode) {
    LOGE("glu_tess: " << gluErrorString(ecode));
}

void glu_tess::on_tess_combine(
        GLdouble coords[3],
        void *vertex_data[4],
        GLfloat weight[4],
        void **out_data) {
    LOGD("glu_tess::on_tess_combine"
            << "(coords = " << coords << "(" << view(3, coords) << ")"
            << ", vertex_data = " << vertex_data
            << ", weight = " << weight << "(" << view(4, weight) << ")"
            << ", out_data = " << out_data
            << ")");
    if (!_M_combiner.get())
        return;
    void *r = _M_combiner->spawn_vertex_data();
    _M_combiner->interp(
            const_cast<void const**>(vertex_data),
            weight,
            r);
    *out_data = r;
}

void glu_tess::begin_polyon() {
    gluTessBeginPolygon(_M_tess, this);
    LOGD("gluTessBeginPolygon(tess = " << _M_tess
            << ", polygon_data = " << this
            << ")");
}

void glu_tess::end_polyon() {
    gluTessEndPolygon(_M_tess);
    LOGD("gluTessEndPolygon(tess = " << _M_tess << ")");
}

void glu_tess::begin_contour() {
    gluTessBeginContour(_M_tess);
    LOGD("gluTessBeginContour(tess = " << _M_tess << ")");
}

void glu_tess::end_contour() {
    gluTessEndContour(_M_tess);
    LOGD("gluTessEndContour(tess = " << _M_tess << ")");
}

void glu_tess::vertex(GLdouble *coords, void *data) {
    // coords is contignous 3d coordinates for tessellation only
    // data can be more than coordinates such as color, normal, UV coordinates
    gluTessVertex(_M_tess, coords, data);
    LOGD("gluTessVertex(tess = " << _M_tess
            << ", coords = " << coords << "(" << view(3, coords) << ")"
            << ", data = " << data
            << ")");
}

glu_tess::triangles_type const&
glu_tess::get_triangles() const {
    return _M_triangles;
}

std::ostream& operator<<(
        std::ostream &os,
        glu_tess::tess_triangle const& obj) {
    obj.print(os);
    return os;
}

} // namespace org
