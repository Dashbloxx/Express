#include <windows.h>

#include "gui.h"
#include "mem.h"

HWND g_hwnd, g_inject_button, g_script_box, g_clear_button, g_execute_button, g_about_button;
BYTE code[BUFFER_SIZE];
int g_roblox_process_id;

int main()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    DWORD dwStyle = GetWindowLongPtr(g_hwnd, GWL_STYLE);
    dwStyle &= ~WS_MAXIMIZEBOX;
    SetWindowLongPtr(g_hwnd, GWL_STYLE, dwStyle);

    int nCmdShow = SW_SHOWDEFAULT;

    return WinMain(hInstance, NULL, NULL, nCmdShow);
}