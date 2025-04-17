#include "ul/string/languages.h" // NOLINT
#include "gtest/gtest.h"
#include <string>
#include <vector>

namespace ul = mb::ul;

TEST(languages, get_all_theoretically_possible_lang_i_ds) {
    const auto& lang_ids = ul::str::get_all_theoretically_possible_lang_i_ds();
    
    // Check that the vector is not empty
    EXPECT_FALSE(lang_ids.empty());
    
    // Check that it contains expected language codes
    EXPECT_TRUE(std::find(lang_ids.begin(), lang_ids.end(), "en") != lang_ids.end());
    EXPECT_TRUE(std::find(lang_ids.begin(), lang_ids.end(), "de") != lang_ids.end());
    EXPECT_TRUE(std::find(lang_ids.begin(), lang_ids.end(), "fr") != lang_ids.end());
    
    // Check that it contains all expected languages
    std::vector<std::string> expected = {
        "ar", "bn", "de", "en", "es", "fr", "hi", "it", "ja", "ko", "pt", "ru", "zh"
    };
    
    for (const auto& expected_lang : expected) {
        EXPECT_TRUE(std::find(lang_ids.begin(), lang_ids.end(), expected_lang) != lang_ids.end())
            << "Expected language code " << expected_lang << " not found";
    }
}

TEST(languages, get_native_language_name) {
    // Test with valid language codes
    EXPECT_EQ("Deutsch", ul::str::get_native_language_name("de"));
    EXPECT_EQ("English", ul::str::get_native_language_name("en"));
    EXPECT_EQ("Français", ul::str::get_native_language_name("fr"));
    
    // Test with auto_systems_language (should throw)
    EXPECT_ANY_THROW(ul::str::get_native_language_name(ul::str::auto_systems_language.data()));
    
    // Test with invalid language code (should throw)
    EXPECT_THROW(ul::str::get_native_language_name("xx"), ul::NotImplemented);
}

TEST(languages, get_english_language_name) {
    // Test with valid language codes
    EXPECT_EQ("German", ul::str::get_english_language_name("de"));
    EXPECT_EQ("English", ul::str::get_english_language_name("en"));
    EXPECT_EQ("French", ul::str::get_english_language_name("fr"));
    
    // Test with auto_systems_language (should throw)
    EXPECT_ANY_THROW(ul::str::get_english_language_name(ul::str::auto_systems_language.data()));
    
    // Test with invalid language code (should throw)
    EXPECT_THROW(ul::str::get_english_language_name("xx"), ul::NotImplemented);
}

TEST(languages, language_name_consistency) {
    // Test that all language codes have both native and English names
    const auto& lang_ids = ul::str::get_all_theoretically_possible_lang_i_ds();
    
    for (const auto& lang_id : lang_ids) {
        // Skip auto_systems_language as it's not a real language code
        if (lang_id == ul::str::auto_systems_language) {
            continue;
        }
        
        // Both functions should return non-empty strings for valid language codes
        EXPECT_FALSE(ul::str::get_native_language_name(lang_id).empty());
        EXPECT_FALSE(ul::str::get_english_language_name(lang_id).empty());
    }
}
