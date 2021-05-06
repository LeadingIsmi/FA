#include "Matrix.h"
#include <iostream>
#include <sstream>
#include <algorithm>

#define epsilon 1e-6

using namespace std;

// выделение памяти для матрицы
void Square_matrix::alloc_memory(size_t n)
{
    data = new double* [n];
    for (int i = 0; i < n; i++)
        data[i] = new double[n];
}

// очистка памяти
void Square_matrix::clear_memory()
{
    if (data != nullptr) {
        for (int i = 0; i < size; i++)
        {
            delete data[i];
        }
        delete[] data;
    }
}

// конструкторы
Square_matrix::Square_matrix()
{
    size = 0;
    data = nullptr;
}

Square_matrix::Square_matrix(unsigned int n)
{
    if (n < 0)
    {
        this->size = 0;
        this->data = nullptr;
    }
    else
    {
        this->size = n;
        alloc_memory(n);
    }
}

Square_matrix::Square_matrix(const Square_matrix& obj)
{
    int i, j;
    this->size = obj.size;
    alloc_memory(size);
    if (this->data != nullptr && obj.data != nullptr)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                data[i][j] = obj.data[i][j];
            }
        }
    }
}

Square_matrix::Square_matrix(double** array, int n)
{
    data = array;
    if (data == nullptr)
    {
        size = 0;
        data = nullptr;
        return;
    }
    int i, j;
    this->size = n;
    alloc_memory(size);
    if (array != nullptr)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                data[i][j] = array[i][j];
            }
        }
    }
}

// деструктор
Square_matrix::~Square_matrix()
{
    clear_memory();
}

// операторы перегрузки

void Square_matrix::operator=(const Square_matrix& right_matrix)
{
    clear_memory();

    this->size = right_matrix.size;
    alloc_memory(size);
    if (data != nullptr)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                data[i][j] = right_matrix.data[i][j];
            }
        }
    }
}

bool Square_matrix::operator==(const Square_matrix& right_matrix) const
{

    if (this->size != right_matrix.size)
        return (false);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (right_matrix.data[i][j] < this->data[i][j] - epsilon ||
                right_matrix.data[i][j] > this->data[i][j] + epsilon)
                return (false);
    return (true);
}

bool Square_matrix::operator!=(const Square_matrix& right_matrix) const 
{
    if (*this == right_matrix)
        return false;
    return true;
}

void Square_matrix::operator+=(const Square_matrix& right_matrix)
{
    if (this->size != right_matrix.size)
        throw runtime_error("The sizes of matrixes are different");
    else {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                data[i][j] += right_matrix.data[i][j];
            }
        }
    }
}

void Square_matrix::operator-=(const Square_matrix& right_matrix)
{
    if (this->size != right_matrix.size)
        throw runtime_error("The sizes of matrixes are different");
    else {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                data[i][j] -= right_matrix.data[i][j];
            }
        }
    }
}

void Square_matrix::operator*=(const Square_matrix& right_matrix)
{
    Square_matrix tmp_matrix(*this);

    if (this->size != right_matrix.size)
        throw runtime_error("The sizes of matrixes are different");
    else
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                tmp_matrix.data[i][j] = 0;
                for (int k = 0; k < size; k++)
                {
                    tmp_matrix.data[i][j] += data[i][k] * right_matrix.data[k][j];
                }
            }
        }
        *this = tmp_matrix;
    }
}

double& Square_matrix::operator[](matrix_indexer indexer)
{
    if (indexer.column_index >= size || indexer.row_index >= size)
        throw runtime_error("The sizes of matrixes are different");

    return data[indexer.row_index][indexer.column_index];
}

Square_matrix Square_matrix::operator+(const Square_matrix& right_matrix) const
{
    Square_matrix tmp_matrix(*this);
    tmp_matrix += right_matrix;

    return tmp_matrix;
}

Square_matrix Square_matrix::operator-(const Square_matrix& right_matrix) const
{
    Square_matrix tmp_matrix(*this);
    tmp_matrix -= right_matrix;

    return tmp_matrix;
}

Square_matrix Square_matrix::operator*(const Square_matrix& right_matrix) const
{
    Square_matrix tmp_matrix(*this);
    tmp_matrix *= right_matrix;

    return tmp_matrix;
}

Square_matrix Square_matrix::operator*(double multiplier) const
{
    if (this->data != nullptr)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                data[i][j] = data[i][j] * multiplier;
            }
        }
    }
    return (*this);

}

// функции
ostream& operator<<(ostream& out_stream, const Square_matrix& right_matrix)
{
    for (int i = 0; i < right_matrix.size; i++)
    {
        for (int j = 0; j < right_matrix.size; j++)
        {
            out_stream << right_matrix.data[i][j] << ' ';
        }
        out_stream << endl;
    }

    return out_stream;
}

