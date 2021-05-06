#include "Parsing.h"
#include "Matrix.h"
#include <string>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

void parsing(ifstream& input_file, ofstream& out_file)
{
    string buffer;
    int size_matr = 0;
    int first_size_matr = 0;
    int second_size_matr = 0;
    vector<string> Vect_of_strings;
    int key1, key2;
    double num_to_multip = 0;
    double** array = nullptr;
    double** first_array = nullptr;
    double** second_array = nullptr;
    string operation;
    bool equality = true;
    double foo_res = 0;
    int flag_ = -1;

    out_file << "\\documentclass{article}" << endl;
    out_file << "\\usepackage[utf8]{inputenc}" << endl;
    out_file << "\\usepackage[utf8]{amsmath}" << endl;
    out_file << "\\begin{document}" << endl;
    out_file << "\\Large" << endl;

    while (getline(input_file, buffer))
    {
        split_to_vector(Vect_of_strings, " ", buffer);

        // если это действие с двумя матрицами или одной матрицей и числом
        if (isdigit(Vect_of_strings[0][0]))
        {
            key1 = string_to_int(Vect_of_strings[0]);
            key2 = string_to_int(Vect_of_strings[1]);

            // если это действие с одной матрицей и числом
            if (key1 == 0 || key2 == 0)
            {
                // число * матрицу
                if (key1 == 0)
                {
                    size_matr = string_to_int(Vect_of_strings[1]);
                    num_to_multip = string_to_double(Vect_of_strings[2]);
                    alloc_array(size_matr, Vect_of_strings, array, 3);

                    Square_matrix M(array, size_matr);
                    Square_matrix M_result(size_matr);

                    out_file << num_to_multip << " * " << endl;
                    out_file << M.convert();
                    M_result = num_to_multip * M;

                    out_file << "=" << endl;
                    out_file << M_result.convert() << endl;
                }
                // матрица * число
                else
                {
                    size_matr = string_to_int(Vect_of_strings[0]);
                    num_to_multip = string_to_double(Vect_of_strings[1 + size_matr * size_matr + 1]);
                    alloc_array(size_matr, Vect_of_strings, array, 2);

                    Square_matrix M(array, size_matr);
                    Square_matrix M_result(size_matr);

                    out_file << M.convert();
                    out_file << "*" << endl;
                    out_file << num_to_multip << endl;
                    M_result = M * num_to_multip;
                    out_file << "=" << endl;
                    out_file << M_result.convert() << endl;
                }
                clear_array(size_matr, array);
            }
            // если это действие с двумя матрицами
            else
            {
                first_size_matr = string_to_int(Vect_of_strings[0]);
                second_size_matr = string_to_int(Vect_of_strings[1]);

                operation = Vect_of_strings[1 + pow(first_size_matr, 2) + pow(second_size_matr, 2) + 1];
                alloc_array(first_size_matr, Vect_of_strings, first_array, 2);
                alloc_array(second_size_matr, Vect_of_strings, second_array, 1 + pow(first_size_matr, 2) + 1);

                Square_matrix M1(first_array, first_size_matr);
                Square_matrix M1_copy(M1);
                Square_matrix M2(second_array, second_size_matr);
                Square_matrix M2_copy(M2);
                Square_matrix M_result(first_size_matr);

                try
                {
                    if (operation == "+")
                    {
                        M_result = M1 + M2;
                    }
                    else if (operation == "-")
                    {
                        M_result = M1 - M2;
                    }
                    else if (operation == "*")
                    {
                        M_result = M1 * M2;
                    }
                    else if (operation == "+=")
                    {
                        M1 += M2;
                        M_result = M1;
                    }
                    else if (operation == "-=")
                    {
                        M1 -= M2;
                        M_result = M1;
                    }
                    else if (operation == "*=")
                    {
                        M1 *= M2;
                        M_result = M1;
                    }
                    else if (operation == "==")
                    {
                        if (M1 == M2)
                            equality = true;
                        else
                            equality = false;
                    }
                    else if (operation == "!=")
                    {
                        if (M1 != M2)
                            equality = true;
                        else
                            equality = false;
                    }
                }
                catch (const std::exception& exception)
                {
                    std::cerr << exception.what() << std::endl;
                    continue;
                }
                out_file << M1_copy.convert();
                out_file << operation << endl;
                out_file << M2_copy.convert();

                if (operation != "==" && operation != "!=")
                {
                    out_file << "=" << endl;
                    out_file << M_result.convert() << endl;
                }
                else
                {
                    out_file << ((equality) ? "True\n" : "False\n") << endl;
                }

                // чистка памяти для массивов
                if (operation == "+" || operation == "-" || operation == "*" ||
                    operation == "+=" || operation == "-=" || operation == "*=")
                {
                    clear_array(first_size_matr, first_array);
                    clear_array(second_size_matr, second_array);
                }
            }
        }
        // если это действие с одной матрицей и функцией
        else
        {
            operation = Vect_of_strings[0];
            size_matr = string_to_int(Vect_of_strings[1]);
            alloc_array(size_matr, Vect_of_strings, array, 2);
            Square_matrix M1(array, size_matr);
            Square_matrix M1_copy(array, size_matr);
            Square_matrix M_result_foo(size_matr);

            try {
                if (operation == "determinant")
                {
                    foo_res = determinant(M1);
                    flag_ = 1;
                }
                else if (operation == "trace")
                {
                    foo_res = trace(M1);
                    flag_ = 1;
                }
                else if (operation == "transpose")
                {
                    M_result_foo = transpose(M1);
                    flag_ = 0;
                }
                else if (operation == "exponent")
                {
                    M_result_foo = exponent(M1, size_matr);
                    flag_ = 0;
                }
                else if (operation == "inverse")
                {
                    M_result_foo = reversed(M1);
                    flag_ = 0;
                }
            }
            catch (const std::exception& exception)
            {
                std::cerr << exception.what() << std::endl;
                continue;
            }

            out_file << operation << endl;
            out_file << M1_copy.convert();
            out_file << " = ";

            if (flag_ == 0)
            {
                out_file << M_result_foo.convert() << endl;
            }
            else
            {
                out_file << foo_res << " \\\\ " << endl << endl;
            }

            clear_array(size_matr, array);
        }

    }

    out_file << "\\end{document}" << endl;
    clear_mem_vector(Vect_of_strings);
    input_file.close();
}

