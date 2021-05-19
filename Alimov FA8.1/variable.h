#pragma once

#include <string>
#include "memory.h"

using namespace std;

class Variable
{
private:
	string name;

public:
	Memblock* mem_block;

	Variable(string name)
	{
		this->name = name;
		this->mem_block = nullptr;
	};
	~Variable() {};

	void operator=(Memblock* mem_block);
	bool operator==(const Variable& var) const;
	void set_name(const string& name);
	string get_name() const;
};
