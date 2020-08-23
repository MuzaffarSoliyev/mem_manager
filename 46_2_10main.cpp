#include <iostream>
#include "os_mem.h"

using namespace std;

int main()
{
	memory_manager_t mm;
	setup_memory_manager(&mm);

	mm.create(10, 3);
	mm.read(5, 2);
	mm.read(12, 2);
	mm.read(23, 2);
	mm.read(35, 2);
	mm.read(6, 2);
	mm.read(16, 2);
	mm.read(43, 2);
	mm.read(7, 2);
	mm.read(11, 2);
	mm.read(24, 2);
	mm.read(36, 2);
	mm.read(45, 2);
	mm.destroy();

	return 0;
}