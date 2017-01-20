/**
 * @file
 *
 * Source code of @ref Error module.
 */

#include "error.h"

/**
 * Performs error handling for FormatMessageW.
 *
 * @param[out] buffer Memory for string. Cannot be NULL.
 * @param size Size of memory. Cannot be 0.
 *
 * @return buffer
 */
static const wchar_t* format_message_error(wchar_t *buffer, size_t size) {
    switch (Error_get_last()) {
        case 122:
            /* Insufficient space so output is truncated. */
            break;
        case 317:
            /* Unknown error */
        default:
            /* Unclear what are possible errors, so treat
             * as unknown for now. */
            buffer[0] = 0;
            wcsncat(buffer, L"Unknown Error.", size);
            break;
    }

    return buffer;
}

/**
 * Clean FormatMessageW's result of newline.
 *
 * @param[out] buffer Memory for string. Cannot be NULL.
 * @param len Message length taken from FormatMessageW.
 *
 * @return buffer
 */
static const wchar_t* format_message_ok(wchar_t *buffer, size_t len) {
    for (size_t idx = len - 1; idx > 0; idx--) {
        /*                 \n                   \r */
        if (buffer[idx] == 10 || buffer[idx] == 13) buffer[idx] = 0;
        else break;
    }

    return buffer;
}

const wchar_t* Error_get_desc(DWORD error, wchar_t *buffer, size_t size) {
    if (buffer == NULL || size == 0) return NULL;

    const DWORD flags = FORMAT_MESSAGE_IGNORE_INSERTS |
                        FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_ARGUMENT_ARRAY;

    const DWORD len = FormatMessageW(flags, 0, error, 0, buffer, size, 0);

    return len == 0 ? format_message_error(buffer, size) : format_message_ok(buffer, len);
}
