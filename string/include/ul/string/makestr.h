// 2011-2016

//!
/** From David Rodr?guez - dribeas, stackoverflow.
 */
//! \file


#ifndef MAKESTR_H_92nzr29h3f8
#define MAKESTR_H_92nzr29h3f8

#include <sstream>
#include <string>


namespace mb::ul
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
    operator std::string() const
    {
        return m_buffer.str();
    }

private:
    std::ostringstream m_buffer;
};
} // namespace str
} // namespace mb::ul

#endif
