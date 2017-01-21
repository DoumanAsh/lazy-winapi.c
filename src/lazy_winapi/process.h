#pragma once

/**
 * @file
 *
 * Header of @ref Process module.
 */

#include <stdint.h>
#include <stdbool.h>

#include <windows.h>

/**
 * @addtogroup Process
 *
 * Interactions with Windows Processes
 */

/*@{*/

/**
 * Alias to `GetCurrentProcess()`
 *
 * @return Handle to current process.
 */
#define Process_self() GetCurrentProcess()

/**
 * Alias to `GetCurrentProcessId()`
 *
 * @return Pid of current process.
 */
#define Process_self_pid() GetCurrentProcessId()

/**
 * Get pid of window
 *
 * @note If you need both pid & tid then  use directly GetWindowThreadProcessId
 *
 * @param[in] window A handle to the window.
 */
uint32_t Process_get_window_pid(const HWND window);

/**
 * Get tid of window
 *
 * @note If you need both pid & tid then  use directly GetWindowThreadProcessId
 *
 * @param[in] window A handle to the window.
 */
uint32_t Process_get_window_tid(const HWND window);

/**
 * Macro wrapper to open process
 *
 * @note See [information](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684880%28v=vs.85%29.aspx) about access rights.
 *
 * @param[in] _pid Process identifier.
 * @param[in] _access_rights Bit mask that specifies desired access rights.
 *
 * @return Handle to opened process.
 */
#define Process_open(_pid, _access_rights) OpenProcess(_access_rights, 0, _pid)

/**
 * Alias to CloseHandle().
 *
 * @param[in] _handle Process handle from Process_open()
 */
#define Process_close(_handle) CloseHandle(_handle)

/**
 * Reads memory of a process.
 *
 * @param[in] process Handle to the process.
 * @param[in] base A pointer to the base address in the specified process from which to read.
 * @param[out] buffer Memory to hold read result.
 * @param[in] size Number of bytes to read.
 *
 * @return buffer on success.
 * @retval NULL on failure.
 */
const uint8_t* Process_read_mem(HANDLE process, uintptr_t base, uint8_t* buffer, size_t size);

/**
 * Write into memory of a process.
 *
 * @param[in] process Handle to the process.
 * @param[in] base A pointer to the base address in the specified process from which to write.
 * @param[in] buffer Data to write.
 * @param[in] size Number of bytes to write.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Process_write_mem(HANDLE process, uintptr_t base, const uint8_t* buffer, size_t size);

/**
 * Retrieves full path to process's binary.
 *
 * @note Requires access rights either PROCESS_QUERY_INFORMATION or PROCESS_QUERY_LIMITED_INFORMATION
 *
 * @param[in] process Handle to process.
 * @param[out] buffer Buffer to hold result.
 * @param[in] size Size of buffer. Result is truncated by it.
 *
 * @return Pointer to buffer.
 * @retval NULL On failure.
 */
const wchar_t* Process_get_exe_path(HANDLE process, wchar_t* buffer, size_t size);

/*@}*/
