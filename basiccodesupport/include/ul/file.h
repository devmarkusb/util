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
/** \param ret_err_detail returns error detail string or is empty on success.
    \return false on success.*/
template <class FStream>
bool fstream_failed(std::string& ret_err_detail, const FStream& fs) {
    ret_err_detail.clear();
    if (fs)
        return false;
    if (fs.eof())
        ret_err_detail = "eof";
    else if (fs.bad())
        ret_err_detail = "bad";
    else if (fs.fail())
        ret_err_detail = "fail";
    return true;
}

enum class Operation {
    save,
    load,
};

[[noreturn]] inline void throw_error(
    const std::string& file_path_name_ext, Operation op, const std::string& ret_err_detail) {
    const auto errno_copy = errno;
    std::stringstream ss;
    ss << file_path_name_ext;
    ss << " could not be ";
    switch (op) {
        case Operation::save:
            ss << "saved";
            break;
        case Operation::load:
            ss << "loaded";
            break;
    }
    ss << ", details: " << ret_err_detail;
    //todo use strerror_s and what platforms demand, remove nolint
    ss << ", ec: " << errno_copy << ", " << std::strerror(errno_copy); // NOLINT
    throw std::runtime_error{ss.str()};
}
} // namespace mb::ul::file

#endif
