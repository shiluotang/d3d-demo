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

} // namespace anonymous

namespace org {

glu_tess::tess_point::tess_point(GLdouble const *addr)
    : _M_addr(addr)
{
}

GLdouble glu_tess::tess_point::operator[](size_t n) {
    return _M_addr[n];
}

void glu_tess::tess_point::print(std::ostream &out) const {
    out << "[" << _M_addr[0] << ", " << _M_addr[1] << ", " << _M_addr[2] << "]";
}

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
        LOGD(__PRETTY_FUNCTION__ << " triangle = " << t);
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
        LOGD(__PRETTY_FUNCTION__ << " triangle = " << t);
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
        LOGD(__PRETTY_FUNCTION__ << " triangle = " << t);
        _M_tess->_M_triangles.push_back(t);
        _M_triangle_points.clear();
    }
}

glu_tess::glu_tess()
    :_M_tess(NULL)
{
    _M_tess = gluNewTess();
    if (!_M_tess)
        throw std::runtime_error("gluNewTess");
    this->setup();
}

glu_tess::~glu_tess() {
    teardown_callbacks();
    if (_M_tess) {
        gluDeleteTess(_M_tess);
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

void glu_tess::set_callback0(GLenum which, tess_callback cb) {
    gluTessCallback(_M_tess, which, cb);
    gluTessCallback(_M_tess, which, reinterpret_cast<tess_callback>(cb));
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

void glu_tess::on_tess_vertex(void *vertex) {
    GLdouble *p = static_cast<GLdouble*>(vertex);
    tess_point tp(p);
    LOGD(__PRETTY_FUNCTION__);
    LOGD("x = " << tp[0] << ", y = " << tp[1] << ", z = " << tp[2]);
    _M_collector->collect(tess_point(p));
}

void glu_tess::on_tess_error(GLenum ecode) {
    LOGE("glu_tess: " << gluErrorString(ecode));
}

void glu_tess::begin() {
    gluTessBeginPolygon(_M_tess, this);
    LOGD("gluTessBeginPolygon(tess = " << _M_tess
            << ", polygon_data = " << this
            << ")");
}

void glu_tess::end() {
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
            << ", coords = " << coords
            << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")"
            << ", data = " << data
            << ")");
}

glu_tess::triangles_type const&
glu_tess::get_triangles() const {
    return _M_triangles;
}

void glu_tess::setup() {
    setup_callbacks();
}

void glu_tess::teardown() {
    teardown_callbacks();
}

std::ostream& operator<<(
        std::ostream &os,
        glu_tess::tess_point const& obj) {
    obj.print(os);
    return os;
}

std::ostream& operator<<(
        std::ostream &os,
        glu_tess::tess_triangle const& obj) {
    obj.print(os);
    return os;
}

void foo() {
    GLdouble quad2[8][3] = {
        // CW (clock-wise)
        {-2,3,0},
        {-2,0,0},
        {2,0,0},
        {2,3,0},

        // CCW (counter clock-wise)
        {-1,2,0},
        {-1,1,0},
        {1,1,0},
        {1,2,0}
    };

    glu_tess tess;
    tess.begin();
    tess.begin_contour();
    tess.vertex(quad2[0], quad2[0]);
    tess.vertex(quad2[1], quad2[1]);
    tess.vertex(quad2[2], quad2[2]);
    tess.vertex(quad2[3], quad2[3]);
    tess.end_contour();
    tess.begin_contour();
    tess.vertex(quad2[4], quad2[4]);
    tess.vertex(quad2[5], quad2[5]);
    tess.vertex(quad2[6], quad2[6]);
    tess.vertex(quad2[7], quad2[7]);
    tess.end_contour();
    tess.end();
    glu_tess::triangles_type const &triangles = tess.get_triangles();
    typedef glu_tess::triangles_type::const_iterator const_iterator;
    for (const_iterator it = triangles.begin(), e = triangles.end();
            it != e; ++it) {
        LOGD("tess triangle = " << *it);
    }
}

} // namespace org
