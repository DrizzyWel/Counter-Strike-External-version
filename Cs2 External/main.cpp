#include "memory.h"
#include <iostream>
#include <vector>
using namespace std;
Memory mem;
int main()
{
	mem.Init(PROCESS_ALL_ACCESS, "Discord.exe");
	cout << mem.GetPid();
}