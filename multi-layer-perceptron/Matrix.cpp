#include "Matrix.h"
#include <iostream>
#define SQRT 0.5


// Constructor
Matrix::Matrix(int rows, int cols) noexcept(false)
{
    if (rows > 0 && cols > 0)
    {
        this->rows = rows;
        this->cols = cols;
        this->mat = new float*[rows];
        for (int row = 0; row < rows; row++)
        {
            this->mat[row] = new float[cols];
            for (int col = 0; col < cols; col++)
            {
                this->mat[row][col] = 0.0;
            }
        }

    } else
    {
        throw std::runtime_error(DIMENSIONS_EXCEPTION);
    }
}

// Default constructor
Matrix::Matrix() : Matrix(1, 1) {}

// Copy constructor
Matrix::Matrix(const Matrix& m) : rows(m.rows), cols(m.cols)
{
    mat = new float*[rows];
    for (int row = 0; row < rows; row++)
    {
        mat[row] = new float[cols];
        for (int col = 0; col < cols ; col++)
        {
            mat[row][col] = m.mat[row][col];
        }
    }
}

// Destructor
Matrix::~Matrix()
{
    // Delete the allocated array for each row.
    for (int i = 0; i < rows; i++)
    {
        delete[] mat[i];
    }
    // Delete the array of pointers (the actual `mat` pointer).
    delete[] mat;
}


float& Matrix::operator() (int i, int j)
{
    if (i < 0 || j < 0 || i >= this->rows || j >= this->cols)
    {
        throw std::invalid_argument(DIMENSIONS_EXCEPTION);
    }
    return this->mat[i][j];
}

float Matrix::operator() (int i, int j) const
{
    if (i < 0 || j < 0 || i >= this->rows || j >= this->cols)
    {
        throw std::invalid_argument(DIMENSIONS_EXCEPTION);
    }
    return this->mat[i][j];
}

float& Matrix::operator[](int idx) noexcept(false)
{
    int r = idx / cols;
    int c = idx % cols;
    if (idx >= 0 && idx < rows * cols)
    {
        return  *(*(mat+r) + c);
    }

    else
    {
        throw std::out_of_range(RANGE_EXCEPTION);
    }
}

float Matrix::operator[](int idx) const noexcept(false)
{
    int r = idx / cols;
    int c = idx % cols;
    if (idx >= 0 && idx < rows * cols)
    {
        return  *(*(mat+r) + c);
    }

    else
    {
        throw std::out_of_range(RANGE_EXCEPTION);
    }
}

int Matrix::get_rows() const
{
    return this->rows;
}

int Matrix::get_cols() const
{
    return this->cols;
}

float Matrix::sum() const
{
    float sum = 0.0;
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            sum += *(*(mat + i) + j);
        }
    }
    return sum;
}

int Matrix::argmax() const
{
    int argmax = -1;
    float max = mat[0][0];
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if(mat[i][j] > max)
            {
                max = mat[i][j];
                argmax = i * cols + j;
            }
        }
    }
    return argmax;
}

void Matrix::plain_print()
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            std::cout << *(*(mat + i) + j) << " ";
//            std::cout << this->mat[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n";
//        mat[i][j]
}

void Matrix::plain_print() const
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            std::cout << *(*(mat + i) + j) << " ";
//            std::cout << this->mat[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n";
//        mat[i][j]
}


Matrix& Matrix::transpose()
{
    Matrix temp = Matrix(get_cols(), get_rows());
    for (int i = 0; i < temp.get_rows(); i++)
    {
        for (int j = 0; j < temp.get_cols(); j++)
        {
            temp.mat[i][j] = this->mat[j][i];
        }
    }
    *this = temp;
    return *this;
}

Matrix Matrix::dot(const Matrix& B) const
{
    if (!check_valid_dim(B))
    {
        throw std::invalid_argument(DIMENSIONS_MISMATCH);
    }

    Matrix c = Matrix(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            c.mat[i][j] = this->mat[i][j] * B.mat[i][j];
        }
    }
    return c;
}

float Matrix::norm() const
{
    float norm = 0.0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            norm += pow(this->mat[i][j], 2);
        }
    }
    norm = pow(norm, SQRT);
    return norm;
}

