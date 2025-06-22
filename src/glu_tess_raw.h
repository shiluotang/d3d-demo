#ifndef GL_TESS_RAW_H_INCLUDED
#define GL_TESS_RAW_H_INCLUDED

#include <memory>
#include <vector>
#include <deque>
#include <iosfwd>

#include <GL/gl.h>
#include <GL/glu.h>

namespace org {

class glu_tess_raw {
    public:
        enum winding_rule {
            ODD = GLU_TESS_WINDING_ODD,
            NONZERO = GLU_TESS_WINDING_NONZERO ,
            POSITIVE = GLU_TESS_WINDING_POSITIVE,
            NEGATIVE = GLU_TESS_WINDING_NEGATIVE,
            ABS_GEQ_TWO = GLU_TESS_WINDING_ABS_GEQ_TWO,
        };

        typedef void* tess_point;

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

        struct tess_line {
            tess_line(
                    tess_point const &a,
                    tess_point const &b);

            tess_point operator[](size_t n) const;
            void print(std::ostream&) const;

            tess_point _M_a;
            tess_point _M_b;
        };

        typedef std::vector<tess_triangle> triangles_type;
        typedef std::vector<tess_line> lines_type;

        struct collector {
            explicit collector(glu_tess_raw *tess);

            virtual void begin();

            virtual void end();

            virtual void collect(tess_point const &tp);

            // just a pointer for nullable reference
            glu_tess_raw *_M_tess;
        };

        struct triangle_fan_collector : collector {
            explicit triangle_fan_collector(glu_tess_raw*);

            virtual void collect(tess_point const&);

            std::deque<tess_point> _M_triangle_points;
        };

        struct triangle_strip_collector : collector {
            explicit triangle_strip_collector(glu_tess_raw*);

            virtual void end();

            virtual void collect(tess_point const&);

            std::deque<tess_point> _M_triangle_points;
        };

        struct triangles_collector : collector {
            explicit triangles_collector(glu_tess_raw*);

            virtual void end();

            virtual void collect(tess_point const &p);

            std::deque<tess_point> _M_triangle_points;
        };

        struct line_loop_collector : collector {
            explicit line_loop_collector(glu_tess_raw*);

            virtual void begin();

            virtual void end();

            virtual void collect(tess_point const &p);

            bool _M_starting;
            tess_point _M_p0;
            std::deque<tess_point> _M_line_points;
        };

        typedef void (CALLBACK *tess_callback)();

        glu_tess_raw();

        virtual ~glu_tess_raw();

        void begin_polyon();

        void end_polyon();

        // contour automatically connect the last vertex to the first one
        void begin_contour();

        // contour automatically connect the last vertex to the first one
        void end_contour();

        /**
         * feed vertex coordinates and callback data to glu tesselator.
         *
         * The second parameter is the data received in callback.
         *
         * @param[in] coords is contignous 3d coordinates for tessellation only
         * @param[in] data can be more than coordinates such as color, normal,
         * UV coordinates.
         */
        void vertex(GLdouble *coords, void *data);

        void set_winding_rule(winding_rule value);

        void set_boundary_only(bool value);

        void set_tolerance(double value);

        void set_normal(GLdouble x, GLdouble y, GLdouble z);

        // FIXME may contains allocated data from combine callback, it's better
        // to deallocate them to prevent memory leak.
        triangles_type const& get_triangles() const;

        lines_type const& get_lines() const;

        virtual
        void handle_triangle_collected(tess_triangle const&);

        virtual
        void handle_line_collected(tess_line const&);
    protected:
        static
        void CALLBACK tess_begin_callback(GLenum type, void *data);

        static
        void CALLBACK tess_end_callback(void *data);

        static
        void CALLBACK tess_vertex_callback(void *vertex, void *data);

        static
        void CALLBACK tess_error_callback(GLenum ecode, void *data);

        static
        void CALLBACK tess_combine_callback(
                GLdouble coords[3],
                void *vertex_data[4],
                GLfloat weight[4],
                void **outData,
                void *polygon_data);

        void set_callback0(GLenum which, tess_callback);

        void set_property(GLenum which, GLdouble value);

        template <typename T>
        void set_callback(GLenum which, T cb) {
            set_callback0(which, reinterpret_cast<tess_callback>(cb));
        }

        void setup_callbacks();

        void teardown_callbacks();

        void on_tess_begin(GLenum type);

        void on_tess_end();

        void on_tess_vertex(tess_point vertex);

        void on_tess_error(GLenum ecode);

        virtual
        void on_tess_combine(
                GLdouble coords[3],
                void *vertex_data[4],
                GLfloat weight[4],
                void **outData);
    private:
        GLUtesselator *_M_tess;
        triangles_type _M_triangles;
        lines_type _M_lines;
        std::auto_ptr<collector> _M_collector;

        glu_tess_raw(glu_tess_raw const&);
        glu_tess_raw& operator=(glu_tess_raw const&);
};

std::ostream& operator<<(std::ostream&, glu_tess_raw::tess_triangle const&);
std::ostream& operator<<(std::ostream&, glu_tess_raw::tess_line const&);

} // namespace org

#endif // GL_TESS_RAW_H_INCLUDED
