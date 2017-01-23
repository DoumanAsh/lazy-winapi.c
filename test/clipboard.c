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
    wchar_t get_format_name[50] = {0};
    const uint8_t test_data[] = {1, 2, 3, 55, 2};
    const size_t test_data_len = sizeof(test_data);
    const UINT format = Clipboard_register_format(format_name);
    uint8_t extract_data[50];
    memset(extract_data, 1, sizeof(extract_data));

    cr_assert(format, "Couldn't register new format");
    cr_assert_eq(Clipboard_get_format_name(format, get_format_name, sizeof(get_format_name) / sizeof(get_format_name[0])),
                 wcslen(format_name),
                 "Failed to get new format name");
    cr_assert_wcs_eq(get_format_name, format_name,
                    "Name of new format differs from the one used to register it.");

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

Test(clipboard, get_predefined_format) {
    wchar_t format_name[50] = {0};

#define CAT(_left, _right)   _left##_right
#define ASSERT_FORMAT(_name) cr_assert(Clipboard_get_format_name(_name, format_name, sizeof(format_name) / sizeof(format_name[0])), "Failed to retrieve format "#_name); cr_assert_wcs_eq(format_name, CAT(L, #_name))
    ASSERT_FORMAT(CF_BITMAP);
    ASSERT_FORMAT(CF_DIB);
    ASSERT_FORMAT(CF_DIBV5);
    ASSERT_FORMAT(CF_DIF);
    ASSERT_FORMAT(CF_DSPBITMAP);
    ASSERT_FORMAT(CF_DSPENHMETAFILE);
    ASSERT_FORMAT(CF_DSPMETAFILEPICT);
    ASSERT_FORMAT(CF_DSPTEXT);
    ASSERT_FORMAT(CF_ENHMETAFILE);
    ASSERT_FORMAT(CF_HDROP);
    ASSERT_FORMAT(CF_LOCALE);
    ASSERT_FORMAT(CF_METAFILEPICT);
    ASSERT_FORMAT(CF_OEMTEXT);
    ASSERT_FORMAT(CF_OWNERDISPLAY);
    ASSERT_FORMAT(CF_PALETTE);
    ASSERT_FORMAT(CF_PENDATA);
    ASSERT_FORMAT(CF_RIFF);
    ASSERT_FORMAT(CF_SYLK);
    ASSERT_FORMAT(CF_TEXT);
    ASSERT_FORMAT(CF_WAVE);
    ASSERT_FORMAT(CF_TIFF);
    ASSERT_FORMAT(CF_UNICODETEXT);
#undef ASSERT_FORMAT
#undef CAT

    for (int format = CF_GDIOBJFIRST; format <= CF_GDIOBJLAST; format++) {
        wchar_t expected_format_name[20] = {0};

        swprintf(expected_format_name,
                 sizeof(expected_format_name) / sizeof(expected_format_name[0]),
                 L"CF_GDIOBJ%u", format - CF_GDIOBJFIRST);

        cr_assert_eq(Clipboard_get_format_name(format, format_name, sizeof(format_name) / sizeof(format_name[0])),
                     wcslen(expected_format_name),
                     "Failed to retrieve pre-defined format CF_GDIOBJ!");

        cr_assert_wcs_eq(format_name, expected_format_name, "Unexpectedly wrong CF_GDIOBJ format!");
    }

    for (int format = CF_PRIVATEFIRST; format <= CF_PRIVATELAST; format++) {
        wchar_t expected_format_name[20] = {0};

        swprintf(expected_format_name,
                 sizeof(expected_format_name) / sizeof(expected_format_name[0]),
                 L"CF_PRIVATE%u", format - CF_PRIVATEFIRST);

        cr_assert_eq(Clipboard_get_format_name(format, format_name, sizeof(format_name) / sizeof(format_name[0])),
                     wcslen(expected_format_name),
                     "Failed to retrieve pre-defined format CF_PRIVATE!");

        cr_assert_wcs_eq(format_name, expected_format_name, "Unexpectedly wrong CF_PRIVATE format!");
    }
}

Test(clipboard, get_unknown_format) {
    const UINT format = 0xF000 + 666;
    wchar_t format_name[50] = {0};

    cr_assert_eq(Clipboard_get_format_name(format, format_name, sizeof(format_name) / sizeof(format_name[0])), 0);
}

Test(clipboard, get_format_trunc) {
    const UINT format = CF_TEXT;
    wchar_t format_name[5] = {0};
    const wchar_t expected_format_name[] = L"CF_T";


    cr_assert_eq(Clipboard_get_format_name(format, format_name, sizeof(format_name) / sizeof(format_name[0])),
                 wcslen(expected_format_name),
                 "Couldn't retrieve CF_TEXT format correctly.");

    cr_assert_wcs_eq(format_name, expected_format_name, "Expected truncated CF_T");
}
