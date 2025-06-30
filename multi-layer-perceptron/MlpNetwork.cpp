#include "MlpNetwork.h"
#define SOFTMAX_VEC_LEN 10

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64, 128},
                                    {20, 64},
                                    {10, 20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64, 1},
                                 {20, 1},
                                 {10, 1}};

MlpNetwork::MlpNetwork(Matrix weights[], Matrix biases[]) :
        first_layer(Dense(weights[0], biases[0], activation::relu)),
        second_layer(Dense(weights[1], biases[1], activation::relu)),
        third_layer(Dense(weights[2], biases[2], activation::relu)),
        fourth_layer(Dense(weights[3], biases[3], activation::softmax))
{}


digit MlpNetwork::operator()(const Matrix &img) const
{
    Matrix softmax_vec = first_layer(img);
    softmax_vec = second_layer(softmax_vec);
    softmax_vec = third_layer(softmax_vec);
    softmax_vec = fourth_layer(softmax_vec);

    unsigned int value = 0;
    float probability = 0.0;

    for (int i = 0; i < SOFTMAX_VEC_LEN; i++)
    {
        if (softmax_vec[i] > probability)
        {
            value = i;
            probability = softmax_vec[i];
        }
    }

    return digit{value, probability};
}