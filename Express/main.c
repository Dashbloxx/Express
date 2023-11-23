#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <wchar.h>

#pragma comment( lib, "shlwapi.lib")

HANDLE g_pipe;

wchar_t *process_name = L"taskmgr.exe";
const char* dll_name = "Probe.dll";

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
    if (loc == 0)
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

int main()
{
    HANDLE g_pipe = CreateNamedPipe(L"\\\\.\\pipe\\ExpressComm", PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 0, 0, 0, NULL);

    if(g_pipe == NULL || g_pipe == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Failed to create a named pipe!\r\n");
        return -1;
    }

    char current_directory[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, current_directory) == 0)
    {
        fprintf(stderr, "Failed to get current directory...\r\n");
        return -4;
    }

    char full_path[MAX_PATH];
    if (PathCombineA(full_path, current_directory, dll_name) == NULL)
    {
        fprintf(stderr, "Failed to combine current directory path and probe filename.\r\n");
        return -5;
    }

    HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(snapshot_handle == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Failed to create a process snapshot!\r\n");
        return -2;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot_handle, &pe32))
    {
        do
        {
            if(wcscmp(pe32.szExeFile, process_name) == 0)
            {
                int return_value = inject_dll(pe32.th32ProcessID, (const char*) & full_path);
                switch (return_value)
                {
                case 0:
                    printf("Injected to process (pid=%lu).\r\n", pe32.th32ProcessID);
                    break;
                case 1:
                    printf("Failed to inject to process (pid=%lu).\r\n", pe32.th32ProcessID);
                }
            }
        } while (Process32Next(snapshot_handle, &pe32));
    }
    else
    {
        fprintf(stderr, "Failed to obtain process information!\r\n");
        return -3;
    }

    CloseHandle(snapshot_handle);

	return 0;
}