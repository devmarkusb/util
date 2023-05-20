//! \file

#ifndef FILE_H_SDUIFHG3GFY324N178FSFFE4F
#define FILE_H_SDUIFHG3GFY324N178FSFFE4F

#include "assert.h"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

namespace mb::ul::file {
/** \param retErrDetail returns error detail string or is empty on success.
    \return false on success.*/
template <class FStream>
bool fstream_failed(std::string& retErrDetail, const FStream& fs) {
    retErrDetail.clear();
    if (fs)
        return false;
    if (fs.eof())
        retErrDetail = "eof";
    else if (fs.bad())
        retErrDetail = "bad";
    else if (fs.fail())
        retErrDetail = "fail";
    return true;
}

enum class Operation {
    save,
    load,
};

[[noreturn]] inline void throw_error(const std::string& filePathNameExt, Operation op, const std::string& retErrDetail) {
    const auto errno_copy = errno;
    std::stringstream ss;
    ss << filePathNameExt;
    ss << " could not be ";
    switch (op) {
        case Operation::save:
            ss << "saved";
            break;
        case Operation::load:
            ss << "loaded";
            break;
    }
    ss << ", details: " << retErrDetail;
    //todo use strerror_s and what platforms demand, remove nolint
    ss << ", ec: " << errno_copy << ", " << std::strerror(errno_copy); // NOLINT
    throw std::runtime_error{ss.str()};
}
} // namespace mb::ul::file

#endif
