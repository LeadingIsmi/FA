#pragma once

#include <iostream>

class Memblock
{
	friend class Memory;
	friend class Memory_allocator1;
	friend class Memory_allocator2;

private:
	void* address;
	size_t size;
	bool is_filled;
	void* prev;
	void* next;

public:
	Memblock(void* address, size_t size);
	Memblock(void* address, size_t size, bool is_filled);
	Memblock(void* address, size_t size, bool is_filled, void* prev, void* next);
	~Memblock();

	size_t get_size() const;
	void* get_address() const;
	bool operator==(const Memblock& right) const;
	Memblock* operator=(const Memblock& right) const;
};
