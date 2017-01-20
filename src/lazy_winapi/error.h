#pragma once

#include <wchar.h>

#include <windows.h>

/**
 * @return Last error code of the calling thread.
 */
DWORD Error_get_last();

/**
 * Gets error description of error.
 *
 * @note Not all error code produces valid error message.
 *       "Unknown Error" is returned in case of that.
 *
 * @param error Error code for which te get description.
 * @param[out] buffer Memory for string. Cannot be NULL.
 * @param size Size of memory. Cannot be 0.
 *             Description is truncated in case of insufficient size.
 *
 * @return Pointer to buffer.
 * @retval NULL on invalid parameters.
 */
const wchar_t* Error_get_desc(DWORD error, wchar_t *buffer, size_t size);
