#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include <windows.h>

/**
 * @return Current value of clipboard sequence number.
 * @retval 0 If you do not have WINSTA_ACCESSCLIPBOARD access.
 */
DWORD Clipboard_get_seq_num();

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
 * @note Can be called only after Clipboard_open()
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_empty();

/**
 * Gets clipboard content of specific format.
 *
 * @note Can be called only after Clipboard_open()
 *
 * @param format Format of clipboard to retrieve.
 * @param[out] ptr Memory to hold content of clipboard.
 * @param size Size of memory to hold content.
 *             Content is truncated by this size if necessary.
 *
 * @return Number of copied bytes.
 * @retval 0 On failure.
 */
size_t Clipboard_get(UINT format, uint8_t *ptr, size_t size);

/**
 * Sets clipboard content of specific format.
 *
 * @note Can be called only after Clipboard_open()
 *
 * @param format Format of clipboard to retrieve.
 * @param ptr Data to set.
 * @param size Size of data to set.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_set(UINT format, const uint8_t *ptr, size_t size);

/**
 * Sets string onto clipboard as format CF_TEXT.
 *
 * @note Can be called only after Clipboard_open()
 *
 * @param text String to set.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_set_wide_string(const wchar_t *text);

/**
 * Sets wide string onto clipboard as format CF_UNICODETEXT.
 *
 * @note Can be called only after Clipboard_open()
 *
 * @param text Unicode string to set.
 *
 * @retval true On success.
 * @retval false On failure.
 */
bool Clipboard_set_string(const char *text);

/**
 * @note Can be called only after Clipboard_open()
 *
 * @return Next available clipboard format.
 * @retval 0 On failure or if there is no more formats.
 */
UINT Clipboard_next_avail_format();

/**
 * @retval true Specified format presents on clipboard.
 * @retval false Otherwise.
 */
bool Clipboard_is_format_avail(UINT format);

/**
 * @return Number of formats currently available on clipboard.
 */
int Clipboard_count_avail_formats();

/**
 * Registers new clipboard format.
 *
 * If format with such name already exists, its identifier is returned.
 *
 * @param name New format's name.
 *
 * @return Identifier of new format.
 * @retval 0 On failure.
 */
UINT Clipboard_register_format(const wchar_t *name);
