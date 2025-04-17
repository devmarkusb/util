#include "ul/file.h" // NOLINT
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <string_view>

TEST(FileTest, fstream_failed) {
    std::string err_detail;
    
    // Test with valid stream
    std::stringstream valid_stream;
    valid_stream << "test data";
    EXPECT_FALSE(ul::file::fstream_failed(err_detail, valid_stream));
    EXPECT_TRUE(err_detail.empty());
    
    // Test with EOF stream
    std::stringstream eof_stream;
    eof_stream.setstate(std::ios::eofbit);
    EXPECT_FALSE(ul::file::fstream_failed(err_detail, eof_stream));
    EXPECT_EQ(err_detail, "eof");
    
    // Test with bad stream
    std::stringstream bad_stream;
    bad_stream.setstate(std::ios::badbit);
    EXPECT_TRUE(ul::file::fstream_failed(err_detail, bad_stream));
    EXPECT_EQ(err_detail, "bad");
    
    // Test with failed stream
    std::stringstream fail_stream;
    fail_stream.setstate(std::ios::failbit);
    EXPECT_TRUE(ul::file::fstream_failed(err_detail, fail_stream));
    EXPECT_EQ(err_detail, "fail");
}

TEST(FileTest, throw_error) {
    // Test save operation error
    try {
        ul::file::throw_error("test.txt", ul::file::Operation::save, "test error");
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& e) {
        std::string_view error_msg{e.what()};
        EXPECT_TRUE(error_msg.find("test.txt") != std::string::npos);
        EXPECT_TRUE(error_msg.find("saved") != std::string::npos);
        EXPECT_TRUE(error_msg.find("test error") != std::string::npos);
    }
    
    // Test load operation error
    try {
        ul::file::throw_error("test.txt", ul::file::Operation::load, "test error");
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& e) {
        std::string_view error_msg{e.what()};
        EXPECT_TRUE(error_msg.find("test.txt") != std::string::npos);
        EXPECT_TRUE(error_msg.find("loaded") != std::string::npos);
        EXPECT_TRUE(error_msg.find("test error") != std::string::npos);
    }
}