Matrix& Matrix::vectorize() noexcept(false)
{
    if (rows == 0 || cols == 0 || mat == nullptr)
    {
        throw std::runtime_error(INIT_EXCEPTION);
    }

    // Calculate total elements and allocate new 1D array
    int total_elements = rows * cols;
    float** new_mat = new float*[total_elements];
    for (int i = 0; i < total_elements; i++)
    {
        new_mat[i] = new float[1];
    }

    // Copy the elements from the 2D array to the 1D array
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            new_mat[i * cols + j][0] = mat[i][j];
        }
    }

    // Deallocate the old 2D array
    for (int i = 0; i < rows; i++)
    {
        delete[] mat[i];
    }
    delete[] mat;

    // Update the class members
    this->mat = new_mat;
    this->rows = total_elements;
    this->cols = 1;

    return *this;
}


Matrix Matrix::operator+ (const Matrix& B) const noexcept(false)
{
    if (!check_valid_dim(B))
    {
        throw std::invalid_argument(DIMENSIONS_MISMATCH);
    }

    Matrix temp = Matrix(this->rows, this->cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            temp.mat[i][j] = this->mat[i][j] + B.mat[i][j];
        }
    }
    return temp;
}

Matrix& Matrix::operator+= (const Matrix& B) noexcept(false)
{
    if (!check_valid_dim(B))
    {
        throw std::invalid_argument(DIMENSIONS_MISMATCH);
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {

            this->mat[i][j] += B.mat[i][j];
        }
    }
    return *this;
}

Matrix Matrix::rref() const
{
    Matrix copied_mat = Matrix(*this);
    int lead = 0;
    for (int r = 0; r < copied_mat.rows; r++)
    {
        if (lead >= copied_mat.cols)
        {
            return copied_mat;
        }
        int i = r;
        while (copied_mat(i, lead) == 0)
        {
            ++i;
            if (i == copied_mat.rows)
            {
                i = r;
                ++lead;
                if (lead == copied_mat.cols)
                {
                    return copied_mat;
                }
            }
        }
        std::swap(copied_mat.mat[i], copied_mat.mat[r]);
        float lv = copied_mat(r, lead);
        for (int j = 0; j < copied_mat.cols; j++)
        {
            copied_mat(r, j) /= lv;
        }
        for (int t = 0; t < copied_mat.rows; t++)
        {
            if (t != r)
            {
                lv = copied_mat(t, lead);
                for (int j = 0; j < copied_mat.cols; ++j)
                {
                    copied_mat(t, j) -= lv * copied_mat(r, j);
                }
            }
        }
        ++lead;
    }
    return copied_mat;
}

// Operator= using copy & swap idiom
Matrix& Matrix::operator=(Matrix A)
{
    swap(A);
    return *this;
}




// Friend methods
Matrix operator*(const Matrix& A, float m)
{
    Matrix temp = Matrix(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++)
    {
        for (int j = 0; j < A.cols; j++)
        {
            temp.mat[i][j] = m * A.mat[i][j];
        }
    }
    return temp;
}


Matrix operator* (float m, const Matrix& A)
{
    return operator*(A, m);
}



Matrix operator* (const Matrix& A, const Matrix& B) noexcept(false)
{
    if (A.get_cols() != B.get_rows())
    {
        throw std::invalid_argument(DIMENSIONS_MISMATCH);
    }

    Matrix c = Matrix(A.get_rows(), B.get_cols());

    for (int i = 0; i < A.get_rows(); i++)
    {
        for (int j = 0; j < B.get_cols(); j++)
        {
            for (int k = 0; k < B.get_rows(); k++)
            {
                c.mat[i][j] += A.mat[i][k] * B.mat[k][j];
            }
        }
    }
    return c;
}

std::ostream& operator<<(std::ostream& os, const Matrix& A)
{
    for (int i = 0; i < A.rows; i++)
    {
        for (int j = 0; j < A.cols; j++)
        {
            if (A(i, j) > THRESHOLD)
            {
                os << "**";
            }
            else
            {
                os << "  ";
            }
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& A) noexcept(false)
{
    is.seekg(0, std::ios_base::end);
    if (!is)
    {
        throw std::runtime_error(SEEK_ERROR);
    }

    long is_len = is.tellg();
    if (is_len == -1)
    {
        throw std::runtime_error(FILE_SIZE_ERROR);
    }

    long a_len = static_cast<long>(A.get_rows() * A.get_cols() * sizeof(float));
    if (is_len < a_len)
    {
        throw std::length_error(FILE_TOO_SHORT);
    }

    is.seekg(0, std::ios_base::beg);
    if (!is)
    {
        throw std::runtime_error(SEEK_ERROR);
    }

    // Read the data into the 2D array
    for (int i = 0; i < A.get_rows(); ++i)
    {
        is.read(reinterpret_cast<char*>(A.mat[i]), A.get_cols() * sizeof(float));
        if (!is)
        {
            throw std::runtime_error(DATA_READ_ERROR);
        }
    }

    return is;
}
