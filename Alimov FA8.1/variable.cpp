#include "variable.h"

using namespace std;

void Variable::operator=(Memblock* block)
{
	this->mem_block = block;
}

string Variable::get_name() const
{
	return name;
}

bool Variable::operator==(const Variable& var) const
{
	if (mem_block == var.mem_block)
		return true;
	return false;
}

void Variable::set_name(const string& name)
{
	this->name = name;
}
