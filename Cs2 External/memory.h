#pragma once
#include <Windows.h>
#include <TlHelp32.h>
class Memory
{
public:
	void Init(DWORD64 dwDesiredAccess,const char*name)
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
private:
	DWORD64 _pid;
	HANDLE _handle;
};