#include <mimick.h>
#include <mimick/assert.h>

#include <criterion/criterion.h>

#include "lazy_winapi.h"

mmk_mock_define(GetClipboardData_mock, HANDLE WINAPI, UINT);
mmk_mock_define(GlobalLock_mock, LPVOID, HGLOBAL);

#define CREATE_MOCK(name) cr_assert( (mmk_fn) mmk_mock(#name "@self", name ## _mock) != MMK_MOCK_INVALID, #name " mock creation failed")
#define MOCK_RETURN_PTR(_data, _type) &(_type){ &_data }

static const DWORD format = CF_TEXT;

static void setup() {
    const char text[] = "For my waifu!";

    cr_assert(Clipboard_open(), "Cannot open clipboard");
    cr_assert(Clipboard_set_string(text), "Cannot set clipboard text");
}

static void teardown() {
    cr_assert(Clipboard_close(), "Cannot close clipboard");
}

TestSuite(clipboard_mock, .init = setup, .fini = teardown);

/**
 * Test setting of clipboard with text.
 *
 * GetClipboardData mock returns NULL.
 */
Test(clipboard_mock, get_fail_no_data) {
    char extract_text[50] = {0};

    CREATE_MOCK(GetClipboardData);
    mmk_when(GetClipboardData(format), .then_return = NULL);

    cr_assert_eq(Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text)), 0);

    cr_assert(mmk_verify(GetClipboardData(format), .times = 1),
              "GetClipboardData incorrect invokation");

    mmk_reset(GetClipboardData);
}

/**
 * Test setting of clipboard with text.
 *
 * GetClipboardData mock returns dummy.
 * GlobalLock mock returns NULL
 */
Test(clipboard_mock, get_fail_no_lock) {
    char extract_text[50] = {0};

    int mock_data = 1;

    CREATE_MOCK(GetClipboardData);
    mmk_when(GetClipboardData(format), .then_return = MOCK_RETURN_PTR(mock_data, HANDLE) );

    CREATE_MOCK(GlobalLock);
    mmk_when(GlobalLock(mmk_eq(HGLOBAL, &mock_data)), .then_return = NULL);

    cr_assert_eq(Clipboard_get(format, (uint8_t*)extract_text, sizeof(extract_text)), 0);

    cr_assert(mmk_verify(GetClipboardData(mmk_eq(UINT, format)), .times = 1),
              "GetClipboardData incorrect invokation");
    cr_assert(mmk_verify(GlobalLock(mmk_eq(HGLOBAL, &mock_data)), .times = 1),
              "GlobalLock incorrect invokation");

    mmk_reset(GetClipboardData);
    mmk_reset(GlobalLock);
}
