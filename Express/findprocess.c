#include <windows.h>
#include <tlhelp32.h>

#include "findprocess.h"

int find_process_id(wchar_t *process_name)
{
	PROCESSENTRY32 entry;

	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (wcscmp(entry.szExeFile, process_name) == 0)
			{
				return entry.th32ProcessID;
			}
		}
	}
	return -1;
}