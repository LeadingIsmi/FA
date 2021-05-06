#include <iostream>
#include "Matrix.h"
#include "Parsing.h"
#include <time.h>
#include <fstream>
#include <stdexcept>

using namespace std;

int main()
{
    ifstream file("input_file.txt");
    ofstream output_file("output_file.tex");

    if (file.is_open())
    {
        parsing(file, output_file);
    }
    else
    {
        throw runtime_error("File can't opening");
    }
    return 0;
}