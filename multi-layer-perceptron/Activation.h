
#ifndef ACTIVATION_H
#define ACTIVATION_H
#include "Matrix.h"

namespace activation
{
    Matrix relu(const Matrix& A);

    Matrix softmax(const Matrix& A);
}

#endif //ACTIVATION_H
