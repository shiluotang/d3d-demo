#ifndef GL_TESS_H_INCLUDED
#define GL_TESS_H_INCLUDED

#include <memory>
#include <vector>
#include <deque>
#include <iosfwd>

#include <GL/gl.h>
#include <GL/glu.h>

namespace org {

class glu_tess {
    public:
        struct tess_point {
            explicit tess_point(GLdouble const *addr = NULL);
            GLdouble operator[](size_t n);
            void print(std::ostream&) const;

            GLdouble const *_M_addr;
        };

        struct tess_triangle {
            tess_triangle(
                    tess_point const &a,
                    tess_point const &b,
                    tess_point const &c);

            tess_point operator[](size_t n) const;
            void print(std::ostream&) const;

            tess_point _M_a;
            tess_point _M_b;
            tess_point _M_c;
        };

        typedef std::vector<tess_triangle> triangles_type;

        struct collector {
            explicit collector(glu_tess *tess);

            virtual void begin();

            virtual void end();

            virtual void collect(tess_point const &tp);

            glu_tess *_M_tess;
        };

        friend struct triangle_strip_collector;
        friend struct triangles_collector;

        struct triangle_strip_collector : collector {
            explicit triangle_strip_collector(glu_tess*);

            virtual void end();

            virtual void collect(tess_point const&);

            std::deque<tess_point> _M_triangle_points;
        };

        struct triangles_collector : collector {
            explicit triangles_collector(glu_tess*);

            virtual void end();

            virtual void collect(tess_point const &p);

            std::deque<tess_point> _M_triangle_points;
        };

        typedef void (CALLBACK *tess_callback)();

        glu_tess();

        virtual ~glu_tess();

        static
        void CALLBACK tess_begin_callback(GLenum type, void *data);

        static
        void CALLBACK tess_end_callback(void *data);

        static
        void CALLBACK tess_vertex_callback(void *vertex, void *data);

        static
        void CALLBACK tess_error_callback(GLenum ecode, void *data);

        template <typename T>
        void setup_callback(GLenum which, T cb);

        void setup_callbacks();

        void teardown_callbacks();

        void on_tess_begin(GLenum type);

        void on_tess_end();

        void on_tess_vertex(void *vertex);

        void on_tess_error(GLenum ecode);

        void begin();

        void end();

        void begin_contour();

        void end_contour();

        void vertex(GLdouble *coords, void *data = NULL);

        std::vector<tess_triangle> const& get_triangles() const;
    protected:
        void setup();
        void teardown();
    private:
        GLUtesselator *_M_tess;
        triangles_type _M_triangles;
        std::auto_ptr<collector> _M_collector;

        glu_tess(glu_tess const&);
        glu_tess& operator=(glu_tess const&);
};

std::ostream& operator<<(std::ostream&, glu_tess::tess_point const&);
std::ostream& operator<<(std::ostream&, glu_tess::tess_triangle const&);

void foo();

} // namespace org

#endif // GL_TESS_H_INCLUDED
