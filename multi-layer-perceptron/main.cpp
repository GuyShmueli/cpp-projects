/**
 * There are 2 modes in this program:
 * 1. Automated-testing:  runs its self-checks automatically rather
 * than waiting for human input.
 * 2. Manual-testing: loads the network weights/biases, prompts you for an image file,
 * runs the MLP, and prints the predicted digit & probability.
 */
// main.cpp - toggle between CLI and automated-tests at build-time
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Matrix.h"
#include "MlpNetwork.h"
#include "autotest_utils.h"

#define RUN_CLI            // comment this line to compile the automated-test build

// --- global constants ---
const int IMG_ROWS = 28;   // MNIST image size
const int IMG_COLS = 28;

// helper: read binary into Matrix
bool readFileToMatrix(const std::string& path, Matrix& dst)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) { return false; }

    for (int r = 0; r < dst.get_rows(); ++r)
    {
        in.read(reinterpret_cast<char*>(&dst(r, 0)),
                dst.get_cols() * sizeof(float));
    }
    return in.good();
}

// CLI MODE
int run_cli(int argc, char** argv)
{
    if (argc != 1 + MLP_SIZE * 2)
    {
        std::cerr << "Usage: ./mlp w1 w2 w3 w4 b1 b2 b3 b4\n";
        return EXIT_FAILURE;
    }

    Matrix weights[MLP_SIZE];
    Matrix biases [MLP_SIZE];

    try
    {
        for (int i = 0; i < MLP_SIZE; ++i)
        {
            weights[i] = Matrix(weights_dims[i].rows, weights_dims[i].cols);
            biases [i] = Matrix(bias_dims[i].rows,  bias_dims[i].cols);

            if (!readFileToMatrix(argv[1 + i],            weights[i]) ||
                !readFileToMatrix(argv[1 + MLP_SIZE + i], biases [i]))
            {
                throw std::runtime_error("Failed reading layer "
                                         + std::to_string(i + 1));
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    MlpNetwork mlp(weights, biases);

    std::string imgPath;
    constexpr char QUIT_CMD[] = "q";
    std::cout << "Enter image path (or '" << QUIT_CMD << "' to quit): ";

    while (std::cin >> imgPath && imgPath != QUIT_CMD)
    {
        Matrix img(IMG_ROWS, IMG_COLS);

        if (!readFileToMatrix(imgPath, img))
        {
            std::cerr << "Error: cannot open '" << imgPath << "'\n";
        }
        else
        {
            digit res = mlp(img.vectorize());
            std::cout << "Prediction: " << res.value
                      << "  (p = " << res.probability << ")\n";
        }
        std::cout << "\nEnter next image path (or '" << QUIT_CMD << "' to quit): ";
    }
    return EXIT_SUCCESS;
}



// TEST MODE
/*  Uses helpers from autotest_utils.h:
 *    - get_ordered_matrix()
 *    - test_reduced_matrix()
 */
int test_transpose()
{
    Matrix A = get_ordered_matrix(3, 5);
    Matrix B = A;
    B.transpose();

    if (B.get_rows() != 5 || B.get_cols() != 3)
        return 1;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 5; ++j)
            if (!float_compare(A(i, j), B(j, i)))
                return 2;
    return 0;
}

int test_matrix_read()
{
    Matrix M(2, 3);
    float data[] = {1, 2, 3, 4, 5, 6};
    std::stringstream ss;
    ss.write(reinterpret_cast<char*>(data), sizeof(data));

    try { ss >> M; }
    catch (...) { return 1; }

    for (int i = 0; i < 6; ++i)
        if (!float_compare(M[i], data[i]))
            return 2;
    return 0;
}

int test_rref_simple()
{
    // 2×3 example: [[1,2,3],[4,5,6]]  → reduced row-echelon form?
    float arr[] = {1,2,3, 4,5,6};
    float sol[] = {1,0,-1, 0,1,2};
    return test_reduced_matrix(arr, sol, 2, 3);            // 0 on success
}

int run_unit_tests()
{
    int rc = test_transpose();
    if (rc) { std::cerr << "Transpose test failed\n"; return rc; }

    rc = test_matrix_read();
    if (rc) { std::cerr << "Matrix-read test failed\n"; return rc; }

    rc = test_rref_simple();
    if (rc) { std::cerr << "RREF test failed\n";       return rc; }

    std::cout << "All tests passed\n";
    return EXIT_SUCCESS;
}


// entry point selector
int main(int argc, char** argv)
{
    #ifdef RUN_CLI
        return run_cli(argc, argv);
    #else
        return run_unit_tests();
    #endif
}
