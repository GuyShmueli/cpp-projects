#include "Dense.h"

Dense::Dense(const Matrix& W, const Matrix& b, ActivationType af)  :
weights(W), bias(b), activation_func(af) {}

Matrix Dense::get_weights() const
{
    return this->weights;
}

Matrix Dense::get_bias() const
{
    return this->bias;
}

ActivationType Dense::get_activation() const
{
    return this->activation_func;
}

Matrix Dense::operator() (const Matrix& A) const
{
    Matrix to_be_activated = this->weights * A + this->bias;
    return this->activation_func(to_be_activated);
}

