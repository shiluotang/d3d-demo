#include "glu_tess.h"

namespace org {

template <typename T>
void glu_tess<T>::begin_polyon() {
    _M_raw.begin_polyon();
}

template <typename T>
void glu_tess<T>::end_polygon() {
    _M_raw.end_polyon();
}

template <typename T>
void glu_tess<T>::begin_contour() {
    _M_raw.begin_contour();
}

template <typename T>
void glu_tess<T>::end_contour() {
    _M_raw.end_contour();
}

template <typename T>
void glu_tess<T>::vertex(GLdouble *coords, T *data) {
    _M_raw.vertex(coords, data);
}

} // namespace org

// vim:set filetype=cpp
