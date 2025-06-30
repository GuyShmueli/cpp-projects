#ifndef DENSE_H
#define DENSE_H
#include "Matrix.h"
#include "Activation.h"

typedef Matrix (*ActivationType) (const Matrix& A);

class Dense {
private:
    Matrix weights;
    Matrix bias;
    ActivationType activation_func;

public:
    // Constructor
    Dense(const Matrix &W, const Matrix &b, ActivationType af);

    // Getter for the weights
    Matrix get_weights() const;

    // Getter for the bias
    Matrix get_bias() const;

    // Getter for the activation
    ActivationType get_activation() const;

    // Applying dense layer
    Matrix operator() (const Matrix& A) const;

};

#endif //DENSE_H
