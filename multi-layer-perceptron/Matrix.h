#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#define DIMENSIONS_EXCEPTION "Number of rows or columns is invalid"
#define INIT_EXCEPTION "Matrix is empty or not initialized"
#define RANGE_EXCEPTION "Index out of range"
#define DIMENSIONS_MISMATCH "Dimension between the two matrices mismatch"
#define FILE_TOO_SHORT "The file's size is shorter than the matrix's size"
#define SEEK_ERROR "Failed to seek to beginning/end of the stream"
#define FILE_SIZE_ERROR "Failed to determine the size of the stream"
#define DATA_READ_ERROR "Failed to read the required amount of data"
#define THRESHOLD 0.1

struct matrix_dims {
    int rows, cols;
};


class Matrix
{
private:
    int rows;
    int cols;
    float** mat = nullptr;

    bool check_valid_dim (const Matrix& B) const
    {
        return this->rows == B.rows && this->cols == B.cols;
    }

    // Swap function
    void swap(Matrix& A)
    {
        using std::swap;
        swap(this->rows, A.rows);
        swap(this->cols, A.cols);
        swap(this->mat, A.mat);
    }

public:
    // friend methods (used when first parameter isn't this)
    friend Matrix operator*(const Matrix& A, float m);
    friend Matrix operator*(float m, const Matrix& A);
    friend Matrix operator*(const Matrix& A, const Matrix& B) noexcept(false);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& A);
    friend std::istream& operator>>(std::istream& is, Matrix& A)
    noexcept(false);
    /**
     * @brief Constructor that initializes a matrix with specified
     * rows and columns.
     * @param rows Number of rows in the matrix.
     * @param cols Number of columns in the matrix.
     * @exception std::runtime_error Thrown if the provided dimensions
     * are non-positive.
     */
    Matrix(int rows, int cols) noexcept(false);

    /**
     * @brief Default constructor that initializes a 1x1 matrix.
     */
    Matrix();

    /**
     * @brief Copy constructor that creates a deep copy of another matrix.
     * @param m A reference to another Matrix object to be copied.
     */
    Matrix(const Matrix& m);

    /**
     * @brief Destructor that deallocates memory used by the matrix.
     */
    ~Matrix();

    /**
     * @brief Accesses a specific element of the matrix in a
     * mutable fashion.
     * @param i Row index.
     * @param j Column index.
     * @return Reference to the matrix element at the specified index.
     */
    float& operator()(int i, int j);

    /**
     * @brief Accesses a specific element of the matrix in an
     * immutable fashion.
     * @param i Row index.
     * @param j Column index.
     * @return Const reference to the matrix element at the specified index.
     */
    float operator() (int i, int j) const;

    /**
     * @brief Accesses a specific element of the matrix based on a linear
     * index in a mutable fashion.
     * @param idx Linear index calculated by row-major order.
     * @return Reference to the matrix element at the specified index.
     * @exception std::out_of_range Thrown if the index is out of bounds.
     */
    float& operator[](int idx) noexcept(false);

    /**
     * @brief Accesses a specific element of the matrix based on a linear
     * index in an immutable fashion.
     * @param idx Linear index calculated by row-major order.
     * @return Const reference to the matrix element at the specified index.
     * @exception std::out_of_range Thrown if the index is out of bounds.
     */
    float operator[](int idx) const noexcept(false);

    /**
     * @brief Retrieves the number of rows in the matrix.
     * @return Number of rows.
     */
    int get_rows() const;

    /**
    * @brief Retrieves the number of columns in the matrix.
    * @return Number of columns.
     */
    int get_cols() const;

    /**
     * @brief Computes the sum of all elements in the matrix.
     * @return Sum of the matrix elements.
     */
    float sum() const;

    /**
     * @brief Finds the index of the maximum element in the matrix.
     * @return Index of the maximum element.
     */
    int argmax() const;

    /**
     * @brief Prints matrix elements to the standard output in a plain format.
     */
    void plain_print();

    void plain_print() const;

    /**
     * @brief Transposes the matrix in-place.
     * @return Reference to the current matrix after transposition.
     */
    Matrix& transpose();


    /**
   * @brief Performs element-wise multiplication of this
    * matrix with another matrix.
   * @param B Another matrix to multiply with.
   * @return A new matrix resulting from the element-wise multiplication.
   */
    Matrix dot(const Matrix& B) const;


    /**
     * @brief Computes the Frobenius norm of the matrix.
     * @return Frobenius norm of the matrix.
     */
    float norm() const;


    /**
     * @brief Reshapes the matrix into a 1D column vector.
     * @return Reference to the current matrix after vectorization.
     */
    Matrix& vectorize() noexcept(false);

    /**
     * @brief Adds another matrix to this matrix.
     * @param B The matrix to be added to this one.
     * @return A new matrix resulting from the addition.
     */
    Matrix operator+ (const Matrix& B) const noexcept(false);


    /**
     * @brief Adds another matrix to this matrix in-place.
     * @param B The matrix to be added to this one.
     * @return Reference to the current matrix after the addition.
     */
    Matrix& operator+= (const Matrix& B) noexcept(false);


    /**
     * @brief Solves the matrix to Reduced Row Echelon Form (RREF).
     * @return A new matrix in RREF.
     */
    Matrix rref() const;


    /**
     * @brief Assignment operator using the copy & swap idiom.
     * @param A Matrix to be assigned to this one.
     * @return Reference to the current matrix after assignment.
     */
    Matrix& operator=(Matrix A);

};


#endif //MATRIX_H
