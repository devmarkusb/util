#include "ul/matrix.h"

#include "gtest/gtest.h"

namespace math = ul::math;

namespace {
using Int = int;

constexpr auto col_vec1{math::Matrix<Int, 2, 1>{{{1}, {2}}}};
constexpr auto row_vec1{math::Matrix<Int, 1, 2>{{{1, 2}}}};
constexpr auto scalar1{math::Matrix<Int, 1, 1>{{{5}}}};
constexpr auto matrix2x2_1{math::Matrix<Int, 2, 2>{{{1, 2}, {2, 4}}}};
constexpr auto matrix2x2_2{math::Matrix<Int, 2, 2>{{{5, 10}, {10, 20}}}};
constexpr auto col_vec2{math::Matrix<Int, 2, 1>{{{5}, {10}}}};
constexpr auto row_vec2{math::Matrix<Int, 1, 2>{{{5, 10}}}};
} // namespace

TEST(multiply, tests) {
    using math::operator<<;
    using math::operator*;
    std::cout << col_vec1 << "\n";
    std::cout << row_vec1 << "\n";
    std::cout << scalar1 << "\n";
    std::cout << matrix2x2_1 << "\n";
    std::cout << matrix2x2_2 << "\n";
    std::cout << col_vec2 << "\n";
    std::cout << row_vec2 << "\n";
    EXPECT_EQ(math::multiply(row_vec1, col_vec1, std::plus<Int>{}, std::multiplies<Int>{}), scalar1);
    EXPECT_EQ(math::multiply(col_vec1, row_vec1, std::plus<Int>{}, std::multiplies<Int>{}), matrix2x2_1);
    EXPECT_EQ(math::multiply(matrix2x2_1, col_vec1, std::plus<Int>{}, std::multiplies<Int>{}), col_vec2);
    EXPECT_EQ(math::multiply(row_vec1, matrix2x2_1, std::plus<Int>{}, std::multiplies<Int>{}), row_vec2);
    EXPECT_EQ(math::multiply(matrix2x2_1, matrix2x2_1, std::plus<Int>{}, std::multiplies<Int>{}), matrix2x2_2);
    EXPECT_EQ(math::multiply(matrix2x2_1, matrix2x2_1), matrix2x2_2);
    EXPECT_EQ(matrix2x2_1 * matrix2x2_1, matrix2x2_2);
}

// aka social_net_paths
TEST(power_semigroup, transitive_closure) {
    constexpr auto dim{7};
    using ElemT = bool;
    using Matrix = math::Matrix<ElemT, dim, dim>;
    // NOLINTBEGIN
    // aka directFriends
    constexpr auto adjacencyMatrix{Matrix{{
        {1, 1, 0, 1, 0, 0, 0},
        {1, 1, 0, 0, 0, 1, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {1, 0, 1, 1, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 1},
        {0, 1, 0, 1, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 1},
    }}};
    // NOLINTEND

    EXPECT_EQ(math::power_semigroup(adjacencyMatrix, 1, math::MatMulGenBool<ElemT, Matrix>{}), adjacencyMatrix);

    using math::operator<<;
    std::cout << math::power_semigroup(adjacencyMatrix, 2, math::MatMulGenBool<ElemT, Matrix>{}) << "\n";

    std::cout << math::power_semigroup(adjacencyMatrix, dim - 1, math::MatMulGenBool<ElemT, Matrix>{}) << "\n";

    // NOLINTBEGIN
    // aka friends over max. paths
    constexpr auto transitiveClosure{Matrix{{
        {1, 1, 1, 1, 0, 1, 0},
        {1, 1, 1, 1, 0, 1, 0},
        {1, 1, 1, 1, 0, 1, 0},
        {1, 1, 1, 1, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 1, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 1},
    }}};
    // NOLINTEND

    EXPECT_EQ(math::power_semigroup(adjacencyMatrix, dim - 1, math::MatMulGenBool<ElemT, Matrix>{}), transitiveClosure);
}

// aka shortest_length
TEST(power_semigroup, transitive_closure2) {
    constexpr auto dim{7};
    constexpr auto inf{math::Tropical::inf};
    using Matrix = math::Matrix<math::Tropical, dim, dim>;
    using MatMulGenTropical = math::MatMulGenTropical<math::Tropical, dim, dim, dim>;
    // aka distances (directed)
    constexpr auto weighted_adjacency_matrix{Matrix{{
        {0, 6, inf, 3, inf, inf, inf},
        {inf, 0, inf, inf, 2, 10, inf},
        {7, inf, 0, inf, inf, inf, inf},
        {inf, inf, 5, 0, inf, 4, inf},
        {inf, inf, inf, inf, 0, inf, 3},
        {inf, inf, 6, inf, 7, 0, 8},
        {inf, 9, inf, inf, inf, inf, 0},
    }}};

    EXPECT_EQ(math::power_semigroup(weighted_adjacency_matrix, 1, MatMulGenTropical{}), weighted_adjacency_matrix);

    using math::operator<<;
    std::cout << math::power_semigroup(weighted_adjacency_matrix, 2, MatMulGenTropical{}) << "\n";

    std::cout << math::power_semigroup(weighted_adjacency_matrix, dim - 1, MatMulGenTropical{}) << "\n";

    // aka the shortest paths to everywhere
    constexpr auto transitive_closure{Matrix{{
        {0, 6, 8, 3, 8, 7, 11},
        {23, 0, 16, 26, 2, 10, 5},
        {7, 13, 0, 10, 15, 14, 18},
        {12, 18, 5, 0, 11, 4, 12},
        {35, 12, 28, 38, 0, 22, 3},
        {13, 17, 6, 16, 7, 0, 8},
        {32, 9, 25, 35, 11, 19, 0},
    }}};

    EXPECT_EQ(math::power_semigroup(weighted_adjacency_matrix, dim - 1, MatMulGenTropical{}), transitive_closure);
}
