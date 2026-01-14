#include "memory.h"
#include <iostream>
using namespace std;
Memory mem;
int main()
{
	mem.Init(PROCESS_ALL_ACCESS, "Discord.exe");
	cout << mem.GetPid();
}