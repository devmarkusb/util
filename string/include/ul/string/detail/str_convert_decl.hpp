//! \file

#ifndef STR_CONVERT_DECL_HPP_ISNGFYUFG875456
#define STR_CONVERT_DECL_HPP_ISNGFYUFG875456

namespace mb::ul::str
{
    struct ConversionErrorToQuestionMark {
        static char on_conversion_error(unsigned int) {
            return '?';
        }
    };
}

#endif
