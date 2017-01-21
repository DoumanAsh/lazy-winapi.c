#include <criterion/criterion.h>

#include "lazy_winapi.h"

Test(clipboard, get_seq_num) {
    cr_assert(Clipboard_get_seq_num(), "Cannot get sequence number");
}

/**
 * Test setting of clipboard with text.
 */
Test(clipboard, set_clipboard_text) {
    const DWORD format = CF_TEXT;
    const char text[] = "For my waifu!";
    const size_t text_len = sizeof(text);
    char extract_text[50] = {0};

    cr_assert(Clipboard_open(), "Cannot open clipboard");

    cr_assert(Clipboard_set_string(text), "Cannot set clipboard text");

    cr_assert_eq(Clipboard_next_avail_format(), format, "Next available format should text");
    cr_assert(Clipboard_is_format_avail(format), "Text format isn't available!");
    cr_assert_eq(Clipboard_count_avail_formats(), 1, "Only one format should present");
    cr_assert_eq(Clipboard_get_size(format), text_len, "Unexpected size of clipboard's data!");

    cr_assert_eq(Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text)), text_len);

    cr_assert(Clipboard_close(), "Cannot close clipboard");

    cr_assert_str_eq(extract_text, text);
}

/**
 * Test setting of clipboard with text in UTF-16 encoding.
 */
Test(clipboard, set_clipboard_wide_text) {
    const DWORD format = CF_UNICODETEXT;
    const wchar_t text[] = L"For my waifu!";
    const size_t text_len = sizeof(text);
    wchar_t extract_text[50] = {0};

    cr_assert(Clipboard_open(), "Cannot open clipboard");

    cr_assert(Clipboard_set_wide_string(text), "Cannot set clipboard text");

    cr_assert_eq(Clipboard_next_avail_format(), format, "Next available format should unicode text");
    cr_assert(Clipboard_is_format_avail(format), "Unicode text format isn't available!");
    cr_assert_eq(Clipboard_count_avail_formats(), 1, "Only one format should present");
    cr_assert_eq(Clipboard_get_size(format), text_len, "Unexpected size of clipboard's data!");

    cr_assert_eq(Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text)), text_len);

    cr_assert(Clipboard_close(), "Cannot close clipboard");

    cr_assert_wcs_eq(extract_text, text);
}

/**
 * Test empty clipboard.
 */
Test(clipboard, empty_clipboard) {
    const DWORD format = CF_TEXT;
    const char text[] = "For my waifu!";
    const size_t text_len = sizeof(text);
    char extract_text[50] = {0};

    cr_assert(Clipboard_open(), "Cannot open clipboard");

    cr_assert(Clipboard_set(format, (uint8_t*)text, text_len), "Cannot set clipboard text");

    cr_assert_eq(Clipboard_next_avail_format(), format, "Next available format should text");
    cr_assert(Clipboard_is_format_avail(format), "Text format isn't available!");
    cr_assert_eq(Clipboard_count_avail_formats(), 1, "Only one format should present");

    cr_assert(Clipboard_empty(), "Failed to empty clipboard");

    cr_assert_eq(Clipboard_next_avail_format(), 0, "Next available format should none");
    cr_assert(!Clipboard_is_format_avail(format), "Text format shouldn't be available!");
    cr_assert_eq(Clipboard_count_avail_formats(), 0, "No formats should present on clipboard");
    cr_assert_eq(Clipboard_get_size(format), 0, "Unexpected size of clipboard's data!");

    cr_assert(!Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text)));

    cr_assert(Clipboard_close(), "Cannot close clipboard");

    cr_assert_str_empty(extract_text, "Clipboard isn't empty!");
}

Test(clipboard, register_format) {
    const wchar_t format_name[] = L"testing";
    const uint8_t test_data[] = {1, 2, 3, 55, 2};
    const size_t test_data_len = sizeof(test_data);
    const UINT format = Clipboard_register_format(format_name);
    uint8_t extract_data[50];

    memset(extract_data, 1, sizeof(extract_data));

    cr_assert(format, "Couldn't register new format");
    cr_assert(Clipboard_open(), "Cannot open clipboard");

    cr_assert(Clipboard_set(format, test_data, test_data_len), "Cannot set clipboard data");

    cr_assert_eq(Clipboard_next_avail_format(), format, "Next available format should custom");
    cr_assert(Clipboard_is_format_avail(format), "Custom format isn't available!");
    cr_assert_eq(Clipboard_count_avail_formats(), 1, "Only one format should present");
    cr_assert_eq(Clipboard_get_size(format), test_data_len, "Unexpected size of clipboard's data!");

    cr_assert(Clipboard_get(format, extract_data, test_data_len));
    cr_assert(Clipboard_close(), "Cannot close clipboard");

    cr_assert_arr_eq(extract_data, test_data, test_data_len);
}
