#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "os_mem.h"

using namespace std;

struct stranica
{
	int virtualNumber;
	bool edited;
};

bool created = false;
int page_size = 0;
int current_pos = 0;
vector<stranica*> pages;

int create(int size, int num_pages)
{
	if (created)
		return 0;
	if (size <= 0)
		return 0;
	if (num_pages <= 0)
		return 0;

	created = true;
	page_size = size;
	for (int i = 0; i < num_pages; i++)
	{
		pages.push_back(new stranica());
		pages[i]->edited = false;
		pages[i]->virtualNumber = -1;
	}

	return 1;
}

int destroy()
{
	if (!created) return 0;

	created = false;
	page_size = 0;
	pages.clear();
	current_pos = 0;

	return 1;
}

int page_num(mem_handle_t block)
{
	if (!created) return -1;

	int startPage = block.addr / page_size;
	int endPage = (block.addr + block.size - 1) / page_size;
	if (startPage != endPage)
		return -1;

	return startPage;
}

int read(int addr, int size)
{
	if (!created) return -1;

	int page = addr / page_size;
	int Endpage = (addr + size - 1) / page_size;
	if (page != Endpage)
		return -1;

	int foundIdx = -1;
	for (auto it = pages.begin(); it != pages.end(); it++)
	{
		if ((*it)->virtualNumber == page)
			foundIdx = distance(pages.begin(), it);
	}

	if (foundIdx != -1)
	{
		cout << "read " << addr << " " << size << " " << foundIdx << endl;
		return 1;
	}

	if (pages.size() <= current_pos)
		current_pos = 0;

	if (pages[current_pos]->edited)
	{
		cout << "save " << current_pos << " " << pages[current_pos]->virtualNumber << endl;
	}

	pages[current_pos]->virtualNumber = page;
	pages[current_pos]->edited = false;

	cout << "load " << page << " " << current_pos << endl;
	cout << "read " << addr << " " << size << " " << current_pos << endl;
	current_pos++;

	return 1;
}

int write(int addr, int size)
{
	if (!created) return -1;

	int page = addr / page_size;
	int Endpage = (addr + size - 1) / page_size;
	if (page != Endpage)
		return -1;

	int foundIdx = -1;
	for (auto it = pages.begin(); it != pages.end(); it++)
	{
		if ((*it)->virtualNumber == page)
			foundIdx = distance(pages.begin(), it);
	}

	if (foundIdx != -1)
	{
		pages[foundIdx]->edited = true;
		cout << "write " << addr << " " << size << " " << foundIdx << endl;
		return 1;
	}

	if (pages.size() <= current_pos)
		current_pos = 0;

	if (pages[current_pos]->edited)
	{
		cout << "save " << current_pos << " " << pages[current_pos]->virtualNumber << endl;
	}

	pages[current_pos]->edited = true;
	pages[current_pos]->virtualNumber = page;

	cout << "load " << page << " " << current_pos << endl;
	cout << "write " << addr << " " << size << " " << current_pos << endl;
	current_pos++;

	return 1;
}

void setup_memory_manager(memory_manager_t* mm)
{
	mm->create = create;
	mm->destroy = destroy;
	mm->page_num = page_num;
	mm->read = read;
	mm->write = write;
}