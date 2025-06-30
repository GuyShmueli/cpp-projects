
#ifndef AUTOTEST_UTILS_H
#define AUTOTEST_UTILS_H

#include "Matrix.h"
#include "Activation.h"
#include "Dense.h"
#include "MlpNetwork.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <type_traits>

#define EPSILON_RREF 0.001


template<typename T>
struct function_traits;

template<class F>
struct function_traits;

// function pointer
template<class R, class... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)> {
};

template<class R, class... Args>
struct function_traits<R(Args...)> {
    using return_type = R;

    static constexpr std::size_t arity = sizeof...(Args);

    template<std::size_t N>
    struct argument {
        static_assert(N < arity, "error: invalid parameter index.");
        using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
};

// member function pointer
template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...)> : public function_traits<R(C &, Args...)> {
};

// const member function pointer
template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) const> : public function_traits<R(C &, Args...)> {
};

// member object pointer
template<class C, class R>
struct function_traits<R(C::*)> : public function_traits<R(C &)> {
};

bool checkSameAddress(const Matrix &lhs, const Matrix &rhs)
{
    return &lhs == &rhs;
}

Matrix get_ordered_matrix(int rows, int cols)
{
    Matrix A = Matrix(rows, cols);
    for(int i = 0; i < A.get_rows(); ++i)
    {
        for(int j = 0; j < A.get_cols(); ++j)
        {
            A[i * A.get_cols() + j] = i * A.get_cols() + j;
        }
    }
    return A;
}

bool float_compare(float a, float b)
{
    return std::abs(a - b) < EPSILON_RREF;
}

int check_equal(Matrix& A, Matrix& B)
{
    // checking correct dimensions
    if(A.get_rows() != B.get_rows() || A.get_cols() != B.get_cols())
    {
        return 1;
    }

    // checking correct new values
    for(int i = 0; i < B.get_cols() * B.get_rows(); i++)
    {
        if(!float_compare(A[i], B[i]))
        {
            return 1;
        }
    }

    return 0;
}

void fill_matrix(Matrix& M, const float* nums)
{
    for(int i = 0; i < M.get_rows(); ++i)
    {
        for(int j = 0; j < M.get_cols(); ++j)
        {
            M[i * M.get_cols() + j] = nums[i * M.get_cols() + j];
        }
    }
}

int test_reduced_matrix(float* arr, float* sol, int rows, int cols)
{
    Matrix M(rows, cols);
    fill_matrix(M, arr);

    Matrix R = M.rref();

    Matrix Jake(rows, cols);
    fill_matrix(Jake, sol);

    return check_equal(Jake, R);
}

#endif //AUTOTEST_UTILS_H