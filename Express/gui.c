#include <windows.h>

#include "gui.h"
#include "mem.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            if ((HWND)lParam == g_about_button)
            {
                MessageBox(hwnd, L"Express is a new & elite ROBLOX exploit written fully in C with the goal of creating a ROBLOX cheat that is free and open source.", L"About", MB_OK);
            }
            else if ((HWND)lParam == g_clear_button)
            {
                SetWindowText(g_script_box, L"");
            }
            else if ((HWND)lParam == g_inject_button)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_roblox_process_id);
                if (hProcess == NULL) {
                    MessageBox(NULL, L"Failed to attach to ROBLOX instance. Is ROBLOX running right now?", L"Error", MB_ICONERROR | MB_OK);
                    return -1;
                }

                LPVOID pRemoteCode = VirtualAllocEx(hProcess, NULL, BUFFER_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                if (pRemoteCode == NULL) {
                    MessageBox(NULL, L"Failed to allocate memory in ROBLOX instance. Is your RAM usage full?", L"Error", MB_ICONERROR | MB_OK);
                    return -1;
                }

                if (!WriteProcessMemory(hProcess, pRemoteCode, code, sizeof(code), NULL)) {
                    MessageBox(NULL, L"Failed to write to allocated memory in ROBLOX instance.", L"Error", MB_ICONERROR | MB_OK);
                    return -1;
                }

                HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteCode, NULL, 0, NULL);
                if (hRemoteThread == NULL) {
                    MessageBox(NULL, L"Failed to create a thread in ROBLOX instance. Is your CPU usage full?", L"Error", MB_ICONERROR | MB_OK);
                    return -1;
                }

                WaitForSingleObject(hRemoteThread, INFINITE);

                CloseHandle(hRemoteThread);
                VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
                CloseHandle(hProcess);
            }
        }
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MainWindow";

    RegisterClass(&wc);

    g_hwnd = CreateWindowEx(0, "MainWindow", L"Express", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (g_hwnd == NULL)
    {
        return 0;
    }

    g_inject_button = CreateWindow(L"BUTTON", L"Inject", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 30, g_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(g_hwnd, GWLP_HINSTANCE), NULL);
    g_clear_button = CreateWindow(L"BUTTON", L"Clear", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 10, 100, 30, g_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(g_hwnd, GWLP_HINSTANCE), NULL);
    g_execute_button = CreateWindow(L"BUTTON", L"Execute", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 230, 10, 100, 30, g_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(g_hwnd, GWLP_HINSTANCE), NULL);
    g_about_button = CreateWindow(L"BUTTON", L"About", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 340, 10, 100, 30, g_hwnd, NULL, (HINSTANCE)GetWindowLongPtr(g_hwnd, GWLP_HINSTANCE), NULL);
    g_script_box = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, 10, 50, 762.5, 500, g_hwnd, NULL, hInstance, NULL);

    ShowWindow(g_hwnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}