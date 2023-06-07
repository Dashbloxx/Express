#pragma once

#include <windows.h>

#include "mem.h"

extern HWND g_hwnd;
extern HWND g_inject_button;
extern HWND g_script_box;
extern HWND g_execute_button;
extern HWND g_clear_button;
extern HWND g_about_button;

extern BYTE code[BUFFER_SIZE];

extern int g_roblox_process_id;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);