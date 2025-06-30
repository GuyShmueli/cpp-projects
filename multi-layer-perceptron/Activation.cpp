#include "Activation.h"

Matrix activation::relu(const Matrix& A)
{
    Matrix copy = Matrix(A.get_rows(), A.get_cols());
    for (int i = 0; i < copy.get_cols() * copy.get_rows(); i++)
    {
        if (A[i] > 0)
        {
            copy[i] = A[i];
        }
    }
    return copy;
}

Matrix activation::softmax(const Matrix& A)
{
    Matrix copy = Matrix(A);
    float c = 0.0;
    for (int i = 0; i < copy.get_cols() * copy.get_rows(); i++)
    {
        copy[i] = std::exp(A[i]);
        c += std::exp(A[i]);
    }
    copy = copy * (1/c);
    return copy;
}