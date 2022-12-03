/** Includes some physical tests on the filesystem. The playground are subdirs under root's tmp dir.
    *Important* From time to time you also want to test compliance of the tests with the standard library
    implementation. For that to happen, please set UL_STD_EXT_FILESYSTEM_FORCE_OWN_IMPL to 0, which only tests our
    own implementation by default otherwise.*/
#if UL_OS_LINUX
#define UL_STD_EXT_FILESYSTEM_FORCE_OWN_IMPL 1
#endif
#include "ul/std/std_ext_filesystem.h"
#undef UL_STD_EXT_FILESYSTEM_FORCE_OWN_IMPL
#include "ul/finally.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <chrono>
#include <string>
#include <thread>

#include "ul/macros.h"

namespace ul = mb::ul;
namespace fs = ul::std_fs;


namespace
{
namespace
{
const std::string physical_test_dir_s{"/tmp/ul/test"};
} // namespace
const fs::path physical_test_dir{physical_test_dir_s};

//! \return finally reversed digits of increasing numbers. Source is a steady clock of the system in milliseconds.
/** There is also a helping sleep for 1ms included. Features preferred changing of leading digits.
    Note, this implies a decrease every tenth step and possible leading zeros.*/
std::string getSteadyUniqueNr()
{
    std::string ret = std::to_string(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
            .count());
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::reverse(std::begin(ret), std::end(ret));
    return ret;
}
} // namespace

TEST(pathTest, constr_assign)
{
    fs::path p;
    p = "foo";
    EXPECT_EQ(p, fs::path{"foo"});
    p = std::string{"bar"};
    EXPECT_EQ(p, fs::path{"bar"});
}

TEST(pathTest, empty)
{
    const fs::path empty;
    EXPECT_TRUE(empty.empty());
    const fs::path dot{"."};
    EXPECT_TRUE(!dot.empty());
}

TEST(pathTest, extension)
{
    const fs::path empty;
    EXPECT_EQ(empty.extension(), empty);

    const fs::path usual{"/home/x/test.txt"};
    EXPECT_EQ(usual.extension(), fs::path{".txt"});

    const fs::path empty_ext{"/home/x/test"};
    EXPECT_EQ(empty_ext.extension(), empty);

    const fs::path dot{"."};
    EXPECT_EQ(dot.extension(), empty);

    const fs::path empty_ext_dot{"/home/x/test."};
    EXPECT_EQ(empty_ext_dot.extension(), dot);

    const fs::path ext_only{"/tmp/.sh"};
    EXPECT_EQ(ext_only.extension(), empty);

    const fs::path usual_justfile{"file.extension"};
    EXPECT_EQ(usual_justfile.extension(), fs::path{".extension"});
}

TEST(pathTest, stem)
{
    const fs::path empty;
    EXPECT_EQ(empty.stem(), empty);

    const fs::path usual{"/home/x/test.txt"};
    EXPECT_EQ(usual.stem(), fs::path{"test"});

    const fs::path empty_ext{"/home/x/test"};
    EXPECT_EQ(empty_ext.stem(), fs::path{"test"});

    const fs::path dot{"."};
    EXPECT_EQ(dot.stem(), fs::path{"."});

    const fs::path empty_ext_dot{"/home/x/test."};
    EXPECT_EQ(empty_ext_dot.stem(), fs::path{"test"});

    const fs::path ext_only{"/tmp/.sh"};
    EXPECT_EQ(ext_only.stem(), fs::path{".sh"});

    const fs::path usual_justfile{"file.extension"};
    EXPECT_EQ(usual_justfile.stem(), fs::path{"file"});
}

TEST(pathTest, stem_ext_example)
{
    fs::path p{"foo.bar.baz.tar"};
    EXPECT_EQ(p.extension(), fs::path{".tar"});
    p = p.stem();
    EXPECT_EQ(p.extension(), fs::path{".baz"});
    p = p.stem();
    EXPECT_EQ(p.extension(), fs::path{".bar"});
    p = p.stem();
    EXPECT_TRUE(p.extension().empty());
}

