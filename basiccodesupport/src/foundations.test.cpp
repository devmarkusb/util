/** Note: many unit tests here are in fact just compilation tests against concept definitions.*/

#include "ul/foundations.h"
#include "gtest/gtest.h"
#include <string>

namespace {
const auto int_ex{42};
const auto double_ex{3.14};
const auto char_ex{'a'};

void regular_check(ul::Regular auto) {
}

struct RegularEx2 {};

void regular_totally_ordered_check(ul::RegularTotallyOrdered auto) {
}

void semi_regular_check(ul::SemiRegular auto) {
}

struct SemiRegularEx1 {};

template <ul::most_generic::FunctionalProcedure F, typename... Args>
void functional_procedure_check(F) {
}

template <ul::most_generic::FunctionalProcedure F>
void functional_procedure_input0_check(F, ul::InputType<0, F>) {
}

template <ul::most_generic::HomogeneousFunction F>
void homogeneous_procedure_domain_check(F, ul::Domain<F>) {
}

[[maybe_unused]] void func_proc_ex1() {
}

int func_proc_ex2() {
    return {};
}

int func_proc_ex3(double) {
    return {};
}

int func_proc_ex4(double, char) {
    return {};
}

struct FuncProcEx5 {
    int operator()(double, char) const {
        return {};
    }
};

int func_proc_ex6(double, double) {
    return {};
}
} // namespace

struct RegularEx1 {};

bool operator==(const RegularEx1&, const RegularEx1&) {
    return true;
}

TEST(RegularTest, misc) {
    regular_check(RegularEx1{});
    // regular_check(RegularEx2{}); // expectedly non-compiling
    regular_check(int{});
    regular_check(std::string{});
    const auto string_ex{std::string{"abc"}};
    regular_check(std::make_pair(string_ex, int_ex));
    regular_totally_ordered_check(std::make_pair(string_ex, int_ex));
    regular_totally_ordered_check(std::make_tuple(int_ex, char_ex, double_ex));
    regular_totally_ordered_check(int{});

    semi_regular_check(SemiRegularEx1{});
    semi_regular_check(RegularEx1{});
    semi_regular_check(int{});
    semi_regular_check(std::string{});
}

TEST(FunctionalProcedure, misc) {
    functional_procedure_check([]() {
        return int_ex;
    });
    functional_procedure_check([](double) {
        return int_ex;
    });
    int i{};
    // unfortunately compiling, we actually don't want this
    functional_procedure_check([i]() {
        return i;
    }); // unfortunately compiling yet, capturing lambda
    // functional_procedure_check(func_proc_ex1); // expectedly non-compiling, void return
    // functional_procedure_check([](){}); // expectedly non-compiling, void return
    functional_procedure_check(func_proc_ex2);
    functional_procedure_check(func_proc_ex3);
    functional_procedure_check(func_proc_ex4);
    functional_procedure_check(FuncProcEx5{});

    functional_procedure_input0_check(func_proc_ex4, double_ex);
    homogeneous_procedure_domain_check(func_proc_ex6, double_ex);
}
