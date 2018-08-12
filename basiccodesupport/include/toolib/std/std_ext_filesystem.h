// Markus Borris, 2017-18
// This file is part of tfl library.

//!
/** Cf. std_extensions.h
    */
//! \file

#ifndef STD_EXT_FILESYSTEM_H_wueihfiwufy3u48732t
#define STD_EXT_FILESYSTEM_H_wueihfiwufy3u48732t

#include "toolib/PPDEFS.h"
#include <fstream>

#if __has_include(<filesystem>)
// Note, no fallback to the experimental versions anymore.
#include <filesystem>
namespace too
{
#if TOO_COMP_MS_VISUAL_STUDIO_CPP && TOO_COMP_MS_VS_VER <= 1914
namespace std_fs = std::experimental::filesystem;
#else
namespace std_fs = std::filesystem;
#endif
} // too

#else

//####################################################################################################################
// A really quick&dirty implementation for the worst case not having the filesystem lib.

#if TOO_OS_UNIX
#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "toolib/assert.h"
#include "toolib/ignore_arg.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <system_error>

namespace too
{
namespace std_fs
{
/** Current deviations from standard:
    - The class can only handle paths with single distinct separators ("/", no repeating "//" ones).
        Except, operators /= and / also work with a single already trailing '/'.
    Notes:
    - The empty path is equivalent to a single dot '.', which represents the current dir, a relative path.
    - For a file starting with  dot the starting dot belongs to the filename.*/
struct path
{
    static const auto preferred_separator = '/';

    //! Constructs an empty path (which is semantically equivalent to current dir '.').
    path() noexcept {}
    //! Expects \param s to be empty or a syntactically valid path with OS conforming or portable separators. Implicit!
    path(std::string s) : s_{std::move(s)} {}
    path(const char* s) : s_{s} {}

    path& make_preferred() { return *this; }
    const char* c_str() const noexcept { return s_.c_str(); }
    std::string string() const
    {
        return s_;
    }

    std::string generic_string() const
    {
        return string();
    }

    bool empty() const noexcept
    {
        return s_.empty();
    }

    path extension() const
    {
        const size_t start = s_.rfind('.');
        if (start == 0 || start == std::string::npos || s_[start - 1] == '/')
            return {};
        return path{s_.substr(start, s_.size() - start)};
    }

    path stem() const
    {
        const size_t end = s_.rfind('.');
        size_t start = s_.rfind('/');

        if (start == std::string::npos)
            start = 0;
        else
            ++start;

        const size_t maxLength = s_.size() - start;
        size_t length = end == std::string::npos ? (s_.size() - start) : (end - start);
        if (!length)
            length = maxLength;

        return path{s_.substr(start, length)};
    }

    path filename() const
    {
        size_t start = s_.rfind('/');

        if (start == std::string::npos)
            start = 0;
        else
            ++start;

        return path{s_.substr(start, s_.size())};
    }

    path parent_path() const
    {
        size_t length = s_.rfind('/');

        if (length == std::string::npos)
            return {};
        else if (length == 0)
            ++length;

        return path{s_.substr(0, length)};
    }

    path& remove_trailing_separator() noexcept
    {
        if (!s_.empty() && s_.back() == '/')
            s_.pop_back();
        return *this;
    }

    //! \Param replacement replaces the current extension *including* the dot.
    path& replace_extension(const path& replacement = {})
    {
        const size_t start = s_.rfind('.');
        if (start == 0 || start == std::string::npos || s_[start - 1] == '/')
            s_ += replacement.string();
        else
            s_.replace(std::begin(s_) + start, std::end(s_), replacement.string());

        return *this;
    }

    //! 'concat'
    path& operator+=(const path& other)
    {
        s_ += other.s_;
        return *this;
    }

    path& operator+=(char c)
    {
        s_ += c;
        return *this;
    }

    //! 'append'
    path& operator/=(const path& other)
    {
        if (!empty() && s_.back() != '/')
            s_ += '/';
        s_ += other.string();
        return *this;
    }

    void clear() noexcept
    {
        s_.clear();
    }

private:
    std::string s_;

    path normalize() const
    {
        path ret{*this};
        ret.normalize();
        return ret;
    }

    void normalize()
    {
        if (s_ == ".")
            clear();
    }

    friend inline bool operator==(const path& lhs, const path& rhs);
};

inline bool operator==(const path& lhs, const path& rhs)
{
    return lhs.string() == rhs.string();
}

inline bool operator!=(const path& lhs, const path& rhs)
{
    return !(lhs == rhs);
}

inline path operator+(const path& lhs, const path& rhs)
{
    path ret{lhs};
    ret += rhs;
    return ret;
}

inline path operator/(const path& lhs, const path& rhs)
{
    path ret{lhs};
    ret /= rhs;
    return ret;
}

inline std::ostream& operator<<(std::ostream& os, const path& p)
{
    os << p.string();
    return os;
}

struct directory_entry
{
    explicit directory_entry(path p) : p_{std::move(p)} {}

    void assign(const path& p) { p_ = p; }
    //! Implicit conversion.
    operator const path& () const noexcept { return p_; }

private:
    std_fs::path p_;

