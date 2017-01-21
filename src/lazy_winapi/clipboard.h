#pragma once
/**
 * @file
 *
 * Header of @ref Clipboard module.
 */

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include <windows.h>

/**
 * @addtogroup Clipboard
 *
 * Provides utilities to acess Windows Clipboard.
 *
 * @warning Access to Clipboard from multiple threads isn't safe.
 *
 * General information
 * ------------------
 *
 * ### Open clipboard
 *
 * To access any information inside clipboard it is necessary
 * to open it by means of Clipboard_open().
 *
 * After that Clipboard cannot be opened anymore until Clipboard_close() is called.
 *
 * Examples
 * ---------
 *
 * Depending on the way of installation header can be included either as
 * `clipboard.h` or `lazy_winapi/clipboard.h`.
 *
 * ### Get current text on clipboard
 *
 * ~~~~~~~~~~~~~~~{.c}
    #include "clipboard.h"

    const DWORD format = CF_TEXT;
    char extract_text[50] = {0};

    if (Clipboard_is_format_avail(format)) {
        Clipboard_open();
        Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text));
        Clipboard_close();

        printf("Content of clipboard=%s\n", extract_text);
    }
    else {
        printf("No text on clipboard... \n");
    }
 * ~~~~~~~~~~~~~~~
 *
 * ### Get current unicode text on clipboard
 *
 * ~~~~~~~~~~~~~~~{.c}
    #include "clipboard.h"

    const DWORD format = CF_UNICODETEXT;
    wchar_t extract_text[50] = {0};

    if (Clipboard_is_format_avail(format)) {
        Clipboard_open();
        Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text));
        Clipboard_close();

        printf("Content of clipboard=%ls\n", extract_text);
    }
    else {
        printf("No unicode on clipboard... \n");
    }
 * ~~~~~~~~~~~~~~~
 *
 * ### Set text onto clipboard
 *
 * ~~~~~~~~~~~~~~~{.c}
    #include "clipboard.h"

    const DWORD format = CF_TEXT;
    const char text[] = "For my waifu!";

    Clipboard_open();
    Clipboard_set_string(text);
    Clipboard_close();

 * ~~~~~~~~~~~~~~~
 *
 * ### Set unicode text onto clipboard
 *
 * ~~~~~~~~~~~~~~~{.c}
    #include "clipboard.h"

    const DWORD format = CF_UNICODETEXT;
    const wchar_t text[] = L"For my waifu!";

    Clipboard_open();
    Clipboard_set_wide_string(text);
    Clipboard_close();
 * ~~~~~~~~~~~~~~~
 *
 * ### Raw set onto clipboard
 *
 * ~~~~~~~~~~~~~~~{.c}
    #include "clipboard.h"

    const DWORD format = CF_TEXT
    const unit8_t data[] = {1, 2, 3, 4, 5, 0};

    Clipboard_open();
    Clipboard_set(format, data, sizeof(data));
    Clipboard_close();
 * ~~~~~~~~~~~~~~~
 *
 * ### Use own clipboard format.
 *
 * ~~~~~~~~~~~~~~~{.c}
    #include "clipboard.h"

    const wchar_t format_name[] = L"testing";
    const UINT format = Clipboard_register_format(format_name);

    const uint8_t data[] = {1, 2, 3, 55, 2};

    printf("Got ID=%u for my format\n", format);

    Clipboard_open();
    if (Clipboard_set(format, data, sizeof(data))) {
        printf("Successfully set data to my clipboard format.\n");
    }
    else {
        printf("Failed to set data :(\n");
    }
    Clipboard_close();
 * ~~~~~~~~~~~~~~~
 */
/*@{*/

/**
 * Alias to `GetClipboardSequenceNumber()`.
 * @return Current value of clipboard sequence number.
 * @retval 0 If you do not have WINSTA_ACCESSCLIPBOARD access.
 */
#define Clipboard_get_seq_num() GetClipboardSequenceNumber()

/**
 * Opens clipboard for use in the current thread.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_open();

/**
 * Closes clipboard.
 *
 * @note Can be called only after Clipboard_open()
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_close();

/**
 * Empties clipboard.
 *
 * @note Can be called only after Clipboard_open().
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_empty();

/**
 * Retrieves size in bytes of clipboard content.
 *
 * `GlobalSize` is used to determine size of content.
 *
 * @note Can be called only after Clipboard_open().
 *
 * @param[in] format Format of clipboard to retrieve.
 *
 * @return Size of clipboard content in bytes.
 * @retval 0 Clipboard doesn't hold data of such format.
 */
size_t Clipboard_get_size(UINT format);

/**
 * Gets clipboard content of specific format.
 *
 * @note Can be called only after Clipboard_open().
 *
 * @param[in] format Format of clipboard to retrieve.
 * @param[out] ptr Memory to hold content of clipboard.
 * @param[in] size Size of memory to hold content.
 *                 Content is truncated by this size if necessary.
 *
 * @return Number of copied bytes.
 * @retval 0 On failure.
 */
size_t Clipboard_get(UINT format, uint8_t *ptr, size_t size);

/**
 * Sets clipboard content of specific format.
 *
 * @note Can be called only after Clipboard_open().
 *
 * @param[in] format Format of clipboard to retrieve.
 * @param[out] ptr Data to set.
 * @param[in] size Size of data to set.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_set(UINT format, const uint8_t *ptr, size_t size);

/**
 * Sets string onto clipboard as format CF_TEXT.
 *
 * @note Can be called only after Clipboard_open().
 *
 * @param[in] text String to set.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_set_wide_string(const wchar_t *text);

/**
 * Sets wide string onto clipboard as format CF_UNICODETEXT.
 *
 * @note Can be called only after Clipboard_open().
 *
 * @param[in] text Unicode string to set.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_set_string(const char *text);

/**
 * Alias to `EnumClipboardFormats(0)`.
 * @note Can be called only after Clipboard_open().
 *
 * @return Next available clipboard format.
 * @retval 0 On failure or if there is no more formats.
 */
#define Clipboard_next_avail_format() EnumClipboardFormats(0)

/**
 * @param[in] format Clipboard format identifier.
 * @retval true Specified format presents on clipboard.
 * @retval false Otherwise.
 */
bool Clipboard_is_format_avail(UINT format);

/**
 * Alias to `CountClipboardFormats()`.
 * @return Number of formats currently available on clipboard.
 */
#define Clipboard_count_avail_formats() CountClipboardFormats()

/**
 * Registers new clipboard format.
 *
 * If format with such name already exists, its identifier is returned.
 *
 * @param[in] name New format's name.
 *
 * @return Identifier of new format.
 * @retval 0 On failure.
 */
UINT Clipboard_register_format(const wchar_t *name);

/*@}*/
