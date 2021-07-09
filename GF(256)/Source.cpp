#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <fstream>
#include <limits>

using namespace std;

#define MAX_DISH 16
#define IRRED_POLY 283
#define REVERSE_POW 254

typedef uint8_t byte;

class Galua_Field_Polynomes
{
private:
    vector<uint16_t> irred_polynomes;

public:

    // основные методы
    byte add(byte poly1, byte poly2) const;
    byte multiply(byte poly1, byte poly2, uint16_t modulo) const;
    byte inverse(byte poly, uint16_t modulo) const;

    // дополнительные методы
    byte get_remainder(uint16_t poly, uint16_t modulo) const;
    int get_degree(uint16_t poly) const;
    bool odd_terms_in_polynomial(uint16_t poly) const;
    int print_irred_polynomes(vector<uint16_t> vect) const;
    int push_irred_polynomes_to_vector();
    vector<uint16_t> get_irred_polynomes() const;
};

byte Galua_Field_Polynomes::add(byte poly1, byte poly2) const
{
    return poly1 ^ poly2;
}

int Galua_Field_Polynomes::get_degree(uint16_t poly) const
{
    int degree = 0;
    uint16_t bit = 1;
    for (int i = 0; i < MAX_DISH; i++)
    {
        if (poly & bit)
        {
            degree = i;
        }
        bit <<= 1;
    }
    return degree;
}

byte Galua_Field_Polynomes::get_remainder(uint16_t poly, uint16_t modulo) const
{
    int degree_modulo;
    int degree_poly;
    int difference;
    uint16_t tmp_poly = poly;

    degree_modulo = get_degree(modulo);

    while ((difference = (degree_poly = get_degree(tmp_poly)) - degree_modulo) >= 0)
    {
        tmp_poly ^= (modulo << difference);
    }
    return tmp_poly;
}

byte Galua_Field_Polynomes::multiply(byte poly1, byte poly2, uint16_t modulo) const
{
    uint16_t result = 0;
    byte tmp_poly2 = poly2;

    for (int i = 0; i < 8; i++)
    {
        result ^= poly1 * (tmp_poly2 & (1 << i));
    }
    result = get_remainder(result, modulo);
    return result;
}

byte Galua_Field_Polynomes::inverse(byte poly, uint16_t modulo) const
{
    int degree_pow;
    byte bit;
    byte tmp_poly;

    tmp_poly = poly;
    degree_pow = get_degree(REVERSE_POW);
    bit = REVERSE_POW;

    for (int i = degree_pow; i >= 0; i--)
    {
        if (modulo & bit)
        {

            poly = multiply(poly, tmp_poly, modulo);
        }
        poly = multiply(poly, poly, modulo);
        bit >>= 1;
    }
    return poly;
}

// проверка на нулевое или нечетное количество одночленов
bool Galua_Field_Polynomes::odd_terms_in_polynomial(uint16_t poly) const
{
    int counter_one = 0;
    uint16_t bit = 1;
    int degree_poly = get_degree(poly);

    for (int i = 0; i <= degree_poly; i++)
    {
        if (bit & poly)
        {
            counter_one++;
        }
        bit <<= 1;
    }

    if ((counter_one % 2) != 0 && counter_one != 1)
        return true;
    else
        return false;
}

// получение 30 неприводимых многочленов
int Galua_Field_Polynomes::push_irred_polynomes_to_vector()
{
    int non_zero_remainders = 0;
    uint16_t min_poly_with_degree_8 = 256;
    uint16_t max_poly_with_degree_8 = 511;
    uint16_t j;

    for (uint16_t i = min_poly_with_degree_8; i <= max_poly_with_degree_8; i++)
    {
        if (odd_terms_in_polynomial(i))
        {
            for (j = 2; j < 32; j++)
            {
                if (get_remainder(i, j) == 0)
                {
                    non_zero_remainders = 0;
                    break;
                }
                else
                    non_zero_remainders++;
            }

            if (non_zero_remainders == 30)
            {
                irred_polynomes.push_back(i);
                non_zero_remainders = 0;
            }
        }
    }
    if (print_irred_polynomes(irred_polynomes) == 0)
        return 0;
    else
        return 1;
}

vector<uint16_t> Galua_Field_Polynomes::get_irred_polynomes() const
{
    return irred_polynomes;
}

int Galua_Field_Polynomes::print_irred_polynomes(vector<uint16_t> Vect) const
{
    ofstream file;
    uint16_t bit;
    vector<uint16_t>::const_iterator it;

    file.open("file.txt");

    if (!file.is_open())
        return 0;
    else
    {
        file << "30 irreducible polynomials:" << endl;
        for (it = Vect.cbegin(); it != Vect.cend(); it++)
        {
            bit = 256;
            file << *it << " = ";

            for (uint16_t k = get_degree(*it); k > 0; k--)
            {
                if (bit & (*it))
                {
                    file << "x^" << (unsigned)k << " + ";
                }
                bit >>= 1;
            }
            file << "1" << endl;
        }
        file.close();
        return 1;
    }
}

int main()
{
    Galua_Field_Polynomes GF_poly;

    byte k = 3;
    byte m = 7;

    cout << (unsigned)GF_poly.add(k, m) << endl;
    cout << (unsigned)GF_poly.multiply(k, m, IRRED_POLY) << endl;
    cout << (unsigned)GF_poly.inverse(k, IRRED_POLY) << endl;

    if (GF_poly.push_irred_polynomes_to_vector() == 0)
        cout << "Сan't open file" << endl;

    return 0;
}