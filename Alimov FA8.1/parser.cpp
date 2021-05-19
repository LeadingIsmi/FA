#include "parser.h"

using namespace std;

void file_processing(ifstream& instream)
{
	vector<string> tokens_vec;
	string buffer;
	size_t allocation_size;

	if (!instream.is_open())
		return;
	getline(instream, buffer);
	allocation_size = atoi(buffer.c_str());
	if (allocation_size == 0)
	{
		cerr << "Incorrect memory allocation size" << endl;
		exit(1);
	}

	Memory_allocator1 memory(allocation_size); 
	list<Memblock> leaked_blocks;
	vector<Variable> variables_vec;

	while (!instream.eof())
	{
		tokens_vec.clear();
		getline(instream, buffer);
		split_to_vector(tokens_vec, " \t=();", buffer);
		if (tokens_vec.size() == 3)
		{
			vector<Variable>::iterator iter;
			string var_name;
			size_t var_size;

			var_name = tokens_vec[0];
			var_size = atoi(tokens_vec[2].c_str());
			iter = find_in_variables_vector(variables_vec, var_name);
			if (iter == variables_vec.end())
			{
				Variable new_var(var_name);

				try
				{
					new_var = memory.allocate(var_size);
				}
				catch (const exception& e)
				{
					cerr << e.what() << endl;
					continue;
				}
				variables_vec.push_back(new_var);
				vector_sort_by_address(variables_vec);
			}
			else
			{
				Memblock leaked_memblock((*iter).mem_block->get_address(),
					(*iter).mem_block->get_size());

				leaked_blocks.push_back(leaked_memblock);
				try
				{
					*iter = memory.allocate(var_size);
				}
				catch (const exception& e)
				{
					cerr << e.what() << '\n';
					continue;
				}
			}
		}
		else if (tokens_vec.size() == 2)
		{
			if (tokens_vec[0] == "dealloc")
			{
				vector<Variable>::iterator iter;
				string var_name;

				var_name = tokens_vec[1];
				iter = find_in_variables_vector(variables_vec, var_name);
				if (iter != variables_vec.end())
				{
					try
					{
						memory.dealocate((*iter).mem_block);
					}
					catch (const exception& e)
					{
						cerr << e.what() << '\n';
						continue;
					}
					variables_vec.erase(iter);
				}
			}
			else
			{
				vector<Variable>::iterator iter1;
				vector<Variable>::iterator iter2;
				string var_name_1;
				string var_name_2;

				var_name_1 = tokens_vec[0];
				var_name_2 = tokens_vec[1];
				iter1 = find_in_variables_vector(variables_vec, var_name_1);
				if (iter1 != variables_vec.end())
				{
					iter2 = find_in_variables_vector(variables_vec, var_name_2);
					if (iter2 != variables_vec.end())
					{
						Memblock leaked_memblock((*iter1).mem_block->get_address(), (*iter1).mem_block->get_size());
						leaked_blocks.push_back(leaked_memblock);
						(*iter1).mem_block = (*iter2).mem_block;
					}
				}
				else
				{
					iter2 = find_in_variables_vector(variables_vec, var_name_2);
					if (iter2 != variables_vec.end())
					{
						Variable new_var(var_name_1);

						new_var.mem_block = (*iter2).mem_block;
						variables_vec.push_back(*iter2);
						vector_sort_by_address(variables_vec);
					}
				}
			}
		}
	}
	if (!variables_vec.empty())
	{
		vector<Variable>::iterator	iter;

		vector_sort_by_address(variables_vec);
		iter = unique(variables_vec.begin(), variables_vec.end());
		variables_vec.erase(iter, variables_vec.end());
		for (auto& it : variables_vec)
			leaked_blocks.push_back(*(it.mem_block));
	}
	leaked_blocks.sort([](const Memblock& a, const Memblock& b)
		{ return (a.get_address() < b.get_address()); });
	if (!leaked_blocks.empty())
	{
		cout << "\nTotal leaked memory blocks = "  << leaked_blocks.size() << endl;
		for (auto& it : leaked_blocks)
			cout << "Leaked address : " << it.get_address() << ", size = " << it.get_size() << endl;
		cout << endl;
	}
	else
		cout << "No leaked blocks found" << endl;
}

void split_to_vector(vector<string>& vec, string dlm, string src)
{
	size_t p;
	size_t start = 0;
	size_t len = src.length();

	vec.clear();
	start = src.find_first_not_of(dlm);
	p = src.find_first_of(dlm, start);
	while (p != string::npos)
	{
		vec.push_back(src.substr(start, p - start));
		start = src.find_first_not_of(dlm, p);
		p = src.find_first_of(dlm, start);
	}
	if (len > start)
		vec.push_back(src.substr(start, len - start));
}

void vector_sort_by_address(vector<Variable>& vec)
{
	sort(vec.begin(), vec.end(), [](const Variable& var1, const Variable& var2)
		{ return (var1.mem_block->get_address() < var2.mem_block->get_address()); });
}

vector<Variable>::iterator find_in_variables_vector(vector<Variable>& vec, string& var_name)
{
	vector<Variable>::iterator	iter = vec.end();

	iter = find_if(vec.begin(), vec.end(),
		[&var_name](const Variable& var)
		{ return (var_name == var.get_name()); });
	return iter;
}
