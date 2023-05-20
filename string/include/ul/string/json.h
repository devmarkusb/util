//! \file

#ifndef JSON_H_YXDSNGFEZRBNYZ8E3BQZF
#define JSON_H_YXDSNGFEZRBNYZ8E3BQZF

#include <string>

namespace mb::ul {
template <typename JsonStrtype, typename JsonValue>
class IJSON {
public:
    virtual ~IJSON() = default;

    using JsonStrType = JsonStrtype;
    using JsonValType = JsonValue;

    //! Generate JSON string of object.
    virtual JsonStrType get_json_str() const = 0;
    //! Generate JSON value of object.
    virtual JsonValType get_json_value() const = 0;
    //! Load JSON string into this object.
    virtual void set_from_json_str(const JsonStrType& str) = 0;
    //! Load Json::JsonValue into this object.
    virtual void set_json_value(const JsonValType& root) = 0;
};

template <typename JsonValue>
using IJsonStdstr = IJSON<std::string, JsonValue>;
} // namespace mb::ul

#endif
