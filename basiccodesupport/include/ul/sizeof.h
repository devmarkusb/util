//! \file

#ifndef SIZEOF_H_gtzh24789xh7893xt78934789t4x37894t
#define SIZEOF_H_gtzh24789xh7893xt78934789t4x37894t

/** Generates a compile error telling you sizeof Type.
    Cannot be used e.g. within functions.
    \param Type the type you want to know sizeof(Type) for.
    \see UL_CT_SIZEOF_OPT2 as alternative to be used e.g. within functions.
*/
#define UL_CT_SIZEOF(Type) \
    template <size_t> \
    class sizeof_##Type##_is_; \
    sizeof_##Type##_is_<sizeof(Type)> you_must_put_this_where_template_declarations_are_allowed;

/** Like UL_CT_SIZEOF, but usable e.g. within functions.
    \see UL_CT_SIZEOF.
*/
#define UL_CT_SIZEOF_OPT2(Type) \
    switch (0) \
    { \
        case sizeof(Type): \
        case sizeof(Type): \
    }

#endif