istream& operator>>(istream& in_stream, Square_matrix& obj)
{
    for (int i = 0; i < obj.size; i++)
    {
        for (int j = 0; j < obj.size; j++)
        {
            in_stream >> obj.data[i][j];
        }
    }
    return in_stream;
}

Square_matrix operator*(const double multiplier, const Square_matrix& matrix)
{
    return matrix * multiplier;
}

// виртуальная функция
std::string Square_matrix::convert() const
{
    std::stringstream string;
    string << "$\\begin{pmatrix}" << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            string << data[i][j];
            if (j != size - 1)
            {
                string << " & ";
            }
            else
            {
                string << "\\\\\n";
            }
        }
    }
    string << "\\end{pmatrix}$" << endl;
    return string.str(); 
}

// гаусс
double determinant(const Square_matrix& obj)
{
    Square_matrix tmp_matrix(obj);
    double det = 1;
    double mtp;
    double key_elem;

    for (int i = 0; i < tmp_matrix.size; i++)
    {
        key_elem = tmp_matrix.data[i][i];
        int j;
        if (!key_elem)
        {
            j = i + 1;
            while (!key_elem && j < tmp_matrix.size)
            {
                key_elem = tmp_matrix.data[j][i];
                j++;
            }
            j--;
            if (!key_elem)
                return 0;
            else
            {
                for (int k = i; k < tmp_matrix.size; k++)
                {
                    tmp_matrix.data[i][k] += tmp_matrix.data[j][k];
                }
            }
        }

        for (j = i + 1; j < tmp_matrix.size; j++)
        {
            mtp = tmp_matrix.data[j][i] / key_elem;
            tmp_matrix.data[j][i] = 0;
            for (int k = i + 1; k < tmp_matrix.size; k++)
            {
                tmp_matrix.data[j][k] = tmp_matrix.data[j][k] - tmp_matrix.data[i][k] * mtp;
            }
        }
        det *= key_elem;
    }
    return det;
}

Square_matrix transpose(Square_matrix& Matrix)
{
    double tmp;
    for (int i = 0; i < Matrix.size; i++)
    {
        for (int j = 0; j < Matrix.size; j++)
        {
            if (i < j) {
                tmp = Matrix.data[j][i];
                Matrix.data[j][i] = Matrix.data[i][j];
                Matrix.data[i][j] = tmp;
            }
        }
    }
    return Matrix;
}

double minor(double** matr, unsigned int size, unsigned int col, unsigned int row)
{
    double** new_matrix = nullptr;
    int new_row = 0, new_col = 0;

    new_matrix = new double* [size];
    for (int i = 0; i < size; i++)
        new_matrix[i] = new double[size];

    if (new_matrix != nullptr)
    {
        for (int i = 0; i < size; i++)
        {
            if (i != row)
            {
                for (int j = 0; j < size; j++)
                {
                    if (j != col)
                    {
                        new_matrix[new_row][new_col] = matr[i][j];
                        new_col++;
                    }
                }
                new_col = 0;
                new_row++;
            }
            if (new_row > size - 2)
                new_row = 0;
        }
    }
    Square_matrix temp_matr(new_matrix, size - 1);
    for (int i = 0; i < size - 1; i++)
        delete new_matrix[i];
    delete[] new_matrix;
    return (determinant(temp_matr));
}

Square_matrix reversed(const Square_matrix& Matrix)
{
    Square_matrix tmp_matrix(Matrix.size);
    double det = determinant(Matrix);

    if (determinant(Matrix) == 0) {
        throw runtime_error("Division by zero");
    }

    for (int i = 0; i < tmp_matrix.size; i++)
    {
        for (int j = 0; j < tmp_matrix.size; j++)
        {
            tmp_matrix.data[i][j] = pow(-1, i + j) * minor(Matrix.data, Matrix.size, i, j);
        }
    }
    tmp_matrix = tmp_matrix * (1 / determinant(Matrix));
    return tmp_matrix;
}

double trace(const Square_matrix& matrix)
{
    double sum = 0;
    for (int i = 0; i < matrix.size; i++)
    {
        sum += matrix.data[i][i];
    }
    return sum;
}

int factorial(int n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

Square_matrix exponent(const Square_matrix& Matrix, unsigned int n)
{
    Square_matrix identity_matrix(Matrix.size); 

    for (int i = 0; i < Matrix.size; i++)
    {
        for (int j = 0; j < Matrix.size; j++)
        {
            if (i == j)
            {
                identity_matrix.data[i][j] = 1;
            }
            else
                identity_matrix.data[i][j] = 0;
        }
    }
    Square_matrix exp_matrix(Matrix.size);
    Square_matrix tmp_matrix(Matrix);
    for (int i = 0; i < n; i++)
    {
        exp_matrix += (1 / factorial(i)) * tmp_matrix;
        tmp_matrix *= Matrix;
    }
    return (exp_matrix + identity_matrix);
}