/** Note: many unit tests here are in fact just compilation tests against concept definitions.
    Note: don't add more _check function if not absolutely needed. They are superseeded by just checking concepts
    in static_asserts.*/

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

template <ul::most_generic::FunctionalProcedure F>
void functional_procedure_input0_check(F, ul::InputType<0, F>) {
}

template <ul::most_generic::HomogeneousFunction F>
void homogeneous_procedure_domain_check(F, ul::Domain<F>) {
}

[[maybe_unused]] void func_proc_ex1() {
}

[[maybe_unused]] int func_proc_ex2() {
    return {};
}

[[maybe_unused]] int func_proc_ex3(double) {
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

[[maybe_unused]] bool func_proc_ex7(double, double) {
    return {};
}

[[maybe_unused]] bool func_proc_ex8() {
    return {};
}

[[maybe_unused]] bool func_proc_ex9(int) {
    return {};
}

[[maybe_unused]] int func_proc_ex10(int, int, int) {
    return {};
}

[[maybe_unused]] char func_proc_ex11(int, int) {
    return {};
}

[[maybe_unused]] int func_proc_ex12(std::string_view) {
    return {};
}
} // namespace

struct RegularEx1 {};

bool operator==(const RegularEx1&, const RegularEx1&) {
    return true;
}

// Example, where the work of defining the required trait isn't done automagically for you.
namespace mb::ul {
template <>
struct InputTypeDecl<1, decltype(&func_proc_ex4)> {
    using Type = char;
};
} // namespace mb::ul

TEST(RegularTest, misc) {
    static_assert(ul::Regular<RegularEx1>);
    regular_check(RegularEx1{});
    // regular_check(RegularEx2{}); // expectedly non-compiling
    static_assert(ul::Regular<int>);
    regular_check(int{});
    static_assert(ul::Regular<std::string>);
    regular_check(std::string{});
    const auto string_ex{std::string{"abc"}};
    static_assert(ul::Regular<std::pair<std::string, int>>);
    regular_check(std::make_pair(string_ex, int_ex));
    static_assert(ul::RegularTotallyOrdered<std::tuple<std::string, int>>);
    regular_totally_ordered_check(std::make_pair(string_ex, int_ex));
    static_assert(ul::RegularTotallyOrdered<std::tuple<int, char, double>>);
    regular_totally_ordered_check(std::make_tuple(int_ex, char_ex, double_ex));
    static_assert(ul::RegularTotallyOrdered<int>);
    regular_totally_ordered_check(int{});

    static_assert(ul::SemiRegular<SemiRegularEx1>);
    semi_regular_check(SemiRegularEx1{});
    static_assert(ul::SemiRegular<RegularEx1>);
    semi_regular_check(RegularEx1{});
    static_assert(ul::SemiRegular<int>);
    semi_regular_check(int{});
    static_assert(ul::SemiRegular<std::string>);
    semi_regular_check(std::string{});
}

TEST(FunctionalProcedure, misc) {
    static_assert(!ul::FunctionalProcedure<decltype([]() {
        return int_ex;
    })>); // no arg
    static_assert(ul::FunctionalProcedure<
                  decltype([](double) {
                      return int_ex;
                  }),
                  double>);
    int i{}; // NOLINT (const, but want to capture)
    // unfortunately true, we actually don't want this, capturing lambda
    static_assert(
        ul::FunctionalProcedure<
            decltype([i](int) {
                return i;
            }),
            int>,
        "actually expected and true so!");
    static_assert(!ul::FunctionalProcedure<decltype(&func_proc_ex1)>); // void return
    static_assert(!ul::FunctionalProcedure<decltype([]() {
    })>); // void return
    static_assert(!ul::FunctionalProcedure<decltype(&func_proc_ex2)>); // no arg
    static_assert(ul::FunctionalProcedure<decltype(&func_proc_ex3), double>);
    static_assert(ul::FunctionalProcedure<decltype(&func_proc_ex4), double, char>);
    static_assert(ul::FunctionalProcedure<FuncProcEx5, double, char>);

    static_assert(std::is_same_v<ul::InputType<0, decltype(&func_proc_ex4)>, double>);
    functional_procedure_input0_check(func_proc_ex4, double_ex);
    static_assert(std::is_same_v<ul::Domain<decltype(&func_proc_ex6)>, double>);
    homogeneous_procedure_domain_check(func_proc_ex6, double_ex);
    static_assert(std::is_same_v<
                  ul::Domain<decltype([](double) {
                      return int_ex;
                  })>,
                  double>);
    homogeneous_procedure_domain_check(
        [](double) {
            return int_ex;
        },
        double_ex);
    static_assert(
        std::is_same_v<
            ul::Domain<decltype([i](double) {
                return i;
            })>,
            double>,
        "actually expected and true so!");
    homogeneous_procedure_domain_check(
        [i](double) {
            return i;
        },
        double_ex); // still unfortunately compiling yet, capturing lambda

    static_assert(std::is_same_v<ul::Codomain<decltype(&func_proc_ex6)>, int>);
    static_assert(std::is_same_v<ul::Domain<decltype(&func_proc_ex4)>, double>);
    static_assert(std::is_same_v<ul::InputType<1, decltype(&func_proc_ex4)>, char>);
    static_assert(std::is_same_v<ul::Codomain<decltype(&func_proc_ex4)>, int>);
    static_assert(std::is_same_v<
                  ul::Codomain<decltype([](double) {
                      return int_ex;
                  })>,
                  int>);
}

TEST(UnaryFunction, misc) {
    static_assert(!ul::UnaryFunction<decltype(&func_proc_ex2)>);
    static_assert(ul::UnaryFunction<decltype(&func_proc_ex3)>);
    static_assert(!ul::UnaryFunction<decltype(&func_proc_ex4)>);
    static_assert(!ul::UnaryFunction<decltype([](int) {
    })>);
}

TEST(Predicate, misc) {
    static_assert(!ul::Predicate<decltype(&func_proc_ex3)>);
    static_assert(ul::Predicate<decltype(&func_proc_ex7), double, double>);
    static_assert(!ul::Predicate<decltype(&func_proc_ex8)>); // no arg
    static_assert(ul::UnaryPredicate<decltype(&func_proc_ex9)>);
}

TEST(Operation, misc) {
    static_assert(ul::Operation<decltype(&func_proc_ex10), int, int, int>);
    static_assert(ul::Operation<decltype(&func_proc_ex11), int, int>); // 'narrowing' ok, result space might still fit
    static_assert(!ul::Operation<decltype(&func_proc_ex12), std::string>);
    static_assert(ul::BinaryOperation<decltype(&func_proc_ex11)>);
    static_assert(ul::UnaryOperation<decltype(&func_proc_ex9)>);
}
