#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <exception>
#include "memory.h"
#include "variable.h"
#include "memblock.h"

using namespace std;

void file_processing(ifstream& instream);
void vector_sort_by_address(vector<Variable>& vec);
void split_to_vector(vector<string>& vec, string dlm, string src);

vector<Variable>::iterator find_in_variables_vector(vector<Variable>& vec, string& var_name);

