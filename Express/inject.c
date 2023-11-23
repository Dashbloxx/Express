#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tlhelp32.h>
#include <shlwapi.h>

#pragma comment( lib, "shlwapi.lib")

int inject_dll(DWORD process_id, const char* dll_path)
{
    BOOL wpm = 0;

    SIZE_T dll_path_size = strlen(dll_path) + 1;
    if (dll_path == 0)
    {
        return -1;
    }

    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, 0, process_id);
    if (process_handle == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    LPVOID loc = VirtualAllocEx(process_handle, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(loc == 0)
    {
        return -1;
    }

    wpm = WriteProcessMemory(process_handle, loc, dll_path, dll_path_size, 0);
    if (!wpm)
    {
        CloseHandle(process_handle);
        return -2;
    }

    HANDLE thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
    if (!thread_handle)
    {
        VirtualFree(loc, 0, MEM_RELEASE);
        CloseHandle(process_handle);
        return -3;
    }

    CloseHandle(process_handle);
    VirtualFree(loc, 0, MEM_RELEASE);
    CloseHandle(thread_handle);
    return 0;
}