TEST(pathTest, filename)
{
    const fs::path empty;
    EXPECT_EQ(empty.filename(), empty);

    const fs::path usual{"/home/x/test.txt"};
    EXPECT_EQ(usual.filename(), fs::path{"test.txt"});

    const fs::path empty_ext{"/home/x/test"};
    EXPECT_EQ(empty_ext.filename(), fs::path{"test"});

    const fs::path dot{"."};
    EXPECT_EQ(dot.filename(), fs::path{"."});

    const fs::path empty_ext_dot{"/home/x/test."};
    EXPECT_EQ(empty_ext_dot.filename(), fs::path{"test."});

    const fs::path ext_only{"/tmp/.sh"};
    EXPECT_EQ(ext_only.filename(), fs::path{".sh"});

    const fs::path usual_justfile{"file.extension"};
    EXPECT_EQ(usual_justfile.filename(), fs::path{"file.extension"});
}

TEST(pathTest, parent_path)
{
    const fs::path empty;
    EXPECT_EQ(empty.parent_path(), empty);

    const fs::path usual{"/home/x/test.txt"};
    EXPECT_EQ(usual.parent_path(), fs::path{"/home/x"});

    const fs::path empty_ext{"/home/x/test"};
    EXPECT_EQ(empty_ext.parent_path(), fs::path{"/home/x"});

    const fs::path dot{"."};
    EXPECT_EQ(dot.parent_path(), empty);

    const fs::path empty_ext_dot{"/home/x/test."};
    EXPECT_EQ(empty_ext_dot.parent_path(), fs::path{"/home/x"});

    const fs::path ext_only{"/tmp/.sh"};
    EXPECT_EQ(ext_only.parent_path(), fs::path{"/tmp"});

    const fs::path usual_justfile{"file.extension"};
    EXPECT_EQ(usual_justfile.parent_path(), empty);

    const fs::path root{"/"};
    EXPECT_EQ(root.parent_path(), root);

    const fs::path dot_abs{"/tmp/."};
    EXPECT_EQ(dot_abs.parent_path(), fs::path{"/tmp"});

    const fs::path dir{"/tmp/dir/"};
    EXPECT_EQ(dir.parent_path(), fs::path{"/tmp/dir"});

    const fs::path root_dir{"/tmp"};
    EXPECT_EQ(root_dir.parent_path(), root);

    const fs::path root_file{"/tmp.txt"};
    EXPECT_EQ(root_file.parent_path(), root);
}

TEST(pathTest, replace_extension)
{
    const fs::path empty;
    fs::path empty_cp{empty};
    empty_cp.replace_extension(".txt");
    EXPECT_EQ(empty_cp, fs::path{".txt"});

    fs::path usual{"/home/x/test.txt"};
    usual.replace_extension(".md");
    EXPECT_EQ(usual, fs::path{"/home/x/test.md"});

    fs::path empty_ext{"/home/x/test"};
    empty_ext.replace_extension(".txt");
    EXPECT_EQ(empty_ext, fs::path{"/home/x/test.txt"});

    fs::path dot{"."};
    dot.replace_extension(".txt");
    EXPECT_STREQ(dot.string().c_str(), "..txt");

    fs::path empty_ext_dot{"/home/x/test."};
    empty_ext_dot.replace_extension();
    EXPECT_EQ(empty_ext_dot, fs::path{"/home/x/test"});

    fs::path ext_only{"/tmp/.hiddenfilename"};
    ext_only.replace_extension(".extension");
    EXPECT_EQ(ext_only, fs::path{"/tmp/.hiddenfilename.extension"});

    fs::path usual_justfile{"file.replace_extension"};
    usual_justfile.replace_extension();
    EXPECT_EQ(usual_justfile, fs::path{"file"});

    fs::path dot_in_parent{"sub.dir/file"};
    dot_in_parent.replace_extension();
    EXPECT_EQ(dot_in_parent, fs::path{"sub.dir/file"});
}

TEST(pathTest, eq)
{
    const fs::path lhs{"/tmp/foo/"};
    const fs::path rhs{"/tmp/foo"};
    EXPECT_TRUE(lhs != rhs);

    const fs::path empty;
    const fs::path dot{"."};
    EXPECT_TRUE(empty != dot);
}

TEST(pathTest, concat)
{
    fs::path some{"/tmp/foo"};
    some /= "bar";
    EXPECT_EQ(some, fs::path{"/tmp/foo/bar"});

    some += '/';
    EXPECT_EQ(some, fs::path{"/tmp/foo/bar/"});

    some += fs::path{"readme.md"};
    EXPECT_EQ(some, fs::path{"/tmp/foo/bar/readme.md"});
}

TEST(pathTest, append)
{
    const fs::path some1{"/tmp/foo"};
    const fs::path some2{"bar"};
    fs::path some{some1 / some2};
    EXPECT_EQ(some, fs::path{"/tmp/foo/bar"});
#if !UL_OS_WINDOWS
    some /= fs::path{};
    EXPECT_EQ(some, fs::path{"/tmp/foo/bar/"});
#endif
    some /= fs::path{"readme.md"};
    EXPECT_EQ(some, fs::path{"/tmp/foo/bar/readme.md"});

    const fs::path empty;
    fs::path current_some{empty};
    current_some /= fs::path{"sub/file"};
    EXPECT_EQ(current_some, fs::path{"sub/file"});

    const fs::path dot{"."};
    fs::path current_dot_some{dot};
    current_dot_some /= fs::path{"sub/file"};
    EXPECT_EQ(current_dot_some, fs::path{"./sub/file"});
}

class PhysicalFilesystemTest : public ::testing::Test
{
    void SetUp() override
    {
        std::error_code ec;
        fs::create_directories(physical_test_dir, ec);
        ASSERT_FALSE(ec);
    }

    void TearDown() override
    {
        // don't risk deleting anything important accidentally, just keep the playground's base dir and content
        UL_NOOP;
    }
};

TEST_F(PhysicalFilesystemTest, create_directories__single__exists_correcttype)
{
    const fs::path dir1 = physical_test_dir / fs::path{getSteadyUniqueNr() + "dir"};
    const fs::path dir2 = physical_test_dir / fs::path{getSteadyUniqueNr() + "dir"};
    std::error_code ec;
    EXPECT_TRUE(fs::create_directories(dir1, ec));
    ASSERT_FALSE(ec);
    EXPECT_FALSE(fs::create_directories(dir1, ec));
    ASSERT_FALSE(ec);
    EXPECT_TRUE(fs::create_directories(dir2, ec));
    ASSERT_FALSE(ec);
    EXPECT_FALSE(fs::create_directories(dir2, ec));
    ASSERT_FALSE(ec);

    EXPECT_TRUE(fs::exists(dir1));
    EXPECT_TRUE(fs::exists(dir2));

    EXPECT_TRUE(!fs::is_regular_file(dir1));
    EXPECT_TRUE(!fs::is_regular_file(dir2));
}

TEST_F(PhysicalFilesystemTest, create_directories__recursive__exists_correcttype)
{
    const std::string subdirs_base = getSteadyUniqueNr() + "subdirs";

    const fs::path subdirs{physical_test_dir / fs::path{subdirs_base} / fs::path{"d1/d2/d3"}};
    std::error_code ec;
    EXPECT_TRUE(fs::create_directories(subdirs, ec));
    ASSERT_FALSE(ec);
    EXPECT_FALSE(fs::create_directories(subdirs, ec));
    ASSERT_FALSE(ec);

    EXPECT_TRUE(fs::exists(subdirs));

    EXPECT_TRUE(!fs::is_regular_file(subdirs));
}

TEST_F(PhysicalFilesystemTest, exists__file_correcttype)
{
    const auto file = physical_test_dir / fs::path{"dummy" + getSteadyUniqueNr()};
    EXPECT_TRUE(!fs::exists(file));
    ul::file::touch(file);
    EXPECT_TRUE(fs::exists(file));
    EXPECT_TRUE(fs::is_regular_file(file));
}

TEST_F(PhysicalFilesystemTest, copy_file__file_existence)
{
    const auto file = physical_test_dir / fs::path{"dummy-to-copy" + getSteadyUniqueNr()};
    EXPECT_TRUE(!fs::exists(file));
    ul::file::touch(file);
    fs::path dest{file};
    dest += "-copy";
    EXPECT_TRUE(!fs::exists(dest));
    std::error_code ec;
    ASSERT_TRUE(fs::copy_file(file, dest, ec));
    ASSERT_FALSE(ec);
    EXPECT_TRUE(fs::exists(dest));
}

TEST_F(PhysicalFilesystemTest, remove__file_existence)
{
    const auto file = physical_test_dir / fs::path{"deadbeef" + getSteadyUniqueNr()};
    ASSERT_TRUE(!fs::exists(file));
    ul::file::touch(file);
    ASSERT_TRUE(fs::exists(file));
    ASSERT_TRUE(fs::remove(file));
    EXPECT_TRUE(!fs::exists(file));
}

TEST_F(PhysicalFilesystemTest, remove__dir_existence)
{
    const auto dir = physical_test_dir / fs::path{"deadbeef" + getSteadyUniqueNr()};
    ASSERT_TRUE(!fs::exists(dir));
    std::error_code ec;
    fs::create_directories(dir, ec);
    ASSERT_FALSE(ec);
    ASSERT_TRUE(fs::exists(dir));
    ASSERT_TRUE(fs::remove(dir));
    EXPECT_TRUE(!fs::exists(dir));
}

TEST_F(PhysicalFilesystemTest, current_path__get_set)
{
    std::error_code ec;
    const auto orig = fs::current_path(ec);
    ASSERT_FALSE(ec);
    std::cout << orig.string() << "\n";
    fs::current_path(physical_test_dir, ec);
    ASSERT_FALSE(ec);
    const auto chdir_back_to_orig = ul::finally([&orig]() {
        std::error_code ec_;
        fs::current_path(orig, ec_);
        EXPECT_FALSE(ec_);
    });
    const auto changed_dir = fs::current_path(ec);
    std::cout << changed_dir.string() << "\n";
    std::cout << physical_test_dir.string() << "\n";
#if UL_OS_WINDOWS
    const auto comparable_changed_dir = changed_dir.root_directory() / changed_dir.relative_path();
    EXPECT_TRUE(comparable_changed_dir == physical_test_dir);
#elif UL_OS_MAC
    auto comparable_physical_dir = "/private" + physical_test_dir_s;
    EXPECT_TRUE(changed_dir == comparable_physical_dir);
#else
    EXPECT_TRUE(changed_dir == physical_test_dir);
#endif
    EXPECT_FALSE(ec);
}

TEST_F(PhysicalFilesystemTest, directory_iterator)
{
    const fs::path dir = physical_test_dir / fs::path{"some" + getSteadyUniqueNr()};
    std::error_code ec;
    fs::create_directories(dir, ec);

    ul::file::touch(dir / "f1");
    fs::create_directories(dir / "sub", ec);
    ASSERT_FALSE(ec);
    ul::file::touch(dir / "f2");
    ul::file::touch(dir / "f3");

    fs::directory_iterator begin(dir);
    fs::directory_iterator end;
    std::vector<fs::directory_entry> entries(begin, end);
    ASSERT_EQ(entries.size(), 4);
    EXPECT_EQ(std::count(std::begin(entries), std::end(entries), dir / "f1"), 1);
    EXPECT_EQ(std::count(std::begin(entries), std::end(entries), dir / "f2"), 1);
    EXPECT_EQ(std::count(std::begin(entries), std::end(entries), dir / "f3"), 1);
    EXPECT_EQ(std::count(std::begin(entries), std::end(entries), dir / "sub"), 1);
}
