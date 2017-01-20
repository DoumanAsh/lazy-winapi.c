#pragma once

/**
 * @file
 *
 * Header of @ref Error module.
 */

#include <wchar.h>

#include <windows.h>

/**
 * @addtogroup Error
 *
 * This module provides utilities to handle WinAPI errors.
 *
 * You can find out meaning of error over [there](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx)
 */

/*@{*/

/**
 * Alias to GetLastError()
 *
 * @return Last error code of the calling thread.
 */
#define Error_get_last GetLastError

/**
 * Gets description of error.
 *
 * @note Not all error code produces valid error message.
 *       "Unknown Error" is returned in case of that.
 *
 * @warning Underlying FormatMessageW will set new value of Error.
 *
 * @param[in] error Error code for which te get description.
 * @param[out] buffer Memory for string. Cannot be NULL.
 * @param[in] size Size of memory. Cannot be 0.
 *                 Description is truncated in case of insufficient size.
 *
 * @return Pointer to buffer.
 * @retval NULL on invalid parameters.
 */
const wchar_t* Error_get_desc(DWORD error, wchar_t *buffer, size_t size);

/*@}*/
