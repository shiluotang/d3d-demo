#ifndef GL_TESS_H_INCLUDED
#define GL_TESS_H_INCLUDED

// #include <GL/gl.h>
#include <GL/glu.h>

#include "glu_tess_raw.h"

namespace org {

template <typename T>
class glu_tess {
    public:
        typedef T *pointer;
        void begin_polyon();
        void end_polygon();
        void begin_contour();
        void end_contour();
        void vertex(GLdouble*, T*);
    protected:
    private:
        glu_tess_raw _M_raw;
};

} // namespace org

#endif // GL_TESS_H_INCLUDED
