//! \file

#ifndef JSON_H_YXDSNGFEZRBNYZ8E3BQZF
#define JSON_H_YXDSNGFEZRBNYZ8E3BQZF

#include <string>

namespace mb::ul {
template <typename JsonStrtype, typename JsonValue>
class IJSON {
public:
    virtual ~IJSON() = default;

    using JSONstr_type = JsonStrtype;
    using JSONval_type = JsonValue;

    //! Generate JSON string of object.
    virtual JSONstr_type get_json_str() const = 0;
    //! Generate JSON value of object.
    virtual JSONval_type get_json_value() const = 0;
    //! Load JSON string into this object.
    virtual void set_from_json_str(const JSONstr_type& str) = 0;
    //! Load Json::JsonValue into this object.
    virtual void set_json_value(const JSONval_type& root) = 0;
};

template <typename JsonValue>
using IJSON_stdstr = IJSON<std::string, JsonValue>;
} // namespace mb::ul

#endif