    friend inline bool operator==(const directory_entry& lhs, const directory_entry& rhs);
};

inline bool operator==(const directory_entry& lhs, const directory_entry& rhs)
{
    return lhs.p_ == rhs.p_;
}

inline bool operator!=(const directory_entry& lhs, const directory_entry& rhs)
{
    return !(lhs == rhs);
}

#if TOO_OS_LINUX
struct directory_iterator : public std::iterator<std::input_iterator_tag, directory_entry>
{
    //! Constructs an iterator pointing behind the last element.
    directory_iterator() = default;
    explicit directory_iterator(path p) : p_{std::move(p)}
    {
        init_dir();
        next_entry();
    }

    ~directory_iterator() = default;

    const directory_entry& operator*() const
    {
        return entry_;
    }

    const directory_entry* operator->() const
    {
        return &entry_;
    }

    directory_iterator& operator++()
    {
        next_entry();

        return *this;
    }

private:
    struct DIR_scoped
    {
        explicit DIR_scoped(path p) : p_{std::move(p)}
        {
            d_ = opendir(p_.c_str());
            if (!d_)
            {
                std::stringstream ss;
                ss << "could not open dir, error code: " << errno << ", dir: " << p_.string();
                throw std::runtime_error{ss.str()};
            }
            TOO_ENSURE(d_);
        }

        ~DIR_scoped()
        {
            if (d_ && closedir(d_))
            {
                // Ignoring the error in release. 1) Don't want to make the destructor noexcept(false) as the class
                // is designed to be used with the std heavily (which is designed for non-throwing destructors, meaning
                // terminating programs, which is way too much here). 2) A lib shouldn't write to cerr.
#ifndef NDEBUG
                std::stringstream ss;
                ss << "could not close dir, error code: " << errno << ", dir: " << p_.string();
                std::string errmsg{ss.str()};
                too::ignore_arg(errmsg);
#endif
                TOO_ASSERT(false);
            }
        }

        DIR* get() const noexcept
        {
            return d_;
        }

        void clear() noexcept
        {
            p_.clear();
            d_ = {};
        };

    private:
        path p_;
        DIR* d_{};
    };

    mutable path p_;
    mutable std::shared_ptr<DIR_scoped> d_;
    mutable struct dirent* entry_impl_{};
    mutable directory_entry entry_{{}};

    void init_dir() const
    {
        TOO_EXPECT(!d_);
        d_ = std::make_shared<DIR_scoped>(p_);
        TOO_ENSURE(d_);
    }

    void next_entry() const
    {
        const auto errno_prev = errno;
        do
        {
            entry_impl_ = readdir(d_->get());
        }
        while (entry_impl_ && (
                (entry_impl_->d_name[0] == '.' && entry_impl_->d_name[1] == 0) ||
                (entry_impl_->d_name[0] == '.' && entry_impl_->d_name[1] == '.' && entry_impl_->d_name[2] == 0)));

        if (entry_impl_)
            entry_.assign(p_ / path{entry_impl_->d_name});
        else
        {
            if (errno != errno_prev)
            {
                std::stringstream ss;
                ss << "could not read dir, error code: " << errno << ", parent dir: " << p_.string();
                throw std::runtime_error{ss.str()};
            }
            p_.clear();
            d_->clear();
            entry_impl_ = {};
            entry_.assign({});
        }
    }

    friend bool operator==(const directory_iterator& lhs, const directory_iterator& rhs);
};

inline bool operator==(const directory_iterator& lhs, const directory_iterator& rhs)
{
    return lhs.entry_ == rhs.entry_;
}

inline bool operator!=(const directory_iterator& lhs, const directory_iterator& rhs)
{
    return !(lhs == rhs);
}

#else
#error "not implemented"
#endif

inline bool exists(const path& p)
{
#if TOO_OS_LINUX
    struct stat dummy;
    memset(&dummy, 0, sizeof(struct stat));
    return (stat(p.c_str(), &dummy) == 0);
#else
#error "not implemented"
#endif
}

//! \Returns false if already existing or error occurred.
inline bool create_directories(const path& p)
{
#if TOO_OS_LINUX
    const auto dir = p.c_str();
    char tmp[256]{};
    size_t len{};

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (char* p_ = tmp + 1; *p_; p_++)
    {
        if (*p_ == '/')
        {
            *p_ = 0;
            mkdir(tmp, S_IRWXU);
            *p_ = '/';
        }
    }
    return mkdir(tmp, S_IRWXU) == 0;
#else
#error "not implemented"
#endif
}

inline bool is_regular_file(const path& p)
{
#if TOO_OS_LINUX
    struct stat path_stat;
    memset(&path_stat, 0, sizeof(struct stat));
    stat(p.c_str(), &path_stat);
    return S_ISREG(path_stat.st_mode);
#else
#error "not implemented"
#endif
}

inline bool remove(const path& p)
{
#if TOO_OS_LINUX
    return ::remove(p.c_str()) == 0;
#else
#error "not implemented"
#endif
}

inline path current_path(std::error_code& ec)
{
#if TOO_OS_LINUX
    char cwd[PATH_MAX]{};
    if (getcwd(cwd, sizeof(cwd)))
    {
        return path{std::string{cwd}};
    }
    else
    {
        ec.assign(errno, std::system_category());
        return {};
    }
#else
#error "not implemented"
#endif
}

inline void current_path(const path& p, std::error_code& ec) noexcept
{
#if TOO_OS_LINUX
    if (chdir(p.c_str()))
        ec.assign(errno, std::system_category());
#else
#error "not implemented"
#endif
}
} // std_fs
} // too

#endif

namespace too
{
namespace file
{
inline void touch(const too::std_fs::path& p)
{
    std::ofstream f{p.string()};
}
} // file
} // too

#endif
