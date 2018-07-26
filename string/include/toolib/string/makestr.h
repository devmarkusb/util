// Markus Borris, 2011-2016
// This file is part of tfl library.

//!
/** From David Rodr�guez - dribeas, stackoverflow.
*/
//! \file


#ifndef MAKESTR_H_INCL_92nzr29h3f8
#define MAKESTR_H_INCL_92nzr29h3f8

#include <sstream>
#include <string>


namespace too
{
namespace str
{
//! Usage: f( (makestr() << "Some string" << sSome_string << iSomeInt << dSomeDouble << "...").c_str() );
class makestr
{
public:
    template <typename T>
    makestr& operator<<(const T& data)
    {
        m_buffer << data;
        return *this;
    }
    operator std::string() const { return m_buffer.str(); }

private:
    std::ostringstream m_buffer;
};
}
}

#endif
