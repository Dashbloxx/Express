#include <windows.h>
#include <stdio.h>

HANDLE main_thread_handle;

DWORD WINAPI main_thread(LPVOID parameter)
{
    while (1)
    {
        printf("Thread ticked!\r\n");
        Sleep(1000);
    }

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();

        freopen_s(stdout, "CONOUT$", "w", stdout);

        main_thread_handle = CreateThread(NULL, 0, main_thread, NULL, 0, NULL);
        if (main_thread_handle == NULL)
        {
            fprintf(stderr, "Failed to create a thread!\r\n");
        }
        break;
    case DLL_PROCESS_DETACH:
        FreeConsole();
        WaitForSingleObject(main_thread_handle , INFINITE);
        CloseHandle(main_thread_handle);
        break;
    }

    return TRUE;
}