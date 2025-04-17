#include "ul/string/json.h" // NOLINT
#include "gtest/gtest.h"
#include <string>

namespace ul = mb::ul;

// Mock implementation of IJSON for testing
class MockJson : public ul::IJsonStdstr<std::string> {
public:
    std::string json_str_;
    std::string json_value_;

    std::string get_json_str() const override {
        return json_str_;
    }

    std::string get_json_value() const override {
        return json_value_;
    }

    void set_from_json_str(const std::string& str) override {
        json_str_ = str;
    }

    void set_json_value(const std::string& root) override {
        json_value_ = root;
    }
};

TEST(json, basic_interface) {
    MockJson mock;
    
    // Test get_json_str
    mock.json_str_ = "{\"key\": \"value\"}";
    EXPECT_EQ("{\"key\": \"value\"}", mock.get_json_str());
    
    // Test get_json_value
    mock.json_value_ = "{\"key\": \"value\"}";
    EXPECT_EQ("{\"key\": \"value\"}", mock.get_json_value());
    
    // Test set_from_json_str
    mock.set_from_json_str("{\"new\": \"data\"}");
    EXPECT_EQ("{\"new\": \"data\"}", mock.json_str_);
    
    // Test set_json_value
    mock.set_json_value("{\"new\": \"value\"}");
    EXPECT_EQ("{\"new\": \"value\"}", mock.json_value_);
}

TEST(json, empty_values) {
    MockJson mock;
    
    // Test with empty strings
    mock.set_from_json_str("");
    EXPECT_EQ("", mock.json_str_);
    
    mock.set_json_value("");
    EXPECT_EQ("", mock.json_value_);
    
    mock.json_str_ = "";
    EXPECT_EQ("", mock.get_json_str());
    
    mock.json_value_ = "";
    EXPECT_EQ("", mock.get_json_value());
}

TEST(json, complex_values) {
    MockJson mock;
    
    // Test with complex JSON strings
    const std::string complex_json = "{\"array\": [1, 2, 3], \"object\": {\"nested\": \"value\"}, \"boolean\": true}";
    mock.set_from_json_str(complex_json);
    EXPECT_EQ(complex_json, mock.json_str_);
    
    mock.set_json_value(complex_json);
    EXPECT_EQ(complex_json, mock.json_value_);
}
