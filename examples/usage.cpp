#include "mb/ul/ul.hpp"

#include "gsl/gsl"
#include <iostream>
#include <print>

int main() {
    try {
        // random feature from ul
        constexpr auto example_nr{static_cast<int32_t>(std::numeric_limits<int8_t>::max())};
        std::println("narrowing {} int to int8_t: {}", example_nr, ul::narrow<int8_t>(example_nr));

        // random feature from gsl
        gsl::owner<int*> p{new int{42}}; // NOLINT // NOSONAR
        const auto _{ul::finally([&p]() {
            delete p; // NOLINT // NOSONAR
        })};
        UL_ASSERT(p);
        std::println("p: {}", *p);

        // random feature from utf8 (via ul/string)
        const std::string s = "\xe4\xf6\xfc"; // latin1 (aeoeue)
        std::print("{} valid utf8: {}", s, utf8::is_valid(s));

        return ul::prog_exit_success;
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n";
        return ul::prog_exit_failure;
    }
}
