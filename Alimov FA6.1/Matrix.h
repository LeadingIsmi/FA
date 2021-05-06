#ifndef SQUARE_MATRIX
#define SQUARE_MATRIX
#include <iostream>
#include <math.h>
#include "TeX_convertible.h"

typedef struct matrix_indexer
{
    int row_index;
    int column_index;
} matrix_indexer;

class Square_matrix {

private:
    double** data;
    unsigned int size;

public:

    // конструторы
    Square_matrix(); // ноль аргументов
    Square_matrix(unsigned int n); // размер матрицы
    Square_matrix(const Square_matrix& obj); // конструктор копий
    Square_matrix(double** array, int n); // указатель на место памяти с эл матрицы и размер матрицы

    // деструктор
    ~Square_matrix();

    // операторы перегрузки
    bool operator==(const Square_matrix& right_matrix) const;
    bool operator!=(const Square_matrix& right_matrix) const;
    void operator=(const Square_matrix& right_matrix);
    void operator+=(const Square_matrix& right_matrix);
    void operator-=(const Square_matrix& right_matrix);
    void operator*=(const Square_matrix& right_matrix);
    double& operator[](matrix_indexer indexer);

    Square_matrix operator+(const Square_matrix& right_matrix) const;
    Square_matrix operator-(const Square_matrix& right_matrix) const;
    Square_matrix operator*(const Square_matrix& right_matrix) const;
    Square_matrix operator*(double multiplier) const; 

    // функции
    friend Square_matrix operator*(const double multiplier, const Square_matrix& right_matrix);
    friend std::ostream& operator<<(std::ostream& out_stream, const Square_matrix& right_matrix);
    friend std::istream& operator>>(std::istream& in_stream, Square_matrix& right_matrix);
    friend double determinant(const Square_matrix& matrix);
    friend Square_matrix transpose(Square_matrix& matrix);
    friend Square_matrix reversed(const Square_matrix& matrix);
    friend double trace(const Square_matrix& matrix);
    friend Square_matrix exponent(const Square_matrix& matrix, unsigned int n);

    // виртуальная функция
    std::string convert() const;

private:
    void alloc_memory(size_t size);
    void clear_memory();

};

#endif
