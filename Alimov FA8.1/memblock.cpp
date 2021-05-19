#include "memblock.h"

Memblock::Memblock(void* address, size_t size)
{
	this->address = address;
	this->size = size;
	this->is_filled = false;
	this->prev = nullptr;
	this->next = nullptr;
}

Memblock::Memblock(void* address, size_t size, bool is_filled)
{
	this->address = address;
	this->size = size;
	this->is_filled = is_filled;
	this->prev = nullptr;
	this->next = nullptr;
}

Memblock::Memblock(void* address, size_t size, bool is_filled, void* prev, void* next)
{
	this->address = address;
	this->size = size;
	this->is_filled = is_filled;
	this->prev = prev;
	this->next = next;
}

Memblock::~Memblock() { }

Memblock* Memblock::operator=(const Memblock& right) const
{
	return (new Memblock(right.address, right.size, right.is_filled));
}

bool Memblock::operator==(const Memblock& right) const
{
	if (this->address == right.address && this->size == right.size)
		return true;
	return false;
}

void* Memblock::get_address() const
{
	return address;
}

size_t Memblock::get_size() const
{
	return size;
}