void split_to_vector(vector<string>& Vect, string separator, const string& buffer)
{
    size_t p;
    size_t start = 0;
    size_t len = buffer.length();

    Vect.clear();
    start = buffer.find_first_not_of(separator);
    p = buffer.find_first_of(separator, start);

    while (p != std::string::npos) // -1
    {
        Vect.push_back(buffer.substr(start, p - start));
        start = buffer.find_first_not_of(separator, p);
        p = buffer.find_first_of(separator, start);
    }

    if (len > start)
    {
        Vect.push_back(buffer.substr(start, len - start));
    }
}

void clear_mem_vector(vector<string>& Vect)
{
    Vect.clear();
}

int string_to_int(string num_str)
{
    int num = 0;
    for (int i = 0; i < num_str.length(); i++)
    {
        num = num * 10 + num_str[i] - '0';
    }
    return num;
}

double string_to_double(string num_double)
{
    return atof(num_double.c_str());
}

void alloc_array(int size, vector<string> Vect_of_strings, double**& array, int place)
{
    array = new double* [size];
    for (int i = 0; i < size; i++)
    {
        array[i] = new double[size];
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array[i][j] = string_to_double(Vect_of_strings[place]);
            place++;
        }
    }
}

void clear_array(int size, double**& array)
{
    for (int i = 0; i < size; i++)
    {
        delete[] array[i];
    }
    delete[] array;
}