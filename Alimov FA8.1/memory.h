#pragma once

#include <list>
#include "memblock.h"

using namespace std;

class Memory
{
protected:
	void* s;
	size_t size;
	list<Memblock> memblocks;
	list<Memblock> available;
	list<Memblock> unavailable;

	void sort_list();
	virtual void heap_compact() = 0;
	virtual void deallocate_algorithm(Memblock* block) = 0;
	virtual Memblock* search_available_memblock(size_t n) const = 0;
	virtual Memblock* split_memblock(Memblock* block, size_t n) = 0;

public:
	Memory(size_t n);
	virtual ~Memory();

	Memblock* allocate(size_t n);
	void dealocate(Memblock* block);
};

class Memory_allocator1 : public Memory
{
private:
	void print_mem(void* s, size_t n, bool is_filled) const;
	void get_prev_next_neighbours(Memblock* block);

protected:
	void heap_compact() override;
	void deallocate_algorithm(Memblock* block) override;
	Memblock* search_available_memblock(size_t n) const override;
	Memblock* split_memblock(Memblock* block, size_t n) override;

public:
	Memory_allocator1(size_t n) : Memory(n) {}
	~Memory_allocator1() {}
};

class Memory_allocator2 : public Memory
{
private:
	void print_mem(void* s, size_t n) const;

protected:
	void heap_compact() override;
	void deallocate_algorithm(Memblock* block) override;
	Memblock* search_available_memblock(size_t n) const override;
	Memblock* split_memblock(Memblock* block, size_t n) override;

public:
	Memory_allocator2(size_t n) : Memory(n) {}
	~Memory_allocator2() {}
};