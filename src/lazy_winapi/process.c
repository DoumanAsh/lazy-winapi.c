#include "process.h"

/**
 * @file
 *
 * Source code of @ref Process module.
 */

uint32_t Process_get_window_pid(const HWND window) {
    DWORD result = 0;

    (void)GetWindowThreadProcessId(window, &result);

    return result;
}

uint32_t Process_get_window_tid(const HWND window) {
    return GetWindowThreadProcessId(window, NULL);
}

const uint8_t* Process_read_mem(HANDLE process, uintptr_t base, uint8_t* buffer, size_t size) {
    return ReadProcessMemory(process, (void*)base, buffer, size, NULL) != 0 ? buffer : NULL;
}

bool Process_write_mem(HANDLE process, uintptr_t base, const uint8_t* buffer, size_t size) {
    return WriteProcessMemory(process, (void*)base, buffer, size, NULL) != 0;
}

const wchar_t* Process_get_exe_path(HANDLE process, wchar_t* buffer, size_t size) {
    DWORD temp = (DWORD)size;
    return QueryFullProcessImageNameW(process, 0, buffer, &temp) != 0 ? buffer : NULL;
}
