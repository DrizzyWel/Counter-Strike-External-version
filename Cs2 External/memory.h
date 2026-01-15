#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
using namespace std;
class Memory
{
public:
	void Init(DWORD64 dwDesiredAccess, const char* name)
	{
		auto Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(Snapshot, &procEntry))
		{
			do
			{
				if (!strcmp(procEntry.szExeFile, name))
				{
					_pid = procEntry.th32ProcessID;
					_handle = OpenProcess(dwDesiredAccess, false, _pid);
					break;
				}

			} while (Process32Next(Snapshot, &procEntry));
		}
		CloseHandle(Snapshot);
	}
	DWORD64 GetModuleHandles(const char* module)
	{
		DWORD64 modBaseAddr = NULL;
		HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _pid);
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(Snapshot, &modEntry))
		{
			do
			{
				if (!strcmp(modEntry.szModule, module))
				{
					_modBaseAddr =(DWORD64) modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(Snapshot, &modEntry));
		}
		modBaseAddr = _modBaseAddr;
		CloseHandle(Snapshot);
		return modBaseAddr;
	}
	DWORD64 GetPointerAddress(DWORD64 startAddress, vector<DWORD64>offsets)
	{
		DWORD64 baseAddr;
		ReadProcessMemory(_handle, (LPCVOID)(_modBaseAddr + startAddress), &baseAddr, sizeof(baseAddr), 0);
		DWORD64 pointeraddr = baseAddr;
		for (int i = 0; i < offsets.size()-1; i++)
		{
			ReadProcessMemory(_handle, (LPCVOID)(baseAddr + offsets.at(i)), &baseAddr, sizeof(baseAddr), 0);
		}
		return pointeraddr += offsets.at(offsets.size() - 1);
	}
	void exitHandle()
	{
		CloseHandle(_handle);
	}

	DWORD GetPid()
	{
		DWORD pid = _pid;
		return pid;
	}
	template<class T> T RPM(DWORD address)
	{
		T buf;
		ReadProcessMemory(_handle, (LPVOID*)address, buf, sizeof(T), 0);
		return buf;
	}
	template <class T>T WPM(DWORD address, T buf)
	{
		T buf;
		WriteProcessMemory(_handle, (LPVOID*)address, buf, sizeof(T), 0);
	}
	 
	

private:
	DWORD64 _pid;
	HANDLE _handle;
	DWORD64 _modBaseAddr;
};