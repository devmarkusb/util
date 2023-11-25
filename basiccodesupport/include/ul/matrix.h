/** \file*/

#ifndef MATRIX_H_EMUHXOIUHGOI54387HJO63425
#define MATRIX_H_EMUHXOIUHGOI54387HJO63425

#include "algebra.h"
#include "foundations.h"
#include "ul/config.h"
#if __has_include(<concepts>)
#include <concepts>
#endif
#include <functional>
#include <ostream>

#if __cpp_concepts && __cpp_lib_concepts
namespace mb::ul::math {
template <Regular ElemT, size_t m, size_t n>
using Matrix = std::array<std::array<ElemT, n>, m>;

template <typename MatrixT, typename ElemT>
concept MatrixLike = requires(MatrixT m, size_t r, size_t c) {
                         m[r];
                         m[r][c];
                         requires std::is_same_v<std::remove_cvref_t<decltype(m[r][c])>, ElemT>;
                     };

// Not yet useful. Even problematic in the difficulty of inferring template parameters.
//template <typename MatrixT, typename ElemT, size_t m, size_t n>
//concept MatrixMNLike = MatrixLike<MatrixT, ElemT> && requires(MatrixT mat, size_t r, size_t c) {
//    mat[0];
//    mat[m - 1];
//    mat[0][0];
//    mat[0][n - 1];
//    mat[m - 1][0];
//    mat[m - 1][n - 1];
//};

template <Regular ElemT, size_t m, size_t n>
std::ostream& operator<<(std::ostream& os, const Matrix<ElemT, m, n>& matrix) {
    for (decltype(m) i{}; i < m; ++i) {
        os << "[";
        for (decltype(n) j{}; j < n; ++j)
            os << " " << matrix[i][j];
        os << " ]\n";
    }
    return os;
}

template <
    size_t m, size_t k, size_t n, CommutativeMonoidOperation OpElemCommutativeMonoid /*add*/,
    MonoidOperation OpElemMonoid /*multiply*/>
    requires SemiRing<OpElemCommutativeMonoid, OpElemMonoid>
auto multiply(
    const Matrix<Domain<OpElemMonoid>, m, k>& l, const Matrix<Domain<OpElemMonoid>, k, n>& r,
    OpElemCommutativeMonoid inner_add, OpElemMonoid inner_mul) {
    Matrix<Domain<OpElemMonoid>, m, n> res{};
    for (decltype(m) i{}; i < m; ++i)
        for (decltype(k) h{}; h < k; ++h)
            for (decltype(n) j{}; j < n; ++j)
                res[i][j] = inner_add(res[i][j], inner_mul(l[i][h], r[h][j]));
    return res;
}

template <SemiRingAddMult ElemT, size_t m, size_t k, size_t n>
auto multiply(const Matrix<ElemT, m, k>& l, const Matrix<ElemT, k, n>& r) {
    return multiply(l, r, std::plus<ElemT>{}, std::multiplies<ElemT>{});
}

template <SemiRingAddMult ElemT, size_t m, size_t k, size_t n>
auto operator*(const Matrix<ElemT, m, k>& l, const Matrix<ElemT, k, n>& r) {
    return multiply(l, r);
}

template <SemiRingAddMult ElemT, MatrixLike<ElemT> M>
struct MatMul {
    M operator()(const M& a1, const M& a2) const {
        return a1 * a2;
    }
};

template <SemiRingAddMult ElemT, MatrixLike<ElemT> M>
struct MatMulGenBool {
    M operator()(const M& a1, const M& a2) const {
        return multiply(a1, a2, std::logical_or<ElemT>{}, std::logical_and<ElemT>{});
    }
};

template <SemiRingAddMult ElemT, size_t m, size_t k, size_t n>
struct MatMulGenTropical {
    Matrix<ElemT, m, n> operator()(const Matrix<ElemT, m, k>& a1, const Matrix<ElemT, k, n>& a2) const {
        return multiply(a1, a2, Min<ElemT>{}, std::plus<ElemT>{});
    }
};
} // namespace mb::ul::math
#endif

UL_HEADER_END

#endif
