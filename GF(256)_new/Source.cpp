#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <fstream>
#include <limits>

using namespace std;

#define MAX_DISH 16
#define IRRED_POLY 283

class Galois_field
{
private:
    vector<uint16_t> irred_polynomes;

public:

    uint8_t add(uint8_t poly1, uint8_t poly2) const;
    uint8_t multiply(uint8_t poly1, uint8_t poly2, uint16_t mod_to_mult) const;
    uint8_t inverse(uint8_t poly, uint16_t mod_to_mult) const;
    void print(uint8_t poly) const;
    uint8_t pow(uint8_t poly, int _pow, uint16_t mod_to_mult) const;
    uint8_t mod(uint16_t, uint16_t) const;

    int get_degree(uint16_t poly) const;

    vector<uint16_t> get_irred_polynomes() const;
};

uint8_t Galois_field::add(uint8_t poly1, uint8_t poly2) const
{
    return poly1 ^ poly2;
}

int Galois_field::get_degree(uint16_t poly) const
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

uint8_t Galois_field::mod(uint16_t poly, uint16_t mod_to_mult) const
{
    int degree_mod_to_mult;
    int degree_poly;
    int difference;
    uint16_t tmp_poly = poly;

    degree_mod_to_mult = get_degree(mod_to_mult);

    while ((difference = (degree_poly = get_degree(tmp_poly)) - degree_mod_to_mult) >= 0)
    {
        tmp_poly = add(tmp_poly, mod_to_mult << difference);
    }
    return tmp_poly;
}

uint8_t Galois_field::multiply(uint8_t poly1, uint8_t poly2, uint16_t mod_to_mult) const
{
    uint16_t result = 0;
    uint8_t tmp_poly2 = poly2;

    for (int i = 0; i < 8; i++)
    {
        result ^= poly1 * (tmp_poly2 & (1 << i));
    }
    return mod(result, mod_to_mult);
}

uint8_t Galois_field::pow(uint8_t poly1, int _pow, uint16_t mod_to_mult) const
{
    if (_pow <= 0)
        return (1);
    if (_pow % 2)
        return (multiply(poly1, pow(poly1, _pow - 1, mod_to_mult), mod_to_mult));
    return (pow(multiply(poly1, poly1, mod_to_mult), _pow / 2, mod_to_mult));
}

uint8_t Galois_field::inverse(uint8_t poly, uint16_t mod_to_mult) const
{
    return (pow(poly, 254, mod_to_mult));
}

vector<uint16_t> Galois_field::get_irred_polynomes() const
{
    std::vector <uint16_t>  ans;
    int non_zero_remainders = 0;
    uint16_t j;

    for (uint16_t i = 257; i < 512; i += 2)
    {
        for (j = 2; j < 32; j++)
        {
            if (mod(i, j) == 0)
            {
                break;
            }
            if (j == 31)
            {
                ans.push_back(i);
            }
        }
    }
    return ans;
}


int main()
{
    Galois_field GF_poly;

    uint8_t k = 3;
    uint8_t m = 7;

    cout << (unsigned)GF_poly.add(k, m) << endl;
    cout << (unsigned)GF_poly.multiply(k, m, IRRED_POLY) << endl;


    vector <uint16_t>  a = GF_poly.get_irred_polynomes();
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i] << endl;
    }
    cout << a.size() << endl;
    return 0;
}