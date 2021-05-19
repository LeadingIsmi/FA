#include <iostream>
#include <fstream>
#include "memblock.h"
#include "memory.h"
#include "variable.h"
#include "parser.h"

using namespace std;

int	main()
{
	ifstream input_file("input.txt");

	if (!input_file.is_open())
		exit(-1);

	file_processing(input_file);

	return 0;
}
