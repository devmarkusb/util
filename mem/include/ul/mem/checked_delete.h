/** \file
    Generating compiler errors when deleting incomplete types (types of objects for which delete doesn't see the
    whole declaration and a non-trivial destructor exists).
    Implementation inspired by boost.
*/

#ifndef CHECKED_DELETE_H_f3287rn32r7nz237
#define CHECKED_DELETE_H_f3287rn32r7nz237

namespace mb::ul::mem
{
template <class T>
inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void)sizeof(type_must_be_complete);
    delete x;
}

template <class T>
inline void checked_array_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void)sizeof(type_must_be_complete);
    delete[] x;
}
} // namespace mb::ul::mem

#endif
