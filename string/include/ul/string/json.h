// 2014-2016

//! \file

#ifndef JSON_H_yxdsngfezrbnyz8e3bqzf
#define JSON_H_yxdsngfezrbnyz8e3bqzf

#include <string>


namespace mb::ul
{
template <typename JSON_strtype, typename JSON_value>
class IJSON
{
public:
    virtual ~IJSON() = default;

    typedef JSON_strtype JSONstr_type;
    typedef JSON_value JSONval_type;

    //! Generate JSON string of object.
    virtual JSONstr_type GetJsonStr() const = 0;
    //! Generate JSON value of object.
    virtual JSONval_type GetJsonValue() const = 0;
    //! Load JSON string into this object.
    virtual void SetFromJsonStr(const JSONstr_type& str) = 0;
    //! Load Json::JsonValue into this object.
    virtual void SetJsonValue(const JSONval_type& root) = 0;
};

template <typename JSON_value>
using IJSON_stdstr = IJSON<std::string, JSON_value>;
} // namespace mb::ul

#endif
