/** \file
    Generating compiler errors when deleting incomplete types (types of objects for which delete doesn't see the
    whole declaration and a non-trivial destructor exists).
    Implementation inspired by boost.
*/

#ifndef CHECKED_DELETE_H_F3287RN32R7NZ237
#define CHECKED_DELETE_H_F3287RN32R7NZ237

namespace mb::ul::mem {
template <class T>
inline void checked_delete(T* x) {
    static_assert(sizeof(T) > 0, "checked_delete requires a complete type");
    delete x; // NOLINT
}

template <class T>
inline void checked_array_delete(T* x) {
    static_assert(sizeof(T) > 0, "checked_array_delete requires a complete type");
    delete[] x; // NOLINT
}
} // namespace mb::ul::mem

#endif
