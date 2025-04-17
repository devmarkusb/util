#include "ul/finally.h"
#include "gtest/gtest.h"
#include <functional>

TEST(FinallyTest, BasicFunctionality) {
    int counter = 0;
    
    {
        // Create a finally action that increments the counter
        auto cleanup = mb::ul::finally([&counter]() {
            ++counter;
        });
        
        // Counter should still be 0
        EXPECT_EQ(counter, 0);
    }
    
    // After the block ends, the finally action should have been executed
    EXPECT_EQ(counter, 1);
}

TEST(FinallyTest, MultipleFinallyActions) {
    int counter = 0;
    
    {
        // Create multiple finally actions
        auto cleanup1 = mb::ul::finally([&counter]() {
            ++counter;
        });
        
        auto cleanup2 = mb::ul::finally([&counter]() {
            counter += 2;
        });
        
        // Counter should still be 0
        EXPECT_EQ(counter, 0);
    }
    
    // After the block ends, both finally actions should have been executed
    // in reverse order of creation (LIFO)
    EXPECT_EQ(counter, 3);
}

TEST(FinallyTest, MoveSemantics) {
    int counter = 0;
    
    {
        // Create a finally action
        auto cleanup1 = mb::ul::finally([&counter]() {
            ++counter;
        });
        
        // Move it to another variable
        auto cleanup2 = std::move(cleanup1);
        
        // Counter should still be 0
        EXPECT_EQ(counter, 0);
    }
    
    // After the block ends, the finally action should have been executed only once
    EXPECT_EQ(counter, 1);
}

TEST(FinallyTest, ExceptionHandling) {
    int counter = 0;
    
    try {
        // Create a finally action
        auto cleanup = mb::ul::finally([&counter]() {
            ++counter;
        });
        
        // Throw an exception
        throw std::runtime_error("Test exception");
    } catch (const std::exception&) {
        // Exception caught
    }
    
    // Even though an exception was thrown, the finally action should have been executed
    EXPECT_EQ(counter, 1);
}

TEST(FinallyTest, LambdaCapture) {
    int value = 42;
    int captured_value = 0;
    
    {
        // Create a finally action that captures 'value' by value
        auto cleanup = mb::ul::finally([value, &captured_value]() {
            captured_value = value;
        });
        
        // Change the original value
        value = 100;
    }
    
    // The captured value should be the original value (42), not the changed value (100)
    EXPECT_EQ(captured_value, 42);
}

TEST(FinallyTest, ReferenceCapture) {
    int value = 42;
    int captured_value = 0;
    
    {
        // Create a finally action that captures 'value' by reference
        auto cleanup = mb::ul::finally([&value, &captured_value]() {
            captured_value = value;
        });
        
        // Change the original value
        value = 100;
    }
    
    // The captured value should be the changed value (100), not the original value (42)
    EXPECT_EQ(captured_value, 100);
}

TEST(FinallyTest, TemporaryObject) {
    int counter = 0;
    
    {
        // Create a temporary finally action (not assigned to a variable)
        mb::ul::finally([&counter]() {
            ++counter;
        });
        
        // Counter should already be 1 because the temporary was destroyed immediately
        EXPECT_EQ(counter, 1);
    }
    
    // Counter should still be 1
    EXPECT_EQ(counter, 1);
}
