#include "ul/basiccodesupport/file.hpp" // NOLINT
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>

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
    const auto get_error_message = [](ul::file::Operation op) -> std::string {
        try {
            ul::file::throw_error("test.txt", op, "test error");
        } catch (const std::runtime_error& e) {
            return e.what();
        } catch (...) {
            ADD_FAILURE() << "Expected std::runtime_error";
        }
        return {};
    };

    // Test save operation error
    const auto save_error_msg = get_error_message(ul::file::Operation::save);
    EXPECT_FALSE(save_error_msg.empty());
    EXPECT_TRUE(save_error_msg.find("test.txt") != std::string::npos);
    EXPECT_TRUE(save_error_msg.find("saved") != std::string::npos);
    EXPECT_TRUE(save_error_msg.find("test error") != std::string::npos);

    // Test load operation error
    const auto load_error_msg = get_error_message(ul::file::Operation::load);
    EXPECT_FALSE(load_error_msg.empty());
    EXPECT_TRUE(load_error_msg.find("test.txt") != std::string::npos);
    EXPECT_TRUE(load_error_msg.find("loaded") != std::string::npos);
    EXPECT_TRUE(load_error_msg.find("test error") != std::string::npos);
}
