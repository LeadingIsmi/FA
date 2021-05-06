#ifndef PARSING
#define PARSING
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void parsing(std::ifstream& input_file, std::ofstream& out_file);
void split_to_vector(vector<string>& Vect, string separator, const string& buffer);
int string_to_int(string num_str);
double string_to_double(string num_double);
void alloc_array(int size, vector<string> Vect_of_strings, double**& array, int place);
void clear_mem_vector(vector<string>& Vect);
void clear_array(int size, double**& array);

#endif
