#include "ul/shared_instance.h"
#include "gtest/gtest.h"
#include <thread>
#include <vector>
#include <atomic>

namespace ul = mb::ul;

namespace {
constexpr auto some_number_choice{42};
} // namespace

TEST(getSharedInstance, basics) {
    auto i1 = ul::get_shared_instance<int>();
    // yes you can assume the implementation to call a value initializing make_shared (for POD types)
    EXPECT_EQ(*i1, 0);
    *i1 = some_number_choice;
    auto i2 = i1;
    i1 = {};
    EXPECT_EQ(*i2, some_number_choice);
    i2 = {};

    i1 = ul::get_shared_instance<int>();
    *i1 = some_number_choice;
    i2 = ul::get_shared_instance<int>();
    EXPECT_EQ(*i2, some_number_choice);
    i1 = {};
    i2 = ul::get_shared_instance<int>();
    EXPECT_EQ(*i2, some_number_choice);
    i2 = {};
    i2 = ul::get_shared_instance<int>();
    EXPECT_EQ(*i2, 0);
}

// Test class with a counter to track instances
class TestClass {
public:
    static inline std::atomic<int> instance_count{};
    
    TestClass() {
        ++instance_count;
    }
    
    ~TestClass() {
        --instance_count;
    }

    static int get_value() {
        return 42;
    }
};

TEST(SharedInstanceTest, BasicFunctionality) {
    // Reset instance count
    TestClass::instance_count = 0;
    
    // Get first instance
    auto instance1 = ul::get_shared_instance<TestClass>();
    EXPECT_EQ(TestClass::instance_count, 1);
    EXPECT_EQ(instance1->get_value(), 42);
    
    // Get second instance (should be the same)
    auto instance2 = ul::get_shared_instance<TestClass>();
    EXPECT_EQ(TestClass::instance_count, 1); // Still only one instance
    EXPECT_EQ(instance2->get_value(), 42);
    
    // Verify they are the same instance
    EXPECT_EQ(instance1.get(), instance2.get());
    
    // Release first instance
    instance1.reset();
    EXPECT_EQ(TestClass::instance_count, 1); // Still one instance
    
    // Release second instance
    instance2.reset();
    EXPECT_EQ(TestClass::instance_count, 0); // No instances left
}

TEST(SharedInstanceTest, ThreadSafety) {
    // Reset instance count
    TestClass::instance_count = 0;
    
    const int num_threads = 10;
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<TestClass>> instances(num_threads);
    
    // Create threads that each get a shared instance
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&instances, i]() {
            instances[i] = ul::get_shared_instance<TestClass>();
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Verify only one instance was created
    EXPECT_EQ(TestClass::instance_count, 1);
    
    // Verify all instances are the same
    for (int i = 1; i < num_threads; ++i) {
        EXPECT_EQ(instances[0].get(), instances[i].get());
    }
    
    // Release all instances
    for (auto& instance : instances) {
        instance.reset();
    }
    
    // Verify no instances are left
    EXPECT_EQ(TestClass::instance_count, 0);
}

// Test with a POD type
TEST(SharedInstanceTest, WithPODType) {
    auto int_instance1 = ul::get_shared_instance<int>();
    auto int_instance2 = ul::get_shared_instance<int>();
    
    // Verify they are the same instance
    EXPECT_EQ(int_instance1.get(), int_instance2.get());
    
    // Verify POD type is value-initialized (to 0)
    EXPECT_EQ(*int_instance1, 0);
    
    // Modify the value
    *int_instance1 = 42;
    EXPECT_EQ(*int_instance2, 42); // Other instance sees the change
}

// Test with a class that has a constructor with parameters
// This should not compile, but we'll include it as a comment to document the limitation
/*
class ParametrizedClass {
public:
    ParametrizedClass(int value) : value_(value) {}
    int get_value() const { return value_; }
private:
    int value_;
};

TEST(SharedInstanceTest, WithParametrizedClass) {
    // This should not compile
    auto instance = ul::get_shared_instance<ParametrizedClass>();
}
*/
