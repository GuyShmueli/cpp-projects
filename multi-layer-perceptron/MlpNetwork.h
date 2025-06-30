#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"
#define MLP_SIZE 4

typedef struct digit
{
    unsigned int value;
    float probability;
} digit;

extern const matrix_dims img_dims;
extern const matrix_dims weights_dims[MLP_SIZE];
extern const matrix_dims bias_dims[MLP_SIZE];

class MlpNetwork
{
private:
    Dense first_layer;
    Dense second_layer;
    Dense third_layer;
    Dense fourth_layer;

public:
    MlpNetwork(Matrix weights[], Matrix biases[]);

    digit operator() (const Matrix& img);

    digit operator() (const Matrix& img) const;
};

#endif //MLPNETWORK_H
