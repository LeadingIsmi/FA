#include "memory.h"
#include <iostream>
#include <stdlib.h>
#include <exception>
#include <algorithm>

using namespace std;

Memory::Memory(size_t n)
{
	this->size = n;
	s = malloc(n);
	if (!s)
		return;

	Memblock new_block(s, size, false);

	memblocks.push_back(new_block);
	available.push_back(new_block);
}

Memory::~Memory()
{
	if (s != nullptr)
		free(s);
}

void Memory::sort_list()
{
	memblocks.sort([](const Memblock& a, const Memblock& b)
		{ return (a.address < b.address); });

	available.sort([](const Memblock& a, const Memblock& b)
		{ return (a.address < b.address); });

	unavailable.sort([](const Memblock& a, const Memblock& b)
		{ return (a.address < b.address); });
}

Memblock* Memory::allocate(size_t n)
{
	Memblock* found_block = nullptr;
	Memblock* created_block = nullptr;

	found_block = search_available_memblock(n);
	if (!found_block)
	{
		heap_compact();
		found_block = search_available_memblock(n);
		if (!found_block)
			throw bad_alloc();
	}
	created_block = split_memblock(found_block, n);
	return created_block;
}

void Memory::dealocate(Memblock* block)
{
	list<Memblock>::iterator iter;

	if (!block)
		throw runtime_error("Double free() detected(1)");
	deallocate_algorithm(block);
	sort_list();
}

void Memory_allocator1::heap_compact()
{
	list<Memblock>::iterator iter;
	list<Memblock>::iterator iter_next;

	iter = memblocks.begin();
	while (iter != memblocks.end())
	{
		iter_next = next(iter);
		if (iter_next == memblocks.end())
		{
			get_prev_next_neighbours(&*iter);
			break;
		}
		if ((*iter).is_filled == false)
		{
			unsigned char* address1, * address2;
			size_t swap_size;

			address1 = static_cast<unsigned char*>((*iter).address);
			address2 = address1 + (*iter_next).size;
			swap_size = (*iter).size;
			(*iter).size = (*iter_next).size;
			(*iter_next).size = swap_size;
			(*iter).address = static_cast<void*>(address1);
			(*iter_next).address = static_cast<void*>(address2);
			(*iter).is_filled = true;
			(*iter_next).is_filled = false;
			get_prev_next_neighbours(&*iter);
			get_prev_next_neighbours(&*iter_next);
		}
		iter++;
	}
	iter = memblocks.begin();
	while (iter != memblocks.end() && (*iter).is_filled != false)
		iter++;
	if (iter == memblocks.end())
		return;

	void* new_address = (*iter).address;
	size_t	new_size = 0;

	while (iter != memblocks.end())
	{
		new_size += (*iter).size;
		iter = memblocks.erase(iter);
	}
	memblocks.push_back(Memblock(new_address, new_size, false));
}

void Memory_allocator1::deallocate_algorithm(Memblock* block)
{
	list<Memblock>::iterator iter;

	iter = find(memblocks.begin(), memblocks.end(), *block);
	get_prev_next_neighbours(&*iter);

	list<Memblock>::iterator prev_block;
	list<Memblock>::iterator next_block;
	size_t new_size;
	void* new_address;

	new_address = (*iter).address;
	new_size = (*iter).size;
	if ((*iter).prev != nullptr)
	{
		prev_block = find(memblocks.begin(), memblocks.end(), *iter);
		if (prev_block != memblocks.end() && (*prev_block).is_filled == false)
		{
			new_address = (*prev_block).address;
			new_size += (*prev_block).size;
			memblocks.erase(prev_block);
		}
	}
	if ((*iter).next != nullptr)
	{
		next_block = find(memblocks.begin(), memblocks.end(), *iter);
		if (next_block != memblocks.end() && (*next_block).is_filled == false)
		{
			new_size += (*next_block).size;
			memblocks.erase(next_block);
		}
	}

	Memblock new_memblock(new_address, new_size, false);

	memblocks.erase(iter);
	memblocks.push_back(new_memblock);
	sort_list();
	iter = find(memblocks.begin(), memblocks.end(), new_memblock);
	get_prev_next_neighbours(&*iter);
}

void Memory_allocator1::get_prev_next_neighbours(Memblock* block)
{
	list<Memblock>::iterator iter;

	iter = find(memblocks.begin(), memblocks.end(), *block);

	list<Memblock>::iterator prev;
	list<Memblock>::iterator next;

	if (iter != memblocks.begin())
	{
		prev = std::prev(iter);
		(*iter).prev = (*prev).address;
	}

	if (iter != --memblocks.end())
	{
		next = std::next(iter);
		(*iter).next = (*next).address;
	}
}

void Memory_allocator1::print_mem(void* s, size_t n, bool is_filled) const
{
	if (!s || !n)
		return;

	unsigned char* ptr = static_cast<unsigned char*>(s);

	cout << ((is_filled == false) ? "empty\n" : "fill\n");
	for (size_t i = 0; i < n; i++)
	{
		cout << i << "\t-\t" << static_cast<void*>(ptr) << endl;
		ptr++;
	}
	cout << endl;
}

Memblock* Memory_allocator1::search_available_memblock(size_t n) const
{
	size_t	size = 1 + n + 2 * sizeof(void*);

	for (auto& it : memblocks)
		if (it.is_filled == false && it.size >= size)
			return (const_cast<Memblock*>(&it));
	return nullptr;
}

