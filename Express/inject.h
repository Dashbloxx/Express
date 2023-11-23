#pragma once

#include <windows.h>

int inject_dll(DWORD process_id, const char* dll_path);