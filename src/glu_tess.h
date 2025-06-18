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
        struct tess_coord {
            GLdouble _M_data[3];
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

        typedef std::vector<tess_triangle> triangles_type;

        struct combiner {
            virtual void* spawn_vertex_data() = 0;
            virtual void* interp(
                    void const *vertex_data[4],
                    GLfloat const weights[4],
                    void *r_vertex_data) = 0;
        };

        struct collector {
            explicit collector(glu_tess *tess);

            virtual void begin();

            virtual void end();

            virtual void collect(tess_point const &tp);

            // just a pointer for nullable reference
            glu_tess *_M_tess;
        };

        friend struct triangle_fan_collector;
        friend struct triangle_strip_collector;
        friend struct triangles_collector;

        struct triangle_fan_collector : collector {
            explicit triangle_fan_collector(glu_tess*);

            virtual void collect(tess_point const&);

            std::deque<tess_point> _M_triangle_points;
        };

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
        void vertex(GLdouble *coords, void *data = NULL);

        triangles_type const& get_triangles() const;
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

        void on_tess_combine(
                GLdouble coords[3],
                void *vertex_data[4],
                GLfloat weight[4],
                void **outData);
    private:
        GLUtesselator *_M_tess;
        triangles_type _M_triangles;
        std::auto_ptr<collector> _M_collector;
        std::auto_ptr<combiner> _M_combiner;
        std::vector<tess_coord> _M_combined_coords;

        glu_tess(glu_tess const&);
        glu_tess& operator=(glu_tess const&);
};

std::ostream& operator<<(std::ostream&, glu_tess::tess_triangle const&);

} // namespace org

#endif // GL_TESS_H_INCLUDED