Memblock* Memory_allocator1::split_memblock(Memblock* block, size_t n)
{
	list<Memblock>::iterator iter;
	size_t size;

	size = 1 + n + 2 * sizeof(void*);
	if (block->size == size)
	{
		iter = find(memblocks.begin(), memblocks.end(), *block);
		(*iter).is_filled = true;
		sort_list();
		get_prev_next_neighbours(&*iter);
		return (&*iter);
	}

	unsigned char* next_addr = static_cast<unsigned char*>(block->address) + size;
	Memblock next(static_cast<void*>(next_addr), block->size - size, false);
	Memblock prev(block->address, size, true);

	memblocks.remove(*block);
	memblocks.push_back(prev);
	memblocks.push_back(next);
	sort_list();
	iter = find(memblocks.begin(), memblocks.end(), next);
	get_prev_next_neighbours(&*iter);
	iter = find(memblocks.begin(), memblocks.end(), prev);
	get_prev_next_neighbours(&*iter);
	return (&*iter);
}

void Memory_allocator2::heap_compact()
{
	list<Memblock> all;

	all.insert(all.end(), unavailable.begin(), unavailable.end());
	all.insert(all.end(), available.begin(), available.end());
	all.sort([](const Memblock& a, const Memblock& b)
		{ return (a.address < b.address); });

	list<Memblock>::iterator iter;
	list<Memblock>::iterator iter_next;
	list<Memblock>::iterator iter_check;
	list<Memblock>::iterator iter_check2;
	list<Memblock> new_available;
	list<Memblock> new_unavailable;

	iter = all.begin();
	while (iter != all.end())
	{
		iter_next = next(iter);
		if (iter_next == --all.end())
		{
			iter_check = find(available.begin(), available.end(), *iter_next);
			if (iter_check != available.end())
				new_available.push_back(*iter_check);
			break;
		}
		iter_check = find(available.begin(), available.end(), *iter);
		iter_check2 = find(new_available.begin(), new_available.end(), *iter);
		if (iter_check != available.end() || iter_check2 != new_available.end())
		{
			if (iter_check2 != new_available.end())
				new_available.erase(iter_check2);

			unsigned char* address1, * address2;
			size_t swap_size;

			address1 = static_cast<unsigned char*>((*iter).address);
			address2 = address1 + (*iter_next).size;
			swap_size = (*iter).size;
			(*iter).size = (*iter_next).size;
			(*iter_next).size = swap_size;
			(*iter).address = static_cast<void*>(address1);
			(*iter_next).address = static_cast<void*>(address2);
			new_available.push_back(*iter_next);
		}
		iter++;
	}
	for (iter = all.begin(); iter != all.end(); iter++)
	{
		iter_check = find(new_available.begin(), new_available.end(), *iter);
		if (iter_check == new_available.end())
			new_unavailable.push_back(*iter);
	}
	if (new_available.size() > 1)
	{
		void* new_address = new_available.front().address;
		size_t new_size = 0;

		for (auto& it : new_available)
			new_size += it.size;
		new_available.clear();
		new_available.push_back(Memblock(new_address, new_size));
	}
	available.clear();
	unavailable.clear();
	available.insert(available.end(), new_available.begin(), new_available.end());
	unavailable.insert(unavailable.end(), new_unavailable.begin(), new_unavailable.end());
	sort_list();
}

void Memory_allocator2::deallocate_algorithm(Memblock* block)
{
	list<Memblock>::iterator iter;
	list<Memblock> all;

	iter = find(available.begin(), available.end(), *block);
	all.insert(all.end(), unavailable.begin(), unavailable.end());
	all.insert(all.end(), available.begin(), available.end());
	all.sort([](const Memblock& a, const Memblock& b)
		{ return (a.address < b.address); });
	iter = find(all.begin(), all.end(), *block);

	list<Memblock>::iterator temp_iter;
	size_t new_size = (*iter).size;
	void* new_adress = (*iter).address;

	if (iter != all.begin())
	{
		temp_iter = prev(iter);
		temp_iter = find(available.begin(), available.end(), *temp_iter);
		if (temp_iter != available.end())
		{
			new_adress = (*temp_iter).address;
			new_size += (*temp_iter).size;
			available.remove(*temp_iter);
		}
	}
	if (iter != all.end())
	{
		temp_iter = next(iter);
		temp_iter = find(available.begin(), available.end(), *temp_iter);
		if (temp_iter != available.end())
		{
			new_size += (*temp_iter).size;
			available.remove(*temp_iter);
		}
	}

	Memblock new_block(new_adress, new_size);

	unavailable.remove(*iter);
	available.push_back(new_block);
}

void Memory_allocator2::print_mem(void* s, size_t n) const
{
	if (!s || !n)
		return;

	unsigned char* ptr = static_cast<unsigned char*>(s);

	for (size_t i = 0; i < n; i++)
	{
		cout << i << "\t-\t" << static_cast<void*>(ptr) << endl;
		ptr++;
	}
	cout << endl;
}

Memblock* Memory_allocator2::search_available_memblock(size_t n) const
{
	for (auto& it : available)
		if (it.size >= n)
			return (const_cast<Memblock*>(&it));
	return nullptr;
}

Memblock* Memory_allocator2::split_memblock(Memblock* block, size_t n)
{
	list<Memblock>::iterator iter;

	if (block->size == n)
	{
		iter = find(available.begin(), available.end(), *block);
		available.erase(iter);
		unavailable.push_back(*iter);
		sort_list();
		return (&*iter);
	}

	unsigned char* next_addr = static_cast<unsigned char*>(block->address) + n;
	Memblock next(static_cast<void*>(next_addr), block->size - n, false);
	Memblock prev(block->address, n, true);

	available.remove(*block);
	unavailable.push_back(prev);
	available.push_back(next);
	sort_list();
	iter = find(unavailable.begin(), unavailable.end(), prev);
	return (&*iter);
